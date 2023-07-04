/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (C) 2021 Alibaba Group Holding Limited.
 */

#ifndef __DT_BINDINGS_CLOCK_LIGHT_H
#define __DT_BINDINGS_CLOCK_LIGHT_H

#define LIGHT_CLK_DUMMY				0
#define LIGHT_CLK_32K				1
#define LIGHT_CLK_24M				2
#define LIGHT_RC_24M				3

#define LIGHT_VIDEO_PLL_FOUTVCO			4
#define LIGHT_VIDEO_PLL_FOUTPOSTDIV		5
#define LIGHT_VIDEO_PLL_FOUT4			6
#define LIGHT_VIDEO_PLL_BYPASS			7

#define LIGHT_GMAC_PLL_FOUTVCO			8
#define LIGHT_GMAC_PLL_FOUTPOSTDIV		9
#define LIGHT_GMAC_PLL_FOUT1PH0			10
#define LIGHT_GMAC_PLL_FOUT4			11
#define LIGHT_GMAC_PLL_BYPASS			12

#define LIGHT_AUDIO_PLL_FOUTVCO			13
#define LIGHT_AUDIO_PLL_FOUTPOSTDIV		14
#define LIGHT_AUDIO_PLL_FOUT3			15
#define LIGHT_AUDIO_PLL_BYPASS			16

#define LIGHT_SYS_PLL_FOUTVCO			17
#define LIGHT_SYS_PLL_FOUTPOSTDIV		18
#define LIGHT_SYS_PLL_FOUT4			19
#define LIGHT_SYS_PLL_BYPASS			20

#define LIGHT_CPU_PLL0_FOUTVCO			21
#define LIGHT_CPU_PLL0_FOUTPOSTDIV		22
#define LIGHT_CPU_PLL0_FOUT4			23
#define LIGHT_CPU_PLL0_BYPASS			24

#define LIGHT_CPU_PLL1_FOUTVCO			25
#define LIGHT_CPU_PLL1_FOUTPOSTDIV		26
#define LIGHT_CPU_PLL1_FOUT4			27
#define LIGHT_CPU_PLL1_BYPASS			28

#define LIGHT_DDR_PLL_FOUTVCO			29
#define LIGHT_DDR_PLL_FOUTPOSTDIV		30
#define LIGHT_DDR_PLL_FOUT4			31
#define LIGHT_DDR_PLL_BYPASS			32

#define LIGHT_AONSYS_CLK_SWITCH_0		33
#define LIGHT_AONSYS_CLK_SWITCH_1		34
#define LIGHT_AONSYS_CLK			35
#define LIGHT_SHARE_SRAM_CLK			36
#define LIGHT_CLKGEN_RTC_PCLK			37
#define LIGHT_CLKGEN_AOGPIO_PCLK		38
#define LIGHT_CLKGEN_AOI2C_PCLK			39
#define LIGHT_CLKGEN_PVTC_PCLK			40
#define LIGHT_CLKGEN_AOAHB_HCLK			41
#define LIGHT_CLKGEN_AOSRAM_HCLK		42
#define LIGHT_CLKGEN_AOAPB_HCLK			43
#define LIGHT_CLKGEN_AOPAD_PCLK			44
#define LIGHT_CLKGEN_AOTIMER_PCLK		45
#define LIGHT_CLKGEN_AOTIMER_CCLK		46
#define LIGHT_CLKGEN_SRAM_AXI_ACLK		47
#define LIGHT_CLKGEN_CPU2RAM_X2X_ACLK_S		48
#define LIGHT_CLKGEN_AOGPIO_DBCLK		49

