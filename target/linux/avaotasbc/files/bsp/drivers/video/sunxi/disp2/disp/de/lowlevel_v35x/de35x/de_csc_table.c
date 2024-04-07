/* SPDX-License-Identifier: GPL-2.0-or-later */
/* Copyright(c) 2020 - 2023 Allwinner Technology Co.,Ltd. All rights reserved. */
/*
 * Allwinner SoCs display driver.
 *
 * Copyright (C) 2017 Allwinner.
 *
 * This file is licensed under the terms of the GNU General Public
 * License version 2.  This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */

#include "de_csc_table.h"
#include "de_top.h"

u32 r2r[2][16] = {
	/* input : Full RGB  */
	/* output : Full RGB 709 */
	{
		0x00020000, 0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x00020000, 0x00000000, 0x00000000,
		0x00000000, 0x00000000, 0x00020000, 0x00000000,
		0x00000000, 0x00000000, 0x00000000, 0x00000000,
	},

	/* input : Full RGB  */
	/* output : Limit RGB  */
	{
		0x0002542a, 0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x0002542a, 0x00000000, 0x00000000,
		0x00000000, 0x00000000, 0x0002542a, 0x00000000,
		0xffffffc0, 0xffffffc0, 0xffffffc0, 0x00000000,
	}
};

u32 r2y[14][16] = {
	/* input : Full RGB 709 */
	/* output : Limit YCbCr 601 */
	{
		0x0000837a, 0x0001021d, 0x00003221, 0x00000040,
		0xffffb41c, 0xffff6b03, 0x0000e0e1, 0x00000200,
		0x0000e0e1, 0xffff43b1, 0xffffdb6e, 0x00000200,
		0x00000000, 0x00000000, 0x00000000, 0x00000000,
	},

	/* input : Full RGB 709 */
	/* output : Limit YCbCr 709 */
	{
		0x00005d7c, 0x00013a7c, 0x00001fbf, 0x00000040,
		0xffffcc78, 0xffff52a7, 0x0000e0e1, 0x00000200,
		0x0000e0e1, 0xffff33be, 0xffffeb61, 0x00000200,
		0x00000000, 0x00000000, 0x00000000, 0x00000000,
	},

	/* input : Full RGB 709 */
	/* output : Limit YCbCr 2020 */
	{
		0x00007384, 0x00012a21, 0x00001a13, 0x00000040,
		0xffffc133, 0xffff5dec, 0x0000e0e1, 0x00000200,
		0x0000e0e1, 0xffff3135, 0xffffedea, 0x00000200,
		0x00000000, 0x00000000, 0x00000000, 0x00000000,
	},

	/* input : Full RGB 709 */
	/* output : Limit YCbCr FCC */
	{
		0x000083ea, 0x0001036f, 0x0000305e, 0x00000040,
		0xffffb433, 0xffff6aec, 0x0000e0e1, 0x00000200,
		0x0000e0e1, 0xffff4276, 0xffffdca9, 0x00000200,
		0x00000000, 0x00000000, 0x00000000, 0x00000000,
	},

	/* input : Full RGB 709 */
	/* output : Limit YCbCr 240M */
	{
		0x00005d38, 0x0001343e, 0x00002641, 0x00000040,
		0xffffcbc8, 0xffff5357, 0x0000e0e1, 0x00000200,
		0x0000e0e1, 0xffff37f3, 0xffffe72c, 0x00000200,
		0x00000000, 0x00000000, 0x00000000, 0x00000000,
	},

	/* input : Full RGB 709 */
	/* output : Limit YCbCr Ycgco */
	{
		0x00006dee, 0x0000dbdc, 0x00006dee, 0x00000040,
		0xffff8f90, 0x0000e0e1, 0xffff8f90, 0x00000200,
		0x0000e0e1, 0x00000000, 0xffff1f1f, 0x00000200,
		0x00000000, 0x00000000, 0x00000000, 0x00000000,
	},

	/* input : Full RGB 709 */
	/* output : Limit YCbCr GBR */
	{
		0x00000000, 0x0001b7b8, 0x00000000, 0x00000040,
		0x00000000, 0x00000000, 0x0001c1c2, 0x00000200,
		0x0001c1c2, 0x00000000, 0x00000000, 0x00000200,
		0x00000000, 0x00000000, 0x00000000, 0x00000000,
	},

	/* input : Full RGB 709 */
	/* output : Full YCbCr 601 */
	{
		0x00009917, 0x00012c8b, 0x00003a5e, 0x00000000,
		0xffffa99b, 0xffff5665, 0x00010000, 0x00000200,
		0x00010000, 0xffff29a2, 0xffffd65e, 0x00000200,
		0x00000000, 0x00000000, 0x00000000, 0x00000000,
	},

	/* input : Full RGB 709 */
	/* output : Full YCbCr 709 */
	{
		0x00006cda, 0x00016e2f, 0x000024f7, 0x00000000,
		0xffffc557, 0xffff3aa9, 0x00010000, 0x00000200,
		0x00010000, 0xffff1779, 0xffffe887, 0x00000200,
		0x00000000, 0x00000000, 0x00000000, 0x00000000,
	},

	/* input : Full RGB 709 */
	/* output : Full YCbCr 2020 */
	{
		0x00008681, 0x00015b23, 0x00001e5d, 0x00000000,
		0xffffb882, 0xffff477e, 0x00010000, 0x00000200,
		0x00010000, 0xffff1497, 0xffffeb69, 0x00000200,
		0x00000000, 0x00000000, 0x00000000, 0x00000000,
	},

	/* input : Full RGB 709 */
	/* output : Full YCbCr FCC */
	{
		0x0000999a, 0x00012e14, 0x00003852, 0x00000000,
		0xffffa9b5, 0xffff564b, 0x00010000, 0x00000200,
		0x00010000, 0xffff283b, 0xffffd7c5, 0x00000200,
		0x00000000, 0x00000000, 0x00000000, 0x00000000,
	},

	/* input : Full RGB 709 */
	/* output : Full YCbCr 240M */
	{
		0x00006c8b, 0x000166e9, 0x00002c8b, 0x00000000,
		0xffffc48e, 0xffff3b72, 0x00010000, 0x00000200,
		0x00010000, 0xffff1c44, 0xffffe3bc, 0x00000200,
		0x00000000, 0x00000000, 0x00000000, 0x00000000,
	},

	/* input : Full RGB 709 */
	/* output : Full YCbCr Ycgco */
	{
		0x00008000, 0x00010000, 0x00008000, 0x00000000,
		0xffff8000, 0x00010000, 0xffff8000, 0x00000200,
		0x00010000, 0x00000000, 0xffff0000, 0x00000200,
		0x00000000, 0x00000000, 0x00000000, 0x00000000,
	},

	/* input : Full RGB 709 */
	/* output : Full YCbCr GBR */
	{
		0x00000000, 0x00020000, 0x00000000, 0x00000000,
		0x00000000, 0x00000000, 0x00020000, 0x00000200,
		0x00020000, 0x00000000, 0x00000000, 0x00000200,
		0x00000000, 0x00000000, 0x00000000, 0x00000000,
	}
};

