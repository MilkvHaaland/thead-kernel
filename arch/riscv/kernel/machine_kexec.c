// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2019 FORTH-ICS/CARV
 *  Nick Kossifidis <mick@ics.forth.gr>
 */

#include <linux/kexec.h>
#include <asm/kexec.h>		/* For riscv_kexec_* symbol defines */
#include <linux/smp.h>		/* For smp_send_stop () */
#include <asm/cacheflush.h>	/* For local_flush_icache_all() */
#include <asm/barrier.h>	/* For smp_wmb() */
#include <asm/page.h>		/* For PAGE_MASK */
#include <linux/libfdt.h>	/* For fdt_check_header() */
#include <asm/set_memory.h>	/* For set_memory_x() */
#include <linux/compiler.h>	/* For unreachable() */
#include <linux/cpu.h>		/* For cpu_down() */
#include <linux/interrupt.h>
#include <linux/irq.h>

/**
 * kexec_image_info - Print received image details
 */
static void
kexec_image_info(const struct kimage *image)
{
	unsigned long i;

	pr_debug("Kexec image info:\n");
	pr_debug("\ttype:        %d\n", image->type);
	pr_debug("\tstart:       %lx\n", image->start);
	pr_debug("\thead:        %lx\n", image->head);
	pr_debug("\tnr_segments: %lu\n", image->nr_segments);

	for (i = 0; i < image->nr_segments; i++) {
		pr_debug("\t    segment[%lu]: %016lx - %016lx", i,
			image->segment[i].mem,
			image->segment[i].mem + image->segment[i].memsz);
		pr_debug("\t\t0x%lx bytes, %lu pages\n",
			(unsigned long) image->segment[i].memsz,
			(unsigned long) image->segment[i].memsz /  PAGE_SIZE);
	}
}

/**
 * machine_kexec_prepare - Initialize kexec
 *
 * This function is called from do_kexec_load, when the user has
 * provided us with an image to be loaded. Its goal is to validate
 * the image and prepare the control code buffer as needed.
 * Note that kimage_alloc_init has already been called and the
 * control buffer has already been allocated.
 */
int
machine_kexec_prepare(struct kimage *image)
{
	struct kimage_arch *internal = &image->arch;
	struct fdt_header fdt = {0};
	void *control_code_buffer = NULL;
	unsigned int control_code_buffer_sz = 0;
	int i = 0;

	kexec_image_info(image);

	/* Find the Flattened Device Tree and save its physical address */
	for (i = 0; i < image->nr_segments; i++) {
		if (image->segment[i].memsz <= sizeof(fdt))
			continue;

		if (copy_from_user(&fdt, image->segment[i].buf, sizeof(fdt)))
			continue;

		if (fdt_check_header(&fdt))
			continue;

		internal->fdt_addr = (unsigned long) image->segment[i].mem;
		break;
	}

	if (!internal->fdt_addr) {
		pr_err("Device tree not included in the provided image\n");
		return -EINVAL;
	}

	/* Copy the assembler code for relocation to the control page */
	if (image->type != KEXEC_TYPE_CRASH) {
		control_code_buffer = page_address(image->control_code_page);
		control_code_buffer_sz = page_size(image->control_code_page);

		if (unlikely(riscv_kexec_relocate_size > control_code_buffer_sz)) {
			pr_err("Relocation code doesn't fit within a control page\n");
			return -EINVAL;
		}

		memcpy(control_code_buffer, riscv_kexec_relocate,
			riscv_kexec_relocate_size);

		/* Mark the control page executable */
		set_memory_x((unsigned long) control_code_buffer, 1);
	}

	return 0;
}


/**
 * machine_kexec_cleanup - Cleanup any leftovers from
 *			   machine_kexec_prepare
 *
 * This function is called by kimage_free to handle any arch-specific
 * allocations done on machine_kexec_prepare. Since we didn't do any
 * allocations there, this is just an empty function. Note that the
 * control buffer is freed by kimage_free.
 */
void
machine_kexec_cleanup(struct kimage *image)
{
}


/*
 * machine_shutdown - Prepare for a kexec reboot
 *
 * This function is called by kernel_kexec just before machine_kexec
 * below. Its goal is to prepare the rest of the system (the other
 * harts and possibly devices etc) for a kexec reboot.
 */
void machine_shutdown(void)
{
	/*
	 * No more interrupts on this hart
	 * until we are back up.
	 */
	local_irq_disable();

#if defined(CONFIG_HOTPLUG_CPU)
	smp_shutdown_nonboot_cpus(smp_processor_id());
#endif
}

