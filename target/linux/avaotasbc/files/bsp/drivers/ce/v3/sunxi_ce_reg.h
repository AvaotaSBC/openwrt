/* SPDX-License-Identifier: GPL-2.0-or-later */
/* Copyright(c) 2020 - 2023 Allwinner Technology Co.,Ltd. All rights reserved. */
/*
 * The register macro of SUNXI SecuritySystem controller.
 *
 * Copyright (C) 2014 Allwinner.
 *
 * Mintow <duanmintao@allwinnertech.com>
 *
 * This file is licensed under the terms of the GNU General Public
 * License version 2.  This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */

#ifndef _SUNXI_SECURITY_SYSTEM_REG_H_
#define _SUNXI_SECURITY_SYSTEM_REG_H_
#include "../sunxi_ce_cdev.h"

/* CE: Crypto Engine, start using CE from sun8iw7/sun8iw9 */
#define CE_REG_TSK			0x00
#ifdef SS_SUPPORT_CE_V3_1
#define CE_REG_CTL			0x04
#endif
#define CE_REG_ICR			0x08
#define CE_REG_ISR			0x0C
#define CE_REG_TLR			0x10
#define CE_REG_TSR			0x14
#define CE_REG_ERR			0x18
#ifdef SS_SUPPORT_CE_V3_1
#define CE_REG_CSS			0x1C
#define CE_REG_CDS			0x20
#endif
#define CE_REG_CSA			0x24
#define CE_REG_CDA			0x28
#ifdef SS_SUPPORT_CE_V3_1
#define CE_REG_TPR			0x2C
#endif
#ifdef SS_SUPPORT_CE_V3_2
#define CE_REG_HCSA			0x34
#define CE_REG_HCDA			0x38
#define CE_REG_ACSA			0x44
#define CE_REG_ACDA			0x48
#define CE_REG_XCSA			0x54
#define CE_REG_XCDA			0x58
#define CE_REG_VER			0x90
#endif

#define CE_CHAN_INT_ENABLE		1

#define CE_CHAN_PENDING			1

#ifndef SS_SUPPORT_CE_V3_1
#define CE_REG_TLR_METHOD_TYPE_SHIFT		8
#endif

#ifdef SS_SUPPORT_CE_V3_1
#define CE_REG_TSR_BUSY			1
#define CE_REG_TSR_IDLE			0
#define CE_REG_TSR_BUSY_SHIFT	0
#define CE_REG_TSR_BUSY_MASK	(0x1 << CE_REG_TSR_BUSY_SHIFT)
#endif

#define CE_REG_ESR_ERR_UNSUPPORT	0
#define CE_REG_ESR_ERR_LEN			1
#define CE_REG_ESR_ERR_KEYSRAM		2
#ifdef SS_SUPPORT_CE_V3_1
#define CE_REG_ESR_ERR_ADDR			3
#else
#define CE_REG_ESR_ERR_ADDR			5
#endif

#ifdef SS_SUPPORT_CE_V3_2
#define CE_REG_ESR_ERR_KEYLADDER		6
#endif

#ifdef SS_SUPPORT_CE_V3_1
#define CE_REG_ESR_CHAN_SHIFT		4
#define CE_REG_ESR_CHAN_MASK(flow)	(0xF << (CE_REG_ESR_CHAN_SHIFT*flow))
#else
#define CE_REG_ESR_CHAN_SHIFT		8
#define CE_REG_ESR_CHAN_MASK(flow)	(0xFF << (CE_REG_ESR_CHAN_SHIFT*flow))
#endif

#ifdef SS_SUPPORT_CE_V3_1
#define CE_REG_CSS_OFFSET_SHIFT		16
#define CE_REG_CDS_OFFSET_SHIFT		16
#endif

/* About the common control word */
#ifdef SS_SUPPORT_CE_V3_1
#define CE_TASK_INT_ENABLE			1
#endif
#define CE_COMM_CTL_TASK_INT_SHIFT	31
#define CE_COMM_CTL_TASK_INT_MASK	(0x1 << CE_COMM_CTL_TASK_INT_SHIFT)

#define CE_CBC_MAC_LEN_SHIFT		17

#define CE_HASH_IV_DEFAULT			0
#define CE_HASH_IV_INPUT			1
#define CE_COMM_CTL_IV_MODE_SHIFT	16

#define CE_HMAC_SHA1_LAST			BIT(15)

#define SS_DIR_ENCRYPT				0
#define SS_DIR_DECRYPT				1
#define CE_COMM_CTL_OP_DIR_SHIFT	8