u32 y2yl2l[21][16] = {
	/* input : Limit YCbCr 601 */
	/* output : Limit YCbCr 601 */
	{
		0x00020000, 0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x00020000, 0x00000000, 0x00000000,
		0x00000000, 0x00000000, 0x00020000, 0x00000000,
		0x00000000, 0x00000000, 0x00000000, 0x00000000,
	},

	/* input : Limit YCbCr 601 */
	/* output : Limit YCbCr 709 */
	{
		0x00020000, 0xffffc4d7, 0xffff9589, 0x00000040,
		0x00000000, 0x0002098b, 0x00003aaf, 0x00000200,
		0x00000000, 0x0000266d, 0x00020cf8, 0x00000200,
		0xffffffc0, 0xfffffe00, 0xfffffe00, 0x00000000,
	},

	/* input : Limit YCbCr 601 */
	/* output : Limit YCbCr 2020 */
	{
		0x00020000, 0xffffbfce, 0xffffc5ff, 0x00000040,
		0x00000000, 0x00020521, 0x00001f89, 0x00000200,
		0x00000000, 0x00002c87, 0x00020f07, 0x00000200,
		0xffffffc0, 0xfffffe00, 0xfffffe00, 0x00000000,
	},

	/* input : Limit YCbCr 709 */
	/* output : Limit YCbCr 601 */
	{
		0x00020000, 0x000032d9, 0x00006226, 0x00000040,
		0x00000000, 0x0001face, 0xffffc759, 0x00000200,
		0x00000000, 0xffffdae7, 0x0001f780, 0x00000200,
		0xffffffc0, 0xfffffe00, 0xfffffe00, 0x00000000,
	},

	/* input : Limit YCbCr 709 */
	/* output : Limit YCbCr 709 */
	{
		0x00020000, 0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x00020000, 0x00000000, 0x00000000,
		0x00000000, 0x00000000, 0x00020000, 0x00000000,
		0x00000000, 0x00000000, 0x00000000, 0x00000000,
	},

	/* input : Limit YCbCr 709 */
	/* output : Limit YCbCr 2020 */
	{
		0x00020000, 0xfffff782, 0x00003036, 0x00000040,
		0x00000000, 0x0001fd99, 0xffffe5ca, 0x00000200,
		0x00000000, 0x000005e4, 0x0002015a, 0x00000200,
		0xffffffc0, 0xfffffe00, 0xfffffe00, 0x00000000,
	},

	/* input : Limit YCbCr 2020 */
	/* output : Limit YCbCr 601 */
	{
		0x00020000, 0x00003b03, 0x000034d2, 0x00000040,
		0x00000000, 0x0001fd8c, 0xffffe183, 0x00000200,
		0x00000000, 0xffffd4f3, 0x0001f3fa, 0x00000200,
		0xffffffc0, 0xfffffe00, 0xfffffe00, 0x00000000,
	},

	/* input : Limit YCbCr 2020 */
	/* output : Limit YCbCr 709 */
	{
		0x00020000, 0x00000916, 0xffffd061, 0x00000040,
		0x00000000, 0x0002021c, 0x00001a40, 0x00000200,
		0x00000000, 0xfffffa19, 0x0001fe5a, 0x00000200,
		0xffffffc0, 0xfffffe00, 0xfffffe00, 0x00000000,
	},

	/* input : Limit YCbCr 2020 */
	/* output : Limit YCbCr 2020 */
	{
		0x00020000, 0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x00020000, 0x00000000, 0x00000000,
		0x00000000, 0x00000000, 0x00020000, 0x00000000,
		0x00000000, 0x00000000, 0x00000000, 0x00000000,
	},

	/* input : Limit YCbCr FCC */
	/* output : Limit YCbCr 601 */
	{
		0x00020000, 0x00000410, 0x0000005e, 0x00000040,
		0x00000000, 0x0001fff7, 0xffffffca, 0x00000200,
		0x00000000, 0xfffffd09, 0x0001ff00, 0x00000200,
		0xffffffc0, 0xfffffe00, 0xfffffe00, 0x00000000,
	},

	/* input : Limit YCbCr FCC */
	/* output : Limit YCbCr 709 */
	{
		0x00020000, 0xffffc985, 0xffff9623, 0x00000040,
		0x00000000, 0x0002092b, 0x00003a5a, 0x00000200,
		0x00000000, 0x00002362, 0x00020bed, 0x00000200,
		0xffffffc0, 0xfffffe00, 0xfffffe00, 0x00000000,
	},

	/* input : Limit YCbCr FCC */
	/* output : Limit YCbCr 2020 */
	{
		0x00020000, 0xffffc435, 0xffffc681, 0x00000040,
		0x00000000, 0x000204e9, 0x00001f42, 0x00000200,
		0x00000000, 0x00002979, 0x00020dfb, 0x00000200,
		0xffffffc0, 0xfffffe00, 0xfffffe00, 0x00000000,
	},

	/* input : Limit YCbCr 240M */
	/* output : Limit YCbCr 601 */
	{
		0x00020000, 0x0000259d, 0x00005fd5, 0x00000040,
		0x00000000, 0x0001f9e5, 0xffffc8af, 0x00000200,
		0x00000000, 0xffffe490, 0x0001f9a1, 0x00000200,
		0xffffffc0, 0xfffffe00, 0xfffffe00, 0x00000000,
	},

	/* input : Limit YCbCr 240M */
	/* output : Limit YCbCr 709 */
	{
		0x00020000, 0xfffff0dc, 0xfffffd16, 0x00000040,
		0x00000000, 0x0002002d, 0x0000019b, 0x00000200,
		0x00000000, 0x000009d5, 0x00020248, 0x00000200,
		0xffffffc0, 0xfffffe00, 0xfffffe00, 0x00000000,
	},

	/* input : Limit YCbCr 240M */
	/* output : Limit YCbCr 2020 */
	{
		0x00020000, 0xffffe94a, 0x00002d7c, 0x00000040,
		0x00000000, 0x0001fd45, 0xffffe746, 0x00000200,
		0x00000000, 0x00000fc1, 0x000203a8, 0x00000200,
		0xffffffc0, 0xfffffe00, 0xfffffe00, 0x00000000,
	},

	/* input : Limit YCbCr Ycgco */
	/* output : Limit YCbCr 601 */
	{
		0x00020000, 0x00005719, 0x00005c9b, 0x00000040,
		0x00000000, 0xfffeacc9, 0xfffea99b, 0x00000200,
		0x00000000, 0xfffe5344, 0x000129a2, 0x00000200,
		0xffffffc0, 0xfffffe00, 0xfffffe00, 0x00000000,
	},

	/* input : Limit YCbCr Ycgco */
	/* output : Limit YCbCr 709 */
	{
		0x00020000, 0x0000d772, 0x00004648, 0x00000040,
		0x00000000, 0xfffe7552, 0xfffec557, 0x00000200,
		0x00000000, 0xfffe2ef3, 0x00011779, 0x00000200,
		0xffffffc0, 0xfffffe00, 0xfffffe00, 0x00000000,
	},

	/* input : Limit YCbCr Ycgco */
	/* output : Limit YCbCr 2020 */
	{
		0x00020000, 0x0000b234, 0x000065d1, 0x00000040,
		0x00000000, 0xfffe8efb, 0xfffeb882, 0x00000200,
		0x00000000, 0xfffe292e, 0x00011497, 0x00000200,
		0xffffffc0, 0xfffffe00, 0xfffffe00, 0x00000000,
	},

	/* input : Limit YCbCr GBR */
	/* output : Limit YCbCr 601 */
	{
		0x00012c8b, 0x00003a5e, 0x00009917, 0x00000040,
		0xffff5285, 0x000105d8, 0xffffa99b, 0x00000200,
		0xffff24bd, 0xffffd56b, 0x00010000, 0x00000200,
		0xffffffc0, 0xffffffc0, 0xffffffc0, 0x00000000,
	},

	/* input : Limit YCbCr GBR */
	/* output : Limit YCbCr 709 */
	{
		0x00016e2f, 0x000024f7, 0x00006cda, 0x00000040,
		0xffff3628, 0x000105d8, 0xffffc557, 0x00000200,
		0xffff122a, 0xffffe7fe, 0x00010000, 0x00000200,
		0xffffffc0, 0xffffffc0, 0xffffffc0, 0x00000000,
	},

	/* input : Limit YCbCr GBR */
	/* output : Limit YCbCr 2020 */
	{
		0x00015b23, 0x00001e5d, 0x00008681, 0x00000040,
		0xffff4347, 0x000105d8, 0xffffb882, 0x00000200,
		0xffff0f37, 0xffffeaf1, 0x00010000, 0x00000200,
		0xffffffc0, 0xffffffc0, 0xffffffc0, 0x00000000,
	},
};