static void machine_kexec_mask_interrupts(void)
{
	unsigned int i;
	struct irq_desc *desc;

	for_each_irq_desc(i, desc) {
		struct irq_chip *chip;
		int ret;

		chip = irq_desc_get_chip(desc);
		if (!chip)
			continue;

		/*
		 * First try to remove the active state. If this
		 * fails, try to EOI the interrupt.
		 */
		ret = irq_set_irqchip_state(i, IRQCHIP_STATE_ACTIVE, false);

		if (ret && irqd_irq_inprogress(&desc->irq_data) &&
		    chip->irq_eoi)
			chip->irq_eoi(&desc->irq_data);

		if (chip->irq_mask)
			chip->irq_mask(&desc->irq_data);

		if (chip->irq_disable && !irqd_irq_disabled(&desc->irq_data))
			chip->irq_disable(&desc->irq_data);
	}
}

/*
 * machine_crash_shutdown - Prepare to kexec after a kernel crash
 *
 * This function is called by crash_kexec just before machine_kexec
 * and its goal is to shutdown non-crashing cpus and save registers.
 */
void
machine_crash_shutdown(struct pt_regs *regs)
{
	local_irq_disable();

#ifdef CONFIG_SMP
	/* shutdown non-crashing cpus */
	crash_smp_send_stop();
#endif
	crash_save_cpu(regs, smp_processor_id());
	machine_kexec_mask_interrupts();

	pr_info("Starting crashdump kernel...\n");
}

/**
 * machine_kexec - Jump to the loaded kimage
 *
 * This function is called by kernel_kexec which is called by the
 * reboot system call when the reboot cmd is LINUX_REBOOT_CMD_KEXEC,
 * or by crash_kernel which is called by the kernel's arch-specific
 * trap handler in case of a kernel panic. It's the final stage of
 * the kexec process where the pre-loaded kimage is ready to be
 * executed. We assume at this point that all other harts are
 * suspended and this hart will be the new boot hart.
 */
void __noreturn
machine_kexec(struct kimage *image)
{
	struct kimage_arch *internal = &image->arch;
	unsigned long jump_addr = (unsigned long) image->start;
	unsigned long first_ind_entry = (unsigned long) &image->head;
	unsigned long this_cpu_id = __smp_processor_id();
	unsigned long this_hart_id = cpuid_to_hartid_map(this_cpu_id);
	unsigned long fdt_addr = internal->fdt_addr;
	void *control_code_buffer = page_address(image->control_code_page);
	riscv_kexec_method kexec_method = NULL;

#ifdef CONFIG_SMP
	WARN(smp_crash_stop_failed(),
		"Some CPUs may be stale, kdump will be unreliable.\n");
#endif

	if (image->type != KEXEC_TYPE_CRASH)
		kexec_method = control_code_buffer;
	else
		kexec_method = (riscv_kexec_method) &riscv_kexec_norelocate;

	pr_notice("Will call new kernel at %08lx from hart id %lx\n",
		  jump_addr, this_hart_id);
	pr_notice("FDT image at %08lx\n", fdt_addr);

	/* Make sure the relocation code is visible to the hart */
	local_flush_icache_all();

	/* Jump to the relocation code */
	pr_notice("Bye...\n");
	kexec_method(first_ind_entry, jump_addr, fdt_addr,
		     this_hart_id, va_pa_offset);
	unreachable();
}
#ifdef CONFIG_HIBERNATION
/*
 * crash_is_nosave
 *
 * Return true only if a page is part of reserved memory for crash dump kernel,
 * but does not hold any data of loaded kernel image.
 *
 * Note that all the pages in crash dump kernel memory have been initially
 * marked as Reserved as memory was allocated via memblock_reserve().
 *
 * In hibernation, the pages which are Reserved and yet "nosave" are excluded
 * from the hibernation iamge. crash_is_nosave() does thich check for crash
 * dump kernel and will reduce the total size of hibernation image.
 */

bool crash_is_nosave(unsigned long pfn)
{
	int i;
	phys_addr_t addr;

	if (!crashk_res.end)
		return false;

	/* in reserved memory? */
	addr = __pfn_to_phys(pfn);
	if ((addr < crashk_res.start) || (crashk_res.end < addr))
		return false;

	if (!kexec_crash_image)
		return true;

	/* not part of loaded kernel image? */
	for (i = 0; i < kexec_crash_image->nr_segments; i++)
		if (addr >= kexec_crash_image->segment[i].mem &&
				addr < (kexec_crash_image->segment[i].mem +
					kexec_crash_image->segment[i].memsz))
			return false;

	return true;
}
#endif