#define SS_METHOD_AES				0
#define SS_METHOD_DES				1
#define SS_METHOD_3DES				2
#define SS_METHOD_MD5				16
#define SS_METHOD_SHA1				17
#define SS_METHOD_SHA224			18
#define SS_METHOD_SHA256			19
#define SS_METHOD_SHA384			20
#define SS_METHOD_SHA512			21
#define SS_METHOD_HMAC_SHA1			22
#define SS_METHOD_HMAC_SHA256		23
#define SS_METHOD_RSA				32
#define SS_METHOD_DH				SS_METHOD_RSA
#ifdef SS_SUPPORT_CE_V3_1
#define SS_METHOD_TRNG				48
#define SS_METHOD_PRNG				49
#define SS_METHOD_ECC				64
#else
#define SS_METHOD_TRNG				28
#define SS_METHOD_PRNG				29
#define SS_METHOD_ECC				33
#define SS_METHOD_RAES				48
#endif
#define CE_COMM_CTL_METHOD_SHIFT		0
#ifdef SS_SUPPORT_CE_V3_1
#define CE_COMM_CTL_METHOD_MASK		0x3F
#else
#define CE_COMM_CTL_METHOD_MASK		0x7F
#endif

#define CE_METHOD_IS_HASH(type) ((type == SS_METHOD_MD5) \
				|| (type == SS_METHOD_SHA1) \
				|| (type == SS_METHOD_SHA224) \
				|| (type == SS_METHOD_SHA256) \
				|| (type == SS_METHOD_SHA384) \
				|| (type == SS_METHOD_SHA512))

#define CE_METHOD_IS_AES(type) ((type == SS_METHOD_AES) \
				|| (type == SS_METHOD_DES) \
				|| (type == SS_METHOD_3DES))

#define CE_METHOD_IS_HMAC(type) ((type == SS_METHOD_HMAC_SHA1) \
			|| (type == SS_METHOD_HMAC_SHA256))

/* About the symmetric control word */

#define CE_KEY_SELECT_INPUT			0
#define CE_KEY_SELECT_SSK			1
#define CE_KEY_SELECT_HUK			2
#define CE_KEY_SELECT_RSSK			3
#ifdef SS_SUPPORT_CE_V3_2
#define CE_KEY_SELECT_SCK0			4
#define CE_KEY_SELECT_SCK1			5
#endif
#define CE_KEY_SELECT_INTERNAL_0	8
#define CE_KEY_SELECT_INTERNAL_1	9
#define CE_KEY_SELECT_INTERNAL_2	10
#define CE_KEY_SELECT_INTERNAL_3	11
#define CE_KEY_SELECT_INTERNAL_4	12
#define CE_KEY_SELECT_INTERNAL_5	13
#define CE_KEY_SELECT_INTERNAL_6	14
#define CE_KEY_SELECT_INTERNAL_7	15
#define CE_SYM_CTL_KEY_SELECT_SHIFT	20

/* The identification string to indicate the key source. */
#define CE_KS_SSK			"KEY_SEL_SSK"
#define CE_KS_HUK			"KEY_SEL_HUK"
#define CE_KS_RSSK			"KEY_SEL_RSSK"
#ifdef SS_SUPPORT_CE_V3_2
#define CE_KS_SCK0			"KEY_SEL_SCK0"
#define CE_KS_SCK1			"KEY_SEL_SCK1"
#endif
#define CE_KS_INTERNAL_0	"KEY_SEL_INTRA_0"
#define CE_KS_INTERNAL_1	"KEY_SEL_INTRA_1"
#define CE_KS_INTERNAL_2	"KEY_SEL_INTRA_2"
#define CE_KS_INTERNAL_3	"KEY_SEL_INTRA_3"
#define CE_KS_INTERNAL_4	"KEY_SEL_INTRA_4"
#define CE_KS_INTERNAL_5	"KEY_SEL_INTRA_5"
#define CE_KS_INTERNAL_6	"KEY_SEL_INTRA_6"
#define CE_KS_INTERNAL_7	"KEY_SEL_INTRA_7"

#define CE_CFB_WIDTH_1				0
#define CE_CFB_WIDTH_8				1
#define CE_CFB_WIDTH_64				2
#define CE_CFB_WIDTH_128			3
#define CE_SYM_CTL_CFB_WIDTH_SHIFT	18

#define CE_SYM_CTL_AES_CTS_LAST		BIT(16)

#ifndef SS_SUPPORT_CE_V3_1
#define CE_SYM_CTL_AES_XTS_LAST		BIT(13)
#define CE_SYM_CTL_AES_XTS_FIRST	BIT(12)
#endif

#define SS_AES_MODE_ECB				0
#define SS_AES_MODE_CBC				1
#define SS_AES_MODE_CTR				2
#define SS_AES_MODE_CTS				3
#define SS_AES_MODE_OFB				4
#define SS_AES_MODE_CFB				5
#define SS_AES_MODE_CBC_MAC			6
#define SS_AES_MODE_XTS				9
#define CE_SYM_CTL_OP_MODE_SHIFT	8

#define CE_CTR_SIZE_16				0
#define CE_CTR_SIZE_32				1
#define CE_CTR_SIZE_64				2
#define CE_CTR_SIZE_128				3
#define CE_SYM_CTL_CTR_SIZE_SHIFT	2