u32 y2yf2l[21][16] = {
	/* input : Full YCbCr 601 */
	/* output : Limit YCbCr 601 */
	{
		0x0001b7b8, 0x00000000, 0x00000000, 0x00000040,
		0x00000000, 0x0001c1c2, 0x00000000, 0x00000200,
		0x00000000, 0x00000000, 0x0001c1c2, 0x00000200,
		0x00000000, 0xfffffe00, 0xfffffe00, 0x00000000,
	},

	/* input : Full YCbCr 601 */
	/* output : Limit YCbCr 709 */
	{
		0x0001b7b8, 0xffffcc08, 0xffffa27b, 0x00000040,
		0x00000000, 0x0001ca24, 0x0000338d, 0x00000200,
		0x00000000, 0x000021c1, 0x0001cd26, 0x00000200,
		0x00000000, 0xfffffe00, 0xfffffe00, 0x00000000,
	},

	/* input : Full YCbCr 601 */
	/* output : Limit YCbCr 2020 */
	{
		0x0001b7b8, 0xffffc79c, 0xffffcd0c, 0x00000040,
		0x00000000, 0x0001c643, 0x00001bb4, 0x00000200,
		0x00000000, 0x0000271d, 0x0001cef5, 0x00000200,
		0x00000000, 0xfffffe00, 0xfffffe00, 0x00000000,
	},

	/* input : Full YCbCr 709 */
	/* output : Limit YCbCr 601 */
	{
		0x0001b7b8, 0x00002cab, 0x00005638, 0x00000040,
		0x00000000, 0x0001bd32, 0xffffce3c, 0x00000200,
		0x00000000, 0xffffdf6a, 0x0001ba4a, 0x00000200,
		0x00000000, 0xfffffe00, 0xfffffe00, 0x00000000,
	},

	/* input : Full YCbCr 709 */
	/* output : Limit YCbCr 709 */
	{
		0x0001b7b8, 0x00000000, 0x00000000, 0x00000040,
		0x00000000, 0x0001c1c2, 0x00000000, 0x00000200,
		0x00000000, 0x00000000, 0x0001c1c2, 0x00000200,
		0x00000000, 0xfffffe00, 0xfffffe00, 0x00000000,
	},

	/* input : Full YCbCr 709 */
	/* output : Limit YCbCr 2020 */
	{
		0x0001b7b8, 0xfffff88a, 0x00002a5a, 0x00000040,
		0x00000000, 0x0001bfa5, 0xffffe8fa, 0x00000200,
		0x00000000, 0x0000052d, 0x0001c2f1, 0x00000200,
		0x00000000, 0xfffffe00, 0xfffffe00, 0x00000000,
	},

	/* input : Full YCbCr 2020 */
	/* output : Limit YCbCr 601 */
	{
		0x0001b7b8, 0x000033d6, 0x00002e66, 0x00000040,
		0x00000000, 0x0001bf9a, 0xffffe538, 0x00000200,
		0x00000000, 0xffffda2f, 0x0001b732, 0x00000200,
		0x00000000, 0xfffffe00, 0xfffffe00, 0x00000000,
	},

	/* input : Full YCbCr 2020 */
	/* output : Limit YCbCr 709 */
	{
		0x0001b7b8, 0x000007fb, 0xffffd62b, 0x00000040,
		0x00000000, 0x0001c39d, 0x0000170f, 0x00000200,
		0x00000000, 0xfffffad1, 0x0001c04f, 0x00000200,
		0x00000000, 0xfffffe00, 0xfffffe00, 0x00000000,
	},

	/* input : Full YCbCr 2020 */
	/* output : Limit YCbCr 2020 */
	{
		0x0001b7b8, 0x00000000, 0x00000000, 0x00000040,
		0x00000000, 0x0001c1c2, 0x00000000, 0x00000200,
		0x00000000, 0x00000000, 0x0001c1c2, 0x00000200,
		0x00000000, 0xfffffe00, 0xfffffe00, 0x00000000,
	},

	/* input : Full YCbCr FCC */
	/* output : Limit YCbCr 601 */
	{
		0x0001b7b8, 0x00000392, 0x00000053, 0x00000040,
		0x00000000, 0x0001c1ba, 0xffffffd0, 0x00000200,
		0x00000000, 0xfffffd66, 0x0001c0e1, 0x00000200,
		0x00000000, 0xfffffe00, 0xfffffe00, 0x00000000,
	},

	/* input : Full YCbCr FCC */
	/* output : Limit YCbCr 709 */
	{
		0x0001b7b8, 0xffffd025, 0xffffa302, 0x00000040,
		0x00000000, 0x0001c9d0, 0x00003342, 0x00000200,
		0x00000000, 0x00001f15, 0x0001cc3c, 0x00000200,
		0x00000000, 0xfffffe00, 0xfffffe00, 0x00000000,
	},

	/* input : Full YCbCr FCC */
	/* output : Limit YCbCr 2020 */
	{
		0x0001b7b8, 0xffffcb7a, 0xffffcd7e, 0x00000040,
		0x00000000, 0x0001c612, 0x00001b75, 0x00000200,
		0x00000000, 0x0000246f, 0x0001ce0a, 0x00000200,
		0x00000000, 0xfffffe00, 0xfffffe00, 0x00000000,
	},

	/* input : Full YCbCr 240M */
	/* output : Limit YCbCr 601 */
	{
		0x0001b7b8, 0x0000210a, 0x0000542f, 0x00000040,
		0x00000000, 0x0001bc64, 0xffffcf69, 0x00000200,
		0x00000000, 0xffffe7e5, 0x0001bc29, 0x00000200,
		0x00000000, 0xfffffe00, 0xfffffe00, 0x00000000,
	},

	/* input : Full YCbCr 240M */
	/* output : Limit YCbCr 709 */
	{
		0x0001b7b8, 0xfffff2b4, 0xfffffd71, 0x00000040,
		0x00000000, 0x0001c1ea, 0x00000169, 0x00000200,
		0x00000000, 0x000008a3, 0x0001c3c3, 0x00000200,
		0x00000000, 0xfffffe00, 0xfffffe00, 0x00000000,
	},

	/* input : Full YCbCr 240M */
	/* output : Limit YCbCr 2020 */
	{
		0x0001b7b8, 0xffffec0d, 0x000027f5, 0x00000040,
		0x00000000, 0x0001bf5c, 0xffffea47, 0x00000200,
		0x00000000, 0x00000dd6, 0x0001c4f8, 0x00000200,
		0x00000000, 0xfffffe00, 0xfffffe00, 0x00000000,
	},

	/* input : Full YCbCr Ycgco */
	/* output : Limit YCbCr 601 */
	{
		0x0001b7b8, 0x00004c83, 0x00005159, 0x00000040,
		0x00000000, 0xfffed606, 0xfffed33b, 0x00000200,
		0x00000000, 0xfffe8763, 0x00010573, 0x00000200,
		0x00000000, 0xfffffe00, 0xfffffe00, 0x00000000,
	},

	/* input : Full YCbCr Ycgco */
	/* output : Limit YCbCr 709 */
	{
		0x0001b7b8, 0x0000bd41, 0x00003dbd, 0x00000040,
		0x00000000, 0xfffea54d, 0xfffeeb98, 0x00000200,
		0x00000000, 0xfffe677c, 0x0000f580, 0x00000200,
		0x00000000, 0xfffffe00, 0xfffffe00, 0x00000000,
	},

	/* input : Full YCbCr Ycgco */
	/* output : Limit YCbCr 2020 */
	{
		0x0001b7b8, 0x00009c8a, 0x00005970, 0x00000040,
		0x00000000, 0xfffebbd8, 0xfffee052, 0x00000200,
		0x00000000, 0xfffe626b, 0x0000f2f7, 0x00000200,
		0x00000000, 0xfffffe00, 0xfffffe00, 0x00000000,
	},

	/* input : Full YCbCr GBR */
	/* output : Limit YCbCr 601 */
	{
		0x0001021d, 0x00003221, 0x0000837a, 0x00000040,
		0xffff6b03, 0x0000e0e1, 0xffffb41c, 0x00000200,
		0xffff43b1, 0xffffdb6e, 0x0000e0e1, 0x00000200,
		0x00000000, 0x00000000, 0x00000000, 0x00000000,
	},

	/* input : Full YCbCr GBR */
	/* output : Limit YCbCr 709 */
	{
		0x00013a7c, 0x00001fbf, 0x00005d7c, 0x00000040,
		0xffff52a7, 0x0000e0e1, 0xffffcc78, 0x00000200,
		0xffff33be, 0xffffeb61, 0x0000e0e1, 0x00000200,
		0x00000000, 0x00000000, 0x00000000, 0x00000000,
	},

	/* input : Full YCbCr GBR */
	/* output : Limit YCbCr 2020 */
	{
		0x00012a21, 0x00001a13, 0x00007384, 0x00000040,
		0xffff5dec, 0x0000e0e1, 0xffffc133, 0x00000200,
		0xffff3135, 0xffffedea, 0x0000e0e1, 0x00000200,
		0x00000000, 0x00000000, 0x00000000, 0x00000000,
	}
};