#define LIGHT_GMAC_CORECLK			50
#define LIGHT_OSC_CLK_DIV24			51
#define LIGHT_GMAC_PLL_FOUTVCO_DIV5		52
#define LIGHT_C910_CCLK_I0			53
#define LIGHT_C910_CCLK				54
#define LIGHT_CPUSYS_AHB_HCLK			55
#define LIGHT_CPUSYS_CFG_AXI_ACLK		56
#define LIGHT_PERISYS_AHB_HCLK			57
#define LIGHT_CLK_OUT_1				58
#define LIGHT_CLK_OUT_2				59
#define LIGHT_CLK_OUT_3				60
#define LIGHT_CLK_OUT_4				61
#define LIGHT_CPUSYS_AHB_HCLK_DIV		62
#define LIGHT_APB3_CPUSYS_PCLK			63
#define LIGHT_CPUSYS_SUB_AXI_ACLK		64
#define LIGHT_CPUSYS_CFG_AXI_ACLK_DIV		65
#define LIGHT_TEESYS_HCLK			66
#define LIGHT_DMAC_1_CLK			67
#define LIGHT_DMAC_2_CLK			68
#define LIGHT_DMAC_3_CLK			69
#define LIGHT_AXI_PORT4_CLK			70
#define LIGHT_PERISYS_AHB_HCLK_DIV		71
#define LIGHT_PERISYS_APB_PCLK			72
#define LIGHT_CLK_OUT_1_DIV			73
#define LIGHT_CLK_OUT_2_DIV			74
#define LIGHT_CLK_OUT_3_DIV			75
#define LIGHT_CLK_OUT_4_DIV			76
#define LIGHT_CLKGEN_PERISYS_AXI_ACLK		77
#define LIGHT_CLKGEN_PERISYS_AHB_HCLK		78
#define LIGHT_CLKGEN_PERISYS_APB1_HCLK		79
#define LIGHT_CLKGEN_PERISYS_APB2_HCLK		80
#define LIGHT_CLKGEN_USB3_DRD_PHY_REF_CLK	81
#define LIGHT_CLKGEN_USB3_DRD_CTRL_REF_CLK	82
#define LIGHT_CLKGEN_USB3_DRD_SPDCLK		83
#define LIGHT_CLKGEN_USB3_DRD_ACLK		84
#define LIGHT_CLKGEN_EMMC1_X2X_ACLK		85
#define LIGHT_CLKGEN_EMMC0_X2X_ACLK		86
#define LIGHT_CLKGEN_EMMC0_HCLK			87
#define LIGHT_CLKGEN_EMMC1_HCLK			88
#define LIGHT_CLKGEN_GMAC_ACLK			89
#define LIGHT_CLKGEN_PWM_PCLK			90
#define LIGHT_CLKGEN_QSPI0_PCLK			91
#define LIGHT_CLKGEN_QSPI1_PCLK			92
#define LIGHT_CLKGEN_SPI_PCLK			93
#define LIGHT_CLKGEN_UART0_PCLK			94
#define LIGHT_CLKGEN_UART1_PCLK			95
#define LIGHT_CLKGEN_UART2_PCLK			96
#define LIGHT_CLKGEN_UART3_PCLK			97
#define LIGHT_CLKGEN_UART4_PCLK			98
#define LIGHT_CLKGEN_UART5_PCLK			99
#define LIGHT_CLKGEN_GPIO0_PCLK			100
#define LIGHT_CLKGEN_GPIO1_PCLK			101
#define LIGHT_CLKGEN_GPIO2_PCLK			102
#define LIGHT_CLKGEN_I2C0_IC_CLK		103
#define LIGHT_CLKGEN_I2C1_IC_CLK		104
#define LIGHT_CLKGEN_I2C2_IC_CLK		105
#define LIGHT_CLKGEN_I2C3_IC_CLK		106
#define LIGHT_CLKGEN_I2C4_IC_CLK		107
#define LIGHT_CLKGEN_I2C5_IC_CLK		108
#define LIGHT_CLKGEN_PERI2DDR_X2X_ACLK_M	109
#define LIGHT_CLKGEN_AXI_DUMMY_SLV_4_ACLK	110
#define LIGHT_CLKGEN_AXI_DUMMY_SLV_3_ACLK	111
#define LIGHT_CLKGEN_AXI_DUMMY_SLV_2_ACLK	112
#define LIGHT_CLKGEN_AXI_DUMMY_SLV_1_ACLK	113
#define LIGHT_CLKGEN_APB_CPU2FG_HCLK		114
#define LIGHT_CLKGEN_AON2CPU_A2X_ACLK		115
#define LIGHT_CLKGEN_CPU2CFG_X2X_ACLK_M		116
#define LIGHT_CLKGEN_CPU2RAM_X2X_ACLK_M		117
#define LIGHT_CLKGEN_AXI4_CPUSYS2_ACLK		118
#define LIGHT_CLKGEN_X2X_CPUSYS_ACLK_M		119
#define LIGHT_CLKGEN_CHIP_DBG_ACLK		120
#define LIGHT_CLKGEN_AXI4_CFG_BUS_ACLK		121
#define LIGHT_CLKGEN_X2H_CPUSYS_ACLK		122
#define LIGHT_CLKGEN_CPU2TEE_X2H_ACLK		123
#define LIGHT_CLKGEN_CPU2AON_X2H_ACLK		124
#define LIGHT_CLKGEN_CPU2CFG_X2H_ACLK		125
#define LIGHT_CLKGEN_CPU2PERI_X2H_MHCLK		126
#define LIGHT_CLKGEN_AHB2_CPUSYS_HCLK		127
#define LIGHT_CLKGEN_APB3_CPUSYS_HCLK		128
#define LIGHT_CLKGEN_C910_BROM_HCLK		129
#define LIGHT_CLKGEN_DMAC_ACLK			130
#define LIGHT_CLKGEN_MBOX0_PCLK			131
#define LIGHT_CLKGEN_MBOX1_PCLK			132
#define LIGHT_CLKGEN_MBOX2_PCLK			133
#define LIGHT_CLKGEN_MBOX3_PCLK			134
#define LIGHT_CLKGEN_WDT0_PCLK			135
#define LIGHT_CLKGEN_WDT1_PCLK			136
#define LIGHT_CLKGEN_TIMER0_CCLK		137
#define LIGHT_CLKGEN_TIMER1_CCLK		138
#define LIGHT_CLKGEN_TRNG_RB_HCLK		139
#define LIGHT_CLKGEN_ADC_PCLK			140
#define LIGHT_CLKGEN_AXI_ACLK_4			141
#define LIGHT_CLKGEN_AXI_ACLK_3			142
#define LIGHT_CLKGEN_AXI_ACLK_2			143
#define LIGHT_CLKGEN_AXI_ACLK_1			145
#define LIGHT_CLKGEN_AXI_ACLK_0			146
#define LIGHT_CLKGEN_DMAC_1_ACLK		147
#define LIGHT_CLKGEN_DMAC_2_ACLK		148
#define LIGHT_CLKGEN_DMAC_3_ACLK		149
#define LIGHT_CLKGEN_SRAM_AXI_PCLK		150
#define LIGHT_CLKGEN_AHB2_TEESYS_HCLK		151
#define LIGHT_CLKGEN_EFUSE_MPW_PCLK		152
#define LIGHT_CLKGEN_CLK_OUT_4_CLK		153
#define LIGHT_CLKGEN_CLK_OUT_3_CLK		154
#define LIGHT_CLKGEN_CLK_OUT_2_CLK		155
#define LIGHT_CLKGEN_CLK_OUT_1_CLK		156
#define LIGHT_CLKGEN_DDR_APB_PCLK		157
#define LIGHT_CLKGEN_PADCTRL_APSYS_PCLK		158
#define LIGHT_CLKGEN_CHIP_DBG_CCLK		159
#define LIGHT_CHIP_DBG_CCLK			160
#define LIGHT_AXI_ACLK				161