#define CE_AES_KEY_SIZE_128			0
#define CE_AES_KEY_SIZE_192			1
#define CE_AES_KEY_SIZE_256			2
#define CE_SYM_CTL_KEY_SIZE_SHIFT	0

#define CE_IS_AES_MODE(type, mode, M) (CE_METHOD_IS_AES(type) \
					&& (mode == SS_AES_MODE_##M))

/* About the asymmetric control word */

#ifdef SS_SUPPORT_CE_V3_1
#define CE_RSA_PUB_MODULUS_WIDTH_512	0
#define CE_RSA_PUB_MODULUS_WIDTH_1024	1
#define CE_RSA_PUB_MODULUS_WIDTH_2048	2
#define CE_RSA_PUB_MODULUS_WIDTH_3072	3
#define CE_RSA_PUB_MODULUS_WIDTH_4096	4
#define CE_ASYM_CTL_RSA_PM_WIDTH_SHIFT	28
#endif

#define CE_RSA_OP_M_EXP		0 /* modular exponentiation */
#ifdef SS_SUPPORT_CE_V3_1
#define CE_RSA_OP_M_MUL		2 /* modular multiplication */
#else
#define CE_RSA_OP_M_ADD                 1 /* modular add */
#define CE_RSA_OP_M_MINUS               2 /* modular minus */
#define CE_RSA_OP_M_MUL                 3 /* modular multiplication */
#endif
#define CE_ASYM_CTL_RSA_OP_SHIFT		16

#ifdef SS_SUPPORT_CE_V3_1
#define CE_ECC_PARA_WIDTH_160			0
#define CE_ECC_PARA_WIDTH_224			2
#define CE_ECC_PARA_WIDTH_256			3
#define CE_ECC_PARA_WIDTH_521			5
#define CE_ASYM_CTL_ECC_PARA_WIDTH_SHIFT	12
#endif

#ifdef SS_SUPPORT_CE_V3_1
#define CE_ECC_OP_POINT_MUL				0
#define CE_ECC_OP_POINT_ADD				1
#define CE_ECC_OP_POINT_DBL				2
#define CE_ECC_OP_POINT_VER				3
#define CE_ECC_OP_ENC					4
#define CE_ECC_OP_DEC					5
#define CE_ECC_OP_SIGN					6
#define CE_ASYM_CTL_ECC_OP_SHIFT		4
#else
#define CE_ECC_OP_POINT_ADD             0 /* point add */
#define CE_ECC_OP_POINT_DBL             1 /* point double */
#define CE_ECC_OP_POINT_MUL             2 /* point multiplication */
#define CE_ECC_OP_POINT_VER             3 /* point verification */
#define CE_ECC_OP_ENC                   4 /* encryption */
#define CE_ECC_OP_DEC                   5 /* decryption */
#define CE_ECC_OP_SIGN                  6 /* sign */
#define CE_ECC_OP_VERIFY                7 /* verification */
#endif

#define SS_SEED_SIZE			24

/* Function declaration */

u32 ss_reg_rd(u32 offset);
void ss_reg_wr(u32 offset, u32 val);

void ss_key_set(char *key, int size, ce_task_desc_t *task);

int ss_pending_get(void);
void ss_pending_clear(int flow);
void ss_irq_enable(int flow);
void ss_irq_disable(int flow);

void ss_iv_set(char *iv, int size, ce_task_desc_t *task);
void ss_iv_mode_set(int mode, ce_task_desc_t *task);

void ss_cnt_set(char *cnt, int size, ce_task_desc_t *task);
void ss_cnt_get(int flow, char *cnt, int size);

void ss_md_get(char *dst, char *src, int size);
void ss_sha_final(void);
void ss_check_sha_end(void);

void ss_rsa_width_set(int size, ce_task_desc_t *task);
void ss_rsa_op_mode_set(int mode, ce_task_desc_t *task);

void ss_ecc_width_set(int size, ce_task_desc_t *task);
void ss_ecc_op_mode_set(int mode, ce_task_desc_t *task);

void ss_cts_last(ce_task_desc_t *task);
void ss_hmac_sha1_last(ce_task_desc_t *task);

void ss_xts_first(ce_task_desc_t *task);
void ss_xts_last(ce_task_desc_t *task);

void ss_method_set(int dir, int type, ce_task_desc_t *task);

void ss_aes_mode_set(int mode, ce_task_desc_t *task);
void ss_cfb_bitwidth_set(int bitwidth, ce_task_desc_t *task);

void ss_wait_idle(void);
void ss_ctrl_start(ce_task_desc_t *task);
void ss_ctrl_stop(void);
int ss_flow_err(int flow);

void ss_data_len_set(int len, ce_task_desc_t *task);

int ss_reg_print(char *buf, int len);
void ss_keyselect_set(int select, ce_task_desc_t *task);
void ss_keysize_set(int size, ce_task_desc_t *task);

#endif /* end of _SUNXI_SECURITY_SYSTEM_REG_H_ */