u32 y2yl2f[21][16] = {
	/* input : Limit YCbCr 601 */
	/* output : Full YCbCr 601 */
	{
		0x0002542a, 0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x000246db, 0x00000000, 0x00000200,
		0x00000000, 0x00000000, 0x000246db, 0x00000200,
		0xffffffc0, 0xfffffe00, 0xfffffe00, 0x00000000,
	},

	/* input : Limit YCbCr 601 */
	/* output : Full YCbCr 709 */
	{
		0x0002542a, 0xffffbb1d, 0xffff8409, 0x00000000,
		0x00000000, 0x000251b9, 0x000042ce, 0x00000200,
		0x00000000, 0x00002bbe, 0x0002559f, 0x00000200,
		0xffffffc0, 0xfffffe00, 0xfffffe00, 0x00000000,
	},

	/* input : Limit YCbCr 601 */
	/* output : Full YCbCr 2020 */
	{
		0x0002542a, 0xffffb540, 0xffffbc76, 0x00000000,
		0x00000000, 0x00024cb2, 0x000023e6, 0x00000200,
		0x00000000, 0x000032b1, 0x000257f7, 0x00000200,
		0xffffffc0, 0xfffffe00, 0xfffffe00, 0x00000000,
	},

	/* input : Limit YCbCr 709 */
	/* output : Full YCbCr 601 */
	{
		0x0002542a, 0x00003b35, 0x00007249, 0x00000000,
		0x00000000, 0x000240f2, 0xffffbf81, 0x00000200,
		0x00000000, 0xffffd5c5, 0x00023d2e, 0x00000200,
		0xffffffc0, 0xfffffe00, 0xfffffe00, 0x00000000,
	},

	/* input : Limit YCbCr 709 */
	/* output : Full YCbCr 709 */
	{
		0x0002542a, 0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x000246db, 0x00000000, 0x00000200,
		0x00000000, 0x00000000, 0x000246db, 0x00000200,
		0xffffffc0, 0xfffffe00, 0xfffffe00, 0x00000000,
	},

	/* input : Limit YCbCr 709 */
	/* output : Full YCbCr 2020 */
	{
		0x0002542a, 0xfffff61c, 0x00003823, 0x00000000,
		0x00000000, 0x0002441f, 0xffffe22a, 0x00000200,
		0x00000000, 0x000006b5, 0x00024865, 0x00000200,
		0xffffffc0, 0xfffffe00, 0xfffffe00, 0x00000000,
	},

	/* input : Limit YCbCr 2020 */
	/* output : Full YCbCr 601 */
	{
		0x0002542a, 0x000044b6, 0x00003d81, 0x00000000,
		0x00000000, 0x00024411, 0xffffdd4b, 0x00000200,
		0x00000000, 0xffffcefe, 0x0002392c, 0x00000200,
		0xffffffc0, 0xfffffe00, 0xfffffe00, 0x00000000,
	},

	/* input : Limit YCbCr 2020 */
	/* output : Full YCbCr 709 */
	{
		0x0002542a, 0x00000a94, 0xffffc88d, 0x00000000,
		0x00000000, 0x00024943, 0x00001de2, 0x00000200,
		0x00000000, 0xfffff948, 0x000244fb, 0x00000200,
		0xffffffc0, 0xfffffe00, 0xfffffe00, 0x00000000,
	},

	/* input : Limit YCbCr 2020 */
	/* output : Full YCbCr 2020 */
	{
		0x0002542a, 0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x000246db, 0x00000000, 0x00000200,
		0x00000000, 0x00000000, 0x000246db, 0x00000200,
		0xffffffc0, 0xfffffe00, 0xfffffe00, 0x00000000,
	},

	/* input : Limit YCbCr FCC */
	/* output : Full YCbCr 601 */
	{
		0x0002542a, 0x000004bb, 0x0000006e, 0x00000000,
		0x00000000, 0x000246d2, 0xffffffc2, 0x00000200,
		0x00000000, 0xfffffca0, 0x000245b8, 0x00000200,
		0xffffffc0, 0xfffffe00, 0xfffffe00, 0x00000000,
	},

	/* input : Limit YCbCr FCC */
	/* output : Full YCbCr 709 */
	{
		0x0002542a, 0xffffc091, 0xffff84bc, 0x00000000,
		0x00000000, 0x0002514c, 0x0000426e, 0x00000200,
		0x00000000, 0x00002848, 0x0002546f, 0x00000200,
		0xffffffc0, 0xfffffe00, 0xfffffe00, 0x00000000,
	},

	/* input : Limit YCbCr FCC */
	/* output : Full YCbCr 2020 */
	{
		0x0002542a, 0xffffba61, 0xffffbd0d, 0x00000000,
		0x00000000, 0x00024c73, 0x00002396, 0x00000200,
		0x00000000, 0x00002f37, 0x000256c6, 0x00000200,
		0xffffffc0, 0xfffffe00, 0xfffffe00, 0x00000000,
	},

	/* input : Limit YCbCr 240M */
	/* output : Full YCbCr 601 */
	{
		0x0002542a, 0x00002bcb, 0x00006f96, 0x00000000,
		0x00000000, 0x00023fe8, 0xffffc107, 0x00000200,
		0x00000000, 0xffffe0c3, 0x00023f9b, 0x00000200,
		0xffffffc0, 0xfffffe00, 0xfffffe00, 0x00000000,
	},

	/* input : Limit YCbCr 240M */
	/* output : Full YCbCr 709 */
	{
		0x0002542a, 0xffffee5f, 0xfffffc9b, 0x00000000,
		0x00000000, 0x0002470f, 0x000001d4, 0x00000200,
		0x00000000, 0x00000b31, 0x00024975, 0x00000200,
		0xffffffc0, 0xfffffe00, 0xfffffe00, 0x00000000,
	},

	/* input : Limit YCbCr 240M */
	/* output : Full YCbCr 2020 */
	{
		0x0002542a, 0xffffe58f, 0x000034f6, 0x00000000,
		0x00000000, 0x000243c0, 0xffffe3da, 0x00000200,
		0x00000000, 0x000011ef, 0x00024b05, 0x00000200,
		0xffffffc0, 0xfffffe00, 0xfffffe00, 0x00000000,
	},

	/* input : Limit YCbCr Ycgco */
	/* output : Full YCbCr 601 */
	{
		0x0002542a, 0x0000656b, 0x00006bd4, 0x00000000,
		0x00000000, 0xfffe7dd7, 0xfffe7a39, 0x00000200,
		0x00000000, 0xfffe17ee, 0x000152d2, 0x00000200,
		0xffffffc0, 0xfffffe00, 0xfffffe00, 0x00000000,
	},

	/* input : Limit YCbCr Ycgco */
	/* output : Full YCbCr 709 */
	{
		0x0002542a, 0x0000fadd, 0x000051d5, 0x00000000,
		0x00000000, 0xfffe3eb4, 0xfffe99cb, 0x00000200,
		0x00000000, 0xfffdee96, 0x00013e27, 0x00000200,
		0xffffffc0, 0xfffffe00, 0xfffffe00, 0x00000000,
	},

	/* input : Limit YCbCr Ycgco */
	/* output : Full YCbCr 2020 */
	{
		0x0002542a, 0x0000cf7f, 0x0000768e, 0x00000000,
		0x00000000, 0xfffe5be9, 0xfffe8b30, 0x00000200,
		0x00000000, 0xfffde805, 0x00013ade, 0x00000200,
		0xffffffc0, 0xfffffe00, 0xfffffe00, 0x00000000,
	},

	/* input : Limit YCbCr GBR */
	/* output : Full YCbCr 601 */
	{
		0x00015df3, 0x000043f6, 0x0000b241, 0x00000000,
		0xffff3a83, 0x00012a15, 0xffff9b68, 0x00000200,
		0xffff0665, 0xffffcf86, 0x00012a15, 0x00000200,
		0xffffffc0, 0xffffffc0, 0xffffffc0, 0x00000000,
	},

	/* input : Limit YCbCr GBR */
	/* output : Full YCbCr 709 */
	{
		0x0001aa60, 0x00002b0b, 0x00007ebf, 0x00000000,
		0xffff1a39, 0x00012a15, 0xffffbbb2, 0x00000200,
		0xfffef140, 0xffffe4ab, 0x00012a15, 0x00000200,
		0xffffffc0, 0xffffffc0, 0xffffffc0, 0x00000000,
	},

	/* input : Limit YCbCr GBR */
	/* output : Full YCbCr 2020 */
	{
		0x00019433, 0x0000235a, 0x00009c9d, 0x00000000,
		0xffff2929, 0x00012a15, 0xffffacc2, 0x00000200,
		0xfffeede4, 0xffffe807, 0x00012a15, 0x00000200,
		0xffffffc0, 0xffffffc0, 0xffffffc0, 0x00000000,
	}
};