#define LIGHT_CLKGEN_CPU2CFG_X2X_ACLK_S		162
#define LIGHT_CLKGEN_CPU2CFG_X2H_ACLK_S		163
#define LIGHT_CLKGEN_AON2CPU_A2X_HCLK		164
#define LIGHT_CLKGEN_DMAC_HCLK			165
#define LIGHT_CLKGEN_X2H_CPUSYS_MHCLK		166
#define LIGHT_CLKGEN_CPU2TEE_X2H_MHCLK		167
#define LIGHT_CLKGEN_CPU2AON_X2H_MHCLK		168
#define LIGHT_AXI_HCLK				169
#define LIGHT_CLKGEN_CPU2CFG_X2H_MHCLK		170
#define LIGHT_CLKGEN_TIMER0_PCLK		171
#define LIGHT_CLKGEN_TIMER1_PCLK		172
#define LIGHT_CLKGEN_PERI2DDR_X2X_ACLK_S	173
#define LIGHT_CLKGEN_USB3_DRD_PCLK		174
#define LIGHT_CLKGEN_GMAC_HCLK			175
#define LIGHT_CLKGEN_GMAC_PCLK			176
#define LIGHT_CLKGEN_GMAC_CCLK			177
#define LIGHT_CLKGEN_EMMC0_ACLK			178
#define LIGHT_CLKGEN_EMMC0_REF_CLK		179
#define LIGHT_CLKGEN_EMMC0_OSC_CLK		180
#define LIGHT_CLKGEN_EMMC1_ACLK			181
#define LIGHT_CLKGEN_EMMC1_REF_CLK		182
#define LIGHT_CLKGEN_EMMC1_OSC_CLK		183
#define LIGHT_CLKGEN_PWM_CCLK			184
#define LIGHT_CLKGEN_QSPI0_SSI_CLK		185
#define LIGHT_CLKGEN_QSPI1_SSI_CLK		186
#define LIGHT_CLKGEN_SPI_SSI_CLK		187
#define LIGHT_CLKGEN_GPIO0_DBCLK		188
#define LIGHT_CLKGEN_GPIO1_DBCLK		189
#define LIGHT_CLKGEN_GPIO2_DBCLK		190
#define LIGHT_CLKGEN_DMAC_1_HCLK		191
#define LIGHT_CLKGEN_DMAC_2_HCLK		192
#define LIGHT_CLKGEN_DMAC_3_HCLK		193
#define LIGHT_EMMC_CLK_DIV			194
#define LIGHT_EMMC0_OSC_CLK			195
#define LIGHT_EMMC1_OSC_CLK			196
#define LIGHT_PWM_CCLK				197
#define LIGHT_USB3_PHY_REF_CLK			198
#define LIGHT_SPI_CLK				199
#define LIGHT_GPIO_DBCLK			200
#define LIGHT_X2H_HCLK				201

#define LIGHT_CLK_END				202
#endif