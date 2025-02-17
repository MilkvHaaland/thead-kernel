// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (C) 2021 Alibaba Group Holding Limited.
 */

#include <linux/cdev.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/of_address.h>
#include <linux/mm.h>
#include <linux/firmware/thead/ipc.h>
#include <linux/firmware/thead/light_event.h>

#define DEVNAME "light-regdump"

struct light_regdump_meminfo {
    u64 startaddr;
    u64 size;
    u64 cfg_offset;
};

struct light_aon_msg_regdump_cfg {
    struct light_aon_rpc_msg_hdr hdr;
    u64 startaddr;
    u64 cfg_offset;
    u64 size;
} __packed __aligned(1);

struct light_regdump_config {
    struct cdev cdev;
    struct class* class;
    struct resource mem;
    struct light_regdump_meminfo meminfo;
	struct light_aon_ipc *ipc_handle;
    struct light_aon_msg_regdump_cfg msg;
};

enum {
    REGDUMP_IOC_GET_RSV_MEM = 0x100,
    REGDUMP_IOC_CONFIG_DONE,
};

static int regdump_open(struct inode* inode, struct file* file)
{
    struct light_regdump_config* priv = container_of(inode->i_cdev, struct light_regdump_config, cdev);

    file->private_data = priv;

    return 0;
}

static int regdump_release(struct inode* inode, struct file* file)
{
    struct light_regdump_config* priv = container_of(inode->i_cdev, struct light_regdump_config, cdev);

    return 0;
}

static long regdump_ioctl(struct file* file, unsigned int cmd, unsigned long arg)
{
    struct light_regdump_config* priv = (struct light_regdump_config*)file->private_data;
    struct light_aon_ipc *ipc = priv->ipc_handle;
    struct light_aon_rpc_ack_common ack_msg = {0};

    int ret = 0;

    switch (cmd) {
        case REGDUMP_IOC_GET_RSV_MEM:
        {
            printk("%s REGDUMP_IOC_GET_RSV_MEM copy_to_user 0x%x 0x%x\n",
                 __func__, priv->meminfo.startaddr, priv->meminfo.size);
            copy_to_user(arg, &priv->meminfo, sizeof(struct light_regdump_meminfo));
        }    break;
        case REGDUMP_IOC_CONFIG_DONE: // inform 902
        {
            struct light_aon_rpc_msg_hdr *hdr = &priv->msg.hdr;
            hdr->svc = (uint8_t)LIGHT_AON_RPC_SVC_MISC;
            hdr->func = (uint8_t)LIGHT_AON_MISC_FUNC_REGDUMP_CFG;
            hdr->size = LIGHT_AON_RPC_MSG_NUM;
            RPC_SET_BE64(&priv->msg.startaddr, 0, priv->meminfo.startaddr);
            RPC_SET_BE64(&priv->msg.startaddr, 8, priv->meminfo.size);
            RPC_SET_BE64(&priv->msg.startaddr, 16, priv->meminfo.cfg_offset);

            ret = light_aon_call_rpc(ipc, &priv->msg, &ack_msg, true);
            if (ret)
                printk("failed to REGDUMP_IOC_CONFIG_DONE\n");
        } break;
        default:
            break;
    }

    return ret;
}

static int regdump_mmap(struct file* file, struct vm_area_struct* vma)
{
    struct light_regdump_config* priv = (struct light_regdump_config*)file->private_data;

    vma->vm_page_prot = pgprot_writecombine(vma->vm_page_prot);
    if (remap_pfn_range(vma, vma->vm_start, vma->vm_pgoff, vma->vm_end - vma->vm_start, vma->vm_page_prot)) {
		pr_err("-->%s: remap_pfn_range error!\n", __func__);
		return -EIO;
    }
    printk("phy: 0x%lx, size: 0x%lx PAGE_SHIFT: %d vma->vm_pgoff: 0x%lx vma->vm_start: 0x%lx\n", pfn_to_phys(vma->vm_pgoff), vma->vm_end - vma->vm_start, PAGE_SHIFT, vma->vm_pgoff, vma->vm_start);

    return 0;
}

static struct file_operations regdump_fops = {
    .owner = THIS_MODULE,
    .open = regdump_open,
    .release = regdump_release,
    .unlocked_ioctl = regdump_ioctl,
    .mmap = regdump_mmap,
};

static int light_regdump_probe(struct platform_device *pdev)
{
    struct device_node *np;
    int ret;
    struct light_regdump_config *priv;
    struct device *dev = &pdev->dev;
    dev_t devid;
    struct light_aon_ipc *ipc_handle;

	ret = light_aon_get_handle(&ipc_handle);
	if (ret == -EPROBE_DEFER) {
        return ret;
    }

    priv = devm_kzalloc(&pdev->dev, sizeof(struct light_regdump_config), GFP_KERNEL);
    if (priv == NULL)
		return -ENOMEM;

    platform_set_drvdata(pdev, priv);

    np = of_parse_phandle(dev->of_node, "memory-region", 0);
    if (!np)
        return -EINVAL;

    ret = of_address_to_resource(np, 0, &priv->mem);
    if (ret)
        return ret;
    
    printk("%s got mem start 0x%x size 0x%x\n", __func__, priv->mem.start, resource_size(&priv->mem));
    priv->meminfo.startaddr = priv->mem.start;
    priv->meminfo.size = resource_size(&priv->mem);
    priv->meminfo.cfg_offset = 0x2800;
    priv->ipc_handle = ipc_handle;

    ret = alloc_chrdev_region(&devid, 0, 1, DEVNAME);
    if (ret)
        return ret;
    cdev_init(&priv->cdev, &regdump_fops);
    ret = cdev_add(&priv->cdev, devid, 1);
    if (ret)
        return ret;
    priv->class = class_create(THIS_MODULE, DEVNAME);
    if (IS_ERR(priv->class))
        return PTR_ERR(priv->class);
    device_create(priv->class, &pdev->dev, devid, priv, "%s", DEVNAME);

		

    return ret;
}

static const struct of_device_id light_regdump_match[] = {
    {.compatible = "thead,light-regdump"},
    {}
};

static struct platform_driver light_regdump_driver = {
    .probe = light_regdump_probe,
    .driver = {
        .name = DEVNAME,
        .of_match_table = light_regdump_match,
    }
};

builtin_platform_driver(light_regdump_driver);

MODULE_AUTHOR("nanli.yd <nanli.yd@linux.alibaba.com>");
MODULE_DESCRIPTION("Thead Light register dump driver");
MODULE_LICENSE("GPL v2");