u32 y2yf2f[21][16] = {
	/* input : Full YCbCr 601 */
	/* output : Full YCbCr 601 */
	{
		0x00020000, 0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x00020000, 0x00000000, 0x00000000,
		0x00000000, 0x00000000, 0x00020000, 0x00000000,
		0x00000000, 0x00000000, 0x00000000, 0x00000000,
	},

	/* input : Full YCbCr 601 */
	/* output : Full YCbCr 709 */
	{
		0x00020000, 0xffffc37d, 0xffff931b, 0x00000000,
		0x00000000, 0x0002098b, 0x00003aaf, 0x00000200,
		0x00000000, 0x0000266d, 0x00020cf8, 0x00000200,
		0x00000000, 0xfffffe00, 0xfffffe00, 0x00000000,
	},

	/* input : Full YCbCr 601 */
	/* output : Full YCbCr 2020 */
	{
		0x00020000, 0xffffbe57, 0xffffc4ac, 0x00000000,
		0x00000000, 0x00020521, 0x00001f89, 0x00000200,
		0x00000000, 0x00002c87, 0x00020f07, 0x00000200,
		0x00000000, 0xfffffe00, 0xfffffe00, 0x00000000,
	},

	/* input : Full YCbCr 709 */
	/* output : Full YCbCr 601 */
	{
		0x00020000, 0x00003402, 0x00006464, 0x00000000,
		0x00000000, 0x0001face, 0xffffc759, 0x00000200,
		0x00000000, 0xffffdae7, 0x0001f780, 0x00000200,
		0x00000000, 0xfffffe00, 0xfffffe00, 0x00000000,
	},

	/* input : Full YCbCr 709 */
	/* output : Full YCbCr 709 */
	{
		0x00020000, 0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x00020000, 0x00000000, 0x00000000,
		0x00000000, 0x00000000, 0x00020000, 0x00000000,
		0x00000000, 0x00000000, 0x00000000, 0x00000000,
	},

	/* input : Full YCbCr 709 */
	/* output : Full YCbCr 2020 */
	{
		0x00020000, 0xfffff750, 0x00003150, 0x00000000,
		0x00000000, 0x0001fd99, 0xffffe5ca, 0x00000200,
		0x00000000, 0x000005e4, 0x0002015a, 0x00000200,
		0x00000000, 0xfffffe00, 0xfffffe00, 0x00000000,
	},

	/* input : Full YCbCr 2020 */
	/* output : Full YCbCr 601 */
	{
		0x00020000, 0x00003c5c, 0x00003607, 0x00000000,
		0x00000000, 0x0001fd8c, 0xffffe183, 0x00000200,
		0x00000000, 0xffffd4f3, 0x0001f3fa, 0x00000200,
		0x00000000, 0xfffffe00, 0xfffffe00, 0x00000000,
	},

	/* input : Full YCbCr 2020 */
	/* output : Full YCbCr 709 */
	{
		0x00020000, 0x0000094b, 0xffffcf4b, 0x00000000,
		0x00000000, 0x0002021c, 0x00001a40, 0x00000200,
		0x00000000, 0xfffffa19, 0x0001fe5a, 0x00000200,
		0x00000000, 0xfffffe00, 0xfffffe00, 0x00000000,
	},

	/* input : Full YCbCr 2020 */
	/* output : Full YCbCr 2020 */
	{
		0x00020000, 0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x00020000, 0x00000000, 0x00000000,
		0x00000000, 0x00000000, 0x00020000, 0x00000000,
		0x00000000, 0x00000000, 0x00000000, 0x00000000,
	},

	/* input : Full YCbCr FCC */
	/* output : Full YCbCr 601 */
	{
		0x00020000, 0x00000428, 0x00000060, 0x00000000,
		0x00000000, 0x0001fff7, 0xffffffca, 0x00000200,
		0x00000000, 0xfffffd09, 0x0001ff00, 0x00000200,
		0x00000000, 0xfffffe00, 0xfffffe00, 0x00000000,
	},

	/* input : Full YCbCr FCC */
	/* output : Full YCbCr 709 */
	{
		0x00020000, 0xffffc847, 0xffff93b8, 0x00000000,
		0x00000000, 0x0002092b, 0x00003a5a, 0x00000200,
		0x00000000, 0x00002362, 0x00020bed, 0x00000200,
		0x00000000, 0xfffffe00, 0xfffffe00, 0x00000000,
	},

	/* input : Full YCbCr FCC */
	/* output : Full YCbCr 2020 */
	{
		0x00020000, 0xffffc2d7, 0xffffc531, 0x00000000,
		0x00000000, 0x000204e9, 0x00001f42, 0x00000200,
		0x00000000, 0x00002979, 0x00020dfb, 0x00000200,
		0x00000000, 0xfffffe00, 0xfffffe00, 0x00000000,
	},

	/* input : Full YCbCr 240M */
	/* output : Full YCbCr 601 */
	{
		0x00020000, 0x00002678, 0x00006205, 0x00000000,
		0x00000000, 0x0001f9e5, 0xffffc8af, 0x00000200,
		0x00000000, 0xffffe490, 0x0001f9a1, 0x00000200,
		0x00000000, 0xfffffe00, 0xfffffe00, 0x00000000,
	},

	/* input : Full YCbCr 240M */
	/* output : Full YCbCr 709 */
	{
		0x00020000, 0xfffff084, 0xfffffd05, 0x00000000,
		0x00000000, 0x0002002d, 0x0000019b, 0x00000200,
		0x00000000, 0x000009d5, 0x00020248, 0x00000200,
		0x00000000, 0xfffffe00, 0xfffffe00, 0x00000000,
	},

	/* input : Full YCbCr 240M */
	/* output : Full YCbCr 2020 */
	{
		0x00020000, 0xffffe8c6, 0x00002e86, 0x00000000,
		0x00000000, 0x0001fd45, 0xffffe746, 0x00000200,
		0x00000000, 0x00000fc1, 0x000203a8, 0x00000200,
		0x00000000, 0xfffffe00, 0xfffffe00, 0x00000000,
	},

	/* input : Full YCbCr Ycgco */
	/* output : Full YCbCr 601 */
	{
		0x00020000, 0x00005917, 0x00005eb8, 0x00000000,
		0x00000000, 0xfffeacc9, 0xfffea99b, 0x00000200,
		0x00000000, 0xfffe5344, 0x000129a2, 0x00000200,
		0x00000000, 0xfffffe00, 0xfffffe00, 0x00000000,
	},

	/* input : Full YCbCr Ycgco */
	/* output : Full YCbCr 709 */
	{
		0x00020000, 0x0000dc5d, 0x000047e3, 0x00000000,
		0x00000000, 0xfffe7552, 0xfffec557, 0x00000200,
		0x00000000, 0xfffe2ef3, 0x00011779, 0x00000200,
		0x00000000, 0xfffffe00, 0xfffffe00, 0x00000000,
	},

	/* input : Full YCbCr Ycgco */
	/* output : Full YCbCr 2020 */
	{
		0x00020000, 0x0000b646, 0x00006824, 0x00000000,
		0x00000000, 0xfffe8efb, 0xfffeb882, 0x00000200,
		0x00000000, 0xfffe292e, 0x00011497, 0x00000200,
		0x00000000, 0xfffffe00, 0xfffffe00, 0x00000000,
	},

	/* input : Full YCbCr GBR */
	/* output : Full YCbCr 601 */
	{
		0x00012c8b, 0x00003a5e, 0x00009917, 0x00000000,
		0xffff5665, 0x00010000, 0xffffa99b, 0x00000200,
		0xffff29a2, 0xffffd65e, 0x00010000, 0x00000200,
		0x00000000, 0x00000000, 0x00000000, 0x00000000,
	},

	/* input : Full YCbCr GBR */
	/* output : Full YCbCr 709 */
	{
		0x00016e2f, 0x000024f7, 0x00006cda, 0x00000000,
		0xffff3aa9, 0x00010000, 0xffffc557, 0x00000200,
		0xffff1779, 0xffffe887, 0x00010000, 0x00000200,
		0x00000000, 0x00000000, 0x00000000, 0x00000000,
	},

	/* input : Full YCbCr GBR */
	/* output : Full YCbCr 2020 */
	{
		0x00015b23, 0x00001e5d, 0x00008681, 0x00000000,
		0xffff477e, 0x00010000, 0xffffb882, 0x00000200,
		0xffff1497, 0xffffeb69, 0x00010000, 0x00000200,
		0x00000000, 0x00000000, 0x00000000, 0x00000000,
	},
};

u32 y2rl2l[7][16] = {
	/* input : Limit YCbCr 601 */
	/* output : Limit RGB 601 */
	{
		0x00020000, 0x00000000, 0x0002bdcd, 0x00000000,
		0x00020000, 0xffff53bc, 0xfffe9a86, 0x00000000,
		0x00020000, 0x00037703, 0x00000000, 0x00000000,
		0xffffffc0, 0xfffffe00, 0xfffffe00, 0x00000000,
	},

	/* input : Limit YCbCr 709 */
	/* output : Limit RGB 601 */
	{
		0x00020000, 0x00000000, 0x0003144d, 0x00000000,
		0x00020000, 0xffffa23b, 0xffff15ac, 0x00000000,
		0x00020000, 0x0003a0dc, 0x00000000, 0x00000000,
		0xffffffc0, 0xfffffe00, 0xfffffe00, 0x00000000,
	},

	/* input : Limit YCbCr 2020 */
	/* output : Limit RGB 601 */
	{
		0x00020000, 0x00000000, 0x0002e225, 0x00000000,
		0x00020000, 0xffffada1, 0xfffee1ff, 0x00000000,
		0x00020000, 0x0003adc6, 0x00000000, 0x00000000,
		0xffffffc0, 0xfffffe00, 0xfffffe00, 0x00000000,
	},

	/* input : Limit YCbCr FCC */
	/* output : Limit RGB 601 */
	{
		0x00020000, 0x00000000, 0x0002bccd, 0x00000000,
		0x00020000, 0xffff59e1, 0xfffe9ba9, 0x00000000,
		0x00020000, 0x00037b04, 0x00000000, 0x00000000,
		0xffffffc0, 0xfffffe00, 0xfffffe00, 0x00000000,
	},

	/* input : Limit YCbCr 240M */
	/* output : Limit RGB 601 */
	{
		0x00020000, 0x00000000, 0x000314e7, 0x00000000,
		0x00020000, 0xffff8e8f, 0xffff116b, 0x00000000,
		0x00020000, 0x0003920b, 0x00000000, 0x00000000,
		0xffffffc0, 0xfffffe00, 0xfffffe00, 0x00000000,
	},

	/* input : Limit YCbCr Ycgco */
	/* output : Limit RGB 601 */
	{
		0x00020000, 0xfffe0b6e, 0x0001f492, 0x00000000,
		0x00020000, 0x0001f492, 0x00000000, 0x00000000,
		0x00020000, 0xfffe0b6e, 0xfffe0b6e, 0x00000000,
		0xffffffc0, 0xfffffe00, 0xfffffe00, 0x00000000,
	},

	/* input : Limit YCbCr GBR */
	/* output : Limit RGB 601 */
	{
		0x00000000, 0x00000000, 0x00020000, 0x00000000,
		0x00020000, 0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x00020000, 0x00000000, 0x00000000,
		0xffffffc0, 0xffffffc0, 0xffffffc0, 0x00000000,
	}
};

u32 y2rf2l[7][16] = {

	/* input : Full YCbCr 601 */
	/* output : Limit RGB 601 */
	{
		0x0001b7b8, 0x00000000, 0x0002687c, 0x00000040,
		0x0001b7b8, 0xffff68ad, 0xfffec5fb, 0x00000040,
		0x0001b7b8, 0x00030b2e, 0x00000000, 0x00000040,
		0x00000000, 0xfffffe00, 0xfffffe00, 0x00000000,
	},

	/* input : Full YCbCr 709 */
	/* output : Limit RGB 601 */
	{
		0x0001b7b8, 0x00000000, 0x0002b478, 0x00000040,
		0x0001b7b8, 0xffffada1, 0xffff3228, 0x00000040,
		0x0001b7b8, 0x00032ff1, 0x00000000, 0x00000040,
		0x00000000, 0xfffffe00, 0xfffffe00, 0x00000000,
	},

	/* input : Full YCbCr 2020 */
	/* output : Limit RGB 601 */
	{
		0x0001b7b8, 0x00000000, 0x00028868, 0x00000040,
		0x0001b7b8, 0xffffb7a5, 0xffff04c4, 0x00000040,
		0x0001b7b8, 0x00033b49, 0x00000000, 0x00000040,
		0x00000000, 0xfffffe00, 0xfffffe00, 0x00000000,
	},

	/* input : Full YCbCr FCC */
	/* output : Limit RGB 601 */
	{
		0x0001b7b8, 0x00000000, 0x0002679b, 0x00000040,
		0x0001b7b8, 0xffff6e13, 0xfffec6fb, 0x00000040,
		0x0001b7b8, 0x00030eb3, 0x00000000, 0x00000040,
		0x00000000, 0xfffffe00, 0xfffffe00, 0x00000000,
	},

	/* input : Full YCbCr 240M */
	/* output : Limit RGB 601 */
	{
		0x0001b7b8, 0x00000000, 0x0002b4ff, 0x00000040,
		0x0001b7b8, 0xffff9c5a, 0xffff2e6c, 0x00000040,
		0x0001b7b8, 0x000322ed, 0x00000000, 0x00000040,
		0x00000000, 0xfffffe00, 0xfffffe00, 0x00000000,
	},

	/* input : Full YCbCr Ycgco */
	/* output : Limit RGB 601 */
	{
		0x0001b7b8, 0xfffe4848, 0x0001b7b8, 0x00000040,
		0x0001b7b8, 0x0001b7b8, 0x00000000, 0x00000040,
		0x0001b7b8, 0xfffe4848, 0xfffe4848, 0x00000040,
		0x00000000, 0xfffffe00, 0xfffffe00, 0x00000000,
	},

	/* input : Full YCbCr GBR */
	/* output : Limit RGB 601 */
	{
		0x00000000, 0x00000000, 0x0001b7b8, 0x00000040,
		0x0001b7b8, 0x00000000, 0x00000000, 0x00000040,
		0x00000000, 0x0001b7b8, 0x00000000, 0x00000040,
		0x00000000, 0x00000000, 0x00000000, 0x00000000,
	}
};

u32 y2rl2f[7][16] = {
	/* input : Limit YCbCr 601 */
	/* output : Full RGB 601 */
	{
		0x0002542a, 0x00000000, 0x0003312a, 0x00000000,
		0x0002542a, 0xffff376b, 0xfffe5fc3, 0x00000000,
		0x0002542a, 0x000408d3, 0x00000000, 0x00000000,
		0xffffffc0, 0xfffffe00, 0xfffffe00, 0x00000000,
	},

	/* input : Limit YCbCr 709 */
	/* output : Full RGB 601 */
	{
		0x0002542a, 0x00000000, 0x000395e2, 0x00000000,
		0x0002542a, 0xffff92d1, 0xfffeef27, 0x00000000,
		0x0002542a, 0x0004398d, 0x00000000, 0x00000000,
		0xffffffc0, 0xfffffe00, 0xfffffe00, 0x00000000,
	},

	/* input : Limit YCbCr 2020 */
	/* output : Full RGB 601 */
	{
		0x0002542a, 0x00000000, 0x00035b7b, 0x00000000,
		0x0002542a, 0xffffa017, 0xfffeb2fc, 0x00000000,
		0x0002542a, 0x00044896, 0x00000000, 0x00000000,
		0xffffffc0, 0xfffffe00, 0xfffffe00, 0x00000000,
	},

	/* input : Limit YCbCr FCC */
	/* output : Full RGB 601 */
	{
		0x0002542a, 0x00000000, 0x00033000, 0x00000000,
		0x0002542a, 0xffff3e92, 0xfffe6116, 0x00000000,
		0x0002542a, 0x00040d7c, 0x00000000, 0x00000000,
		0xffffffc0, 0xfffffe00, 0xfffffe00, 0x00000000,
	},

	/* input : Limit YCbCr 240M */
	/* output : Full RGB 601 */
	{
		0x0002542a, 0x00000000, 0x00039695, 0x00000000,
		0x0002542a, 0xffff7be9, 0xfffeea33, 0x00000000,
		0x0002542a, 0x0004284c, 0x00000000, 0x00000000,
		0xffffffc0, 0xfffffe00, 0xfffffe00, 0x00000000,
	},

	/* input : Limit YCbCr Ycgco */
	/* output : Full RGB 601 */
	{
		0x0002542a, 0xfffdb925, 0x000246db, 0x00000000,
		0x0002542a, 0x000246db, 0x00000000, 0x00000000,
		0x0002542a, 0xfffdb925, 0xfffdb925, 0x00000000,
		0xffffffc0, 0xfffffe00, 0xfffffe00, 0x00000000,
	},

	/* input : Limit YCbCr GBR */
	/* output : Full RGB 601 */
	{
		0x00000000, 0x00000000, 0x0002542a, 0x00000000,
		0x0002542a, 0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x0002542a, 0x00000000, 0x00000000,
		0xffffffc0, 0xffffffc0, 0xffffffc0, 0x00000000,
	}
};

u32 y2rf2f[7][16] = {
	/* input : Full YCbCr 601 */
	/* output : Full RGB 601 */
	{
		0x00020000, 0x00000000, 0x0002cdd3, 0x00000000,
		0x00020000, 0xffff4fcd, 0xfffe925d, 0x00000000,
		0x00020000, 0x00038b44, 0x00000000, 0x00000000,
		0x00000000, 0xfffffe00, 0xfffffe00, 0x00000000,
	},

	/* input : Full YCbCr 709 */
	/* output : Full RGB 601 */
	{
		0x00020000, 0x00000000, 0x0003264c, 0x00000000,
		0x00020000, 0xffffa017, 0xffff1052, 0x00000000,
		0x00020000, 0x0003b611, 0x00000000, 0x00000000,
		0x00000000, 0xfffffe00, 0xfffffe00, 0x00000000,
	},

	/* input : Full YCbCr 2020 */
	/* output : Full RGB 601 */
	{
		0x00020000, 0x00000000, 0x0002f2ff, 0x00000000,
		0x00020000, 0xffffabc0, 0xfffedb78, 0x00000000,
		0x00020000, 0x0003c347, 0x00000000, 0x00000000,
		0x00000000, 0xfffffe00, 0xfffffe00, 0x00000000,
	},

	/* input : Full YCbCr FCC */
	/* output : Full RGB 601 */
	{
		0x00020000, 0x00000000, 0x0002cccd, 0x00000000,
		0x00020000, 0xffff5616, 0xfffe9387, 0x00000000,
		0x00020000, 0x00038f5c, 0x00000000, 0x00000000,
		0x00000000, 0xfffffe00, 0xfffffe00, 0x00000000,
	},

	/* input : Full YCbCr 240M */
	/* output : Full RGB 601 */
	{
		0x00020000, 0x00000000, 0x000326e9, 0x00000000,
		0x00020000, 0xffff8bf8, 0xffff0bf8, 0x00000000,
		0x00020000, 0x0003a6e9, 0x00000000, 0x00000000,
		0x00000000, 0xfffffe00, 0xfffffe00, 0x00000000,
	},

	/* input : Full YCbCr Ycgco */
	/* output : Full RGB 601 */
	{
		0x00020000, 0xfffe0000, 0x00020000, 0x00000000,
		0x00020000, 0x00020000, 0x00000000, 0x00000000,
		0x00020000, 0xfffe0000, 0xfffe0000, 0x00000000,
		0x00000000, 0xfffffe00, 0xfffffe00, 0x00000000,
	},

	/* input : Full YCbCr GBR */
	/* output : Full RGB 601 */
	{
		0x00000000, 0x00000000, 0x00020000, 0x00000000,
		0x00020000, 0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x00020000, 0x00000000, 0x00000000,
		0x00000000, 0x00000000, 0x00000000, 0x00000000,
	}
};

static u32 get_cscmod_idx(enum de_color_space color_space)
{
	u32 idx;

	switch (color_space) {
	case DE_COLOR_SPACE_BT601:
	case DE_COLOR_SPACE_BT470BG:
		idx = 0; break;
	case DE_COLOR_SPACE_BT709:
		idx = 1; break;
	case DE_COLOR_SPACE_BT2020NC:
	case DE_COLOR_SPACE_BT2020C:
		idx = 2; break;
	case DE_COLOR_SPACE_FCC:
		idx = 3; break;
	case DE_COLOR_SPACE_SMPTE240M:
		idx = 4; break;
	case DE_COLOR_SPACE_YCGCO:
		idx = 5; break;
	case DE_COLOR_SPACE_GBR:
		idx = 6; break;
	default:
		idx = 0; break;
	}
	return idx;
}

s32 de_csc_coeff_calc(struct de_csc_info *in_info,
	struct de_csc_info *out_info, u32 **csc_coeff)
{
	u32 inidx; /* index for in_color_space */
	u32 outidx; /* index for out_color_space */

	inidx = get_cscmod_idx(in_info->color_space);
	outidx = get_cscmod_idx(out_info->color_space);

	if (in_info->px_fmt_space == DE_FORMAT_SPACE_RGB) {
		/* only support inrange is DE_COLOR_RANGE_0_255 */
		if (out_info->px_fmt_space == DE_FORMAT_SPACE_YUV) {
			if (out_info->color_range == DE_COLOR_RANGE_0_255)
				*csc_coeff = &r2y[7 + outidx][0];
			else /* outrange == DISP_COLOR_RANGE_16_235 */
				*csc_coeff = &r2y[outidx][0];
		} else {
			/*outfmt == DE_RGB */
			if (out_info->color_range == DE_COLOR_RANGE_0_255)
				*csc_coeff = &r2r[0][0];
			else /* outrange == DISP_COLOR_RANGE_16_235 */
				*csc_coeff = &r2r[1][0];
		}
	} else {
		/* infmt == DE_YUV */
		if (out_info->px_fmt_space == DE_FORMAT_SPACE_YUV) {
			if (in_info->color_range == DE_COLOR_RANGE_0_255) {
				if (out_info->color_range == DE_COLOR_RANGE_0_255)
					*csc_coeff = &y2yf2f[inidx * 3 + outidx][0];
				else /* outrange == DISP_COLOR_RANGE_16_235 */
					*csc_coeff = &y2yf2l[inidx * 3 + outidx][0];
			} else {
				/* inrange == DISP_COLOR_RANGE_16_235 */
				if (out_info->color_range == DE_COLOR_RANGE_0_255)
					*csc_coeff = &y2yl2f[inidx * 3 + outidx][0];
				else /* outrange == DISP_COLOR_RANGE_16_235 */
					*csc_coeff = &y2yl2l[inidx * 3 + outidx][0];
			}
		} else { /*outfmt == DE_RGB */
			if (in_info->color_range == DE_COLOR_RANGE_0_255) {
				if (out_info->color_range == DE_COLOR_RANGE_0_255)
					*csc_coeff = &y2rf2f[inidx][0];
				else
					*csc_coeff = &y2rf2l[inidx][0];
			} else {
				if (out_info->color_range == DE_COLOR_RANGE_0_255)
					*csc_coeff = &y2rl2f[inidx][0];
				else
					*csc_coeff = &y2rl2l[inidx][0];
			}
		}
	}

	return 0;
}
