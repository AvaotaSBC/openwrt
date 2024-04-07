/* SPDX-License-Identifier: GPL-2.0-or-later */
/* Copyright(c) 2020 - 2023 Allwinner Technology Co.,Ltd. All rights reserved. */
/*
 * A V4L2 driver for GC2365 Raw cameras.
 *
 * Copyright (c) 2018 by Allwinnertech Co., Ltd.  http://www.allwinnertech.com
 *
 * Authors:  Zheng ZeQun <zequnzheng@allwinnertech.com>
 *    Liang WeiJie <liangweijie@allwinnertech.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include "../../utility/vin_log.h"
#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/i2c.h>
#include <linux/delay.h>
#include <linux/videodev2.h>
#include <linux/clk.h>
#include <media/v4l2-device.h>
#include <media/v4l2-mediabus.h>
#include <linux/io.h>

#include "camera.h"
#include "sensor_helper.h"

MODULE_AUTHOR("lwj");
MODULE_DESCRIPTION("A low-level driver for GC0339 sensors");
MODULE_LICENSE("GPL");

#define MCLK              (24*1000*1000)
#define V4L2_IDENT_SENSOR 0xc8

/*
 * Our nominal (default) frame rate.
 */

#define SENSOR_FRAME_RATE 30

/*
 * The GC0310 i2c address
 */
#define I2C_ADDR 0x42

#define SENSOR_NAME "gc0339_mipi"

/*
 * The default register settings
 */

static struct regval_list sensor_default_regs[] = {
};


static struct regval_list sensor_480p30_regs[] = {

	{0xfc, 0x10},
	{0xfe, 0x00},
	{0xf6, 0x05},/* mode 24M pll_en */
	{0xf7, 0x03},
	{0xfc, 0x16},
	{0x01, 0x6a}, /* HB  0x6a=106 */
	{0x02, 0x1c}, /* VB  0x60=96 */
	{0x0f, 0x00}, /* [7:4]VB high; [3:0]HB high */
	{0x06, 0x00},
	{0x08, 0x06},
	{0x09, 0x01},/* window height high[0]   0x01e2= 482 */
	{0x0a, 0xe2},/* window height low [7:0] */
	{0x0b, 0x02},/* window width high[1:0] 027c= 636 */
	{0x0c, 0x7c},/* window width low [7:0] */
	 /* Direction Set Reg */
	{0x14, 0x10},/* Normal */
	 /* {0x14,0x01}, Mirror */
	 /* {0x14,0x32}, Updown */
	 /*  {0x14,0x23}, MirrorUpdown */
	{0x1a, 0x21},
	{0x1b, 0x08},
	{0x1c, 0x19},
	{0x1d, 0xea},

	{0xd0, 0x00},
	{0xd3, 0x50},

	{0xd2, 0x00},
	{0x20, 0x90},
	{0x2e, 0x30},
	{0x2b, 0x00},
	{0x2c, 0x03},

	{0xe8, 0x04},
	{0xe9, 0xbb},

	{0x30, 0xb7},
	{0x31, 0x7f},
	{0x32, 0x00},
	{0x39, 0x04},
	{0x3a, 0x20},
	{0x3b, 0x20},
	{0x3c, 0x04},
	{0x3d, 0x04},
	{0x3e, 0x04},
	{0x3f, 0x04},

	{0x40, 0x0f},
	{0x42, 0x20},
	{0x44, 0x02},
	{0x47, 0x10},
	{0x4a, 0x40},
	{0x4b, 0x40},
	{0x4c, 0x40},
	{0x50, 0x44},

	{0x61, 0x2b},
	{0x62, 0x20},
	{0x63, 0x03},
	{0x65, 0x10},
	{0x67, 0x10},
	{0x69, 0x03},
	{0x6a, 0x11},
	{0x6c, 0xaa},
	{0x6d, 0x00},
	{0x71, 0x05},
	{0x72, 0x01},
	{0x73, 0x01},
	{0x79, 0x01},
	{0x7a, 0x01},
	{0xfe, 0x50},
	{0x69, 0x03},
	{0x60, 0x90},

};

/*
 * Here we'll try to encapsulate the changes for just the output
 * video format.
 *
 */

static struct regval_list sensor_fmt_raw[] = {

};

/*
 * Code for dealing with controls.
 * fill with different sensor module
 * different sensor module has different settings here
 * if not support the follow function ,retrun -EINVAL
 */

static int sensor_g_exp(struct v4l2_subdev *sd, __s32 *value)
{
	struct sensor_info *info = to_state(sd);
	*value = info->exp;
	sensor_dbg("sensor_get_exposure = %d\n", info->exp);
	return 0;
}

static int sensor_s_exp(struct v4l2_subdev *sd, unsigned int exp_val)
{
	unsigned char explow, exphigh;
	unsigned int all_exp;
	struct sensor_info *info = to_state(sd);

	all_exp = exp_val >> 4;
	if (all_exp > 0x0fff)
		all_exp = 0x0fff;
	/* if (all_exp > 2100) */
	/*	all_exp = 2100; */

	sensor_write(sd, 0xfe, 0x00);

	exphigh  = (unsigned char) ((all_exp >> 8) & 0x0f);
	explow  = (unsigned char) (all_exp & 0xff);

	sensor_write(sd, 0x03, exphigh);
	sensor_write(sd, 0x04, explow);

	sensor_dbg("sensor_set_exp exp_val = %d, Done!\n", exp_val);
	sensor_dbg("sensor_set_exp all_exp = %d, Done!\n", all_exp);

	info->exp = exp_val;
	return 0;
}

static int sensor_g_gain(struct v4l2_subdev *sd, __s32 *value)
{
	struct sensor_info *info = to_state(sd);
	*value = info->gain;
	sensor_dbg("sensor_get_gain = %d\n", info->gain);
	return 0;
}

static int sensor_s_gain(struct v4l2_subdev *sd, unsigned int gain_val)
{
	struct sensor_info *info = to_state(sd);
	unsigned int All_gain, Auto_pregain,
				Global_gain = 0x40, Channel_gain = 0x80;


	All_gain = gain_val*4;

	if (All_gain < 256) {
		Auto_pregain = All_gain;
	} else if (All_gain < 2048) {
		Auto_pregain = 255;
		if (All_gain < 512)
			Global_gain = 64 + 64/256*(All_gain-256);
		else if (All_gain < 1024)
			Global_gain = 128 + 64/512*(All_gain-512);
		else
			Global_gain = 192 + 64/1024*(All_gain-1024);

	} else if (All_gain < 4096) {
		Auto_pregain = 0xff;
		Global_gain = 0xff;
		Channel_gain = All_gain >> 4;
	} else {
		All_gain = 4096;
		Auto_pregain = 255;
		Global_gain = 255;
		Channel_gain = 255;
	}

	sensor_write(sd, 0x51, Auto_pregain);
	sensor_write(sd, 0x50, Global_gain);
	sensor_write(sd, 0x53, Channel_gain);
	sensor_write(sd, 0x54, Channel_gain);
	sensor_write(sd, 0x55, Channel_gain);
	sensor_write(sd, 0x56, Channel_gain);


	sensor_dbg("sensor_set_gain: gain_val = %d, Done!\n", gain_val);
	sensor_dbg("sensor_set_gain: All_gain = %d, Done!\n", All_gain);
	sensor_dbg("sensor_set_gain: Auto_pregain = %d, Done!\n", Auto_pregain);
	sensor_dbg("sensor_set_gain: Global_gain  = %d, Done!\n", Global_gain);
	sensor_dbg("sensor_set_gain: Channel_gain = %d, Done!\n", Channel_gain);

	info->gain = gain_val;

	return 0;
}
static int sensor_s_exp_gain(struct v4l2_subdev *sd,
				struct sensor_exp_gain *exp_gain)
{
	int exp_val, gain_val;
	struct sensor_info *info = to_state(sd);

	exp_val = exp_gain->exp_val;
	gain_val = exp_gain->gain_val;

	if (gain_val < 16)
		gain_val = 16;
	if (gain_val > 1024)
		gain_val = 1024;


	if (exp_val > 0xffff)
		exp_val = 0xffff;

	sensor_s_exp(sd, exp_val);
	sensor_s_gain(sd, gain_val);

	sensor_dbg("sensor_set_gain exp = %d, gain = %d Done!\n", exp_val, gain_val);

	info->exp = exp_val;
	info->gain = gain_val;
	return 0;
}

static int sensor_s_sw_stby(struct v4l2_subdev *sd, int on_off)
{
	int ret = 0;
	return ret;
}

/*
 * Stuff that knows about the sensor.
 */
static int sensor_power(struct v4l2_subdev *sd, int on)
{
	int ret;

	switch (on) {
	case STBY_ON:
		sensor_print("STBY_ON!\n");
		cci_lock(sd);
		ret = sensor_s_sw_stby(sd, STBY_ON);
		if (ret < 0)
			sensor_err("soft stby falied!\n");
		usleep_range(1000, 1200);
		cci_unlock(sd);
		break;
	case STBY_OFF:
		sensor_print("STBY_OFF!\n");
		cci_lock(sd);
		usleep_range(1000, 1200);
		ret = sensor_s_sw_stby(sd, STBY_OFF);
		if (ret < 0)
			sensor_err("soft stby falied!\n");
		cci_unlock(sd);
		break;
	case PWR_ON:
		sensor_print("PWR_ON!\n");
		cci_lock(sd);
		vin_gpio_set_status(sd, PWDN, 1);
		vin_gpio_set_status(sd, RESET, 1);
		usleep_range(100, 120);
		vin_gpio_write(sd, PWDN, CSI_GPIO_LOW);
		vin_gpio_write(sd, RESET, CSI_GPIO_LOW);
		vin_gpio_write(sd, POWER_EN, CSI_GPIO_HIGH);
		vin_set_pmu_channel(sd, IOVDD, ON);
		usleep_range(100, 120);
		vin_set_pmu_channel(sd, DVDD, ON);
		usleep_range(100, 120);
		vin_set_pmu_channel(sd, AVDD, ON);
		usleep_range(200, 220);
		vin_set_mclk_freq(sd, MCLK);
		vin_set_mclk(sd, ON);
		/* usleep_range(100, 120); */
		/* vin_gpio_write(sd, PWDN, CSI_GPIO_HIGH);
		usleep_range(100, 120);
		vin_gpio_write(sd, RESET, CSI_GPIO_HIGH);
		usleep_range(300, 310);
		cci_unlock(sd);
		break;
	case PWR_OFF:
		sensor_print("PWR_OFF!\n");
		cci_lock(sd);
		usleep_range(100, 120);
		vin_gpio_write(sd, PWDN, CSI_GPIO_HIGH);
		vin_gpio_write(sd, RESET, CSI_GPIO_LOW);
		usleep_range(100, 120);
		vin_set_mclk(sd, OFF);
		vin_gpio_write(sd, POWER_EN, CSI_GPIO_LOW);
		vin_set_pmu_channel(sd, AVDD, OFF);
		vin_set_pmu_channel(sd, DVDD, OFF);
		vin_set_pmu_channel(sd, IOVDD, OFF);
		usleep_range(100, 120);
		vin_gpio_write(sd, PWDN, CSI_GPIO_LOW);
		usleep_range(100, 120);
		vin_gpio_set_status(sd, RESET, 0);
		vin_gpio_set_status(sd, PWDN, 0);
		cci_unlock(sd);
		break;
	default:
		return -EINVAL;
	}

	return 0;
}

static int sensor_reset(struct v4l2_subdev *sd, u32 val)
{
	switch (val) {
	case 0:
		vin_gpio_write(sd, RESET, CSI_GPIO_HIGH);
		usleep_range(100, 120);
		break;
	case 1:
		vin_gpio_write(sd, RESET, CSI_GPIO_LOW);
		usleep_range(100, 120);
		break;
	default:
		return -EINVAL;
	}
	return 0;
}

static int sensor_detect(struct v4l2_subdev *sd)
{
	sensor_print("hc gc0339_mipi sensor_detect\n");
	unsigned int SENSOR_ID = 0;
	data_type val;
	int cnt = 0;

	sensor_write(sd, 0xfc, 0x10);
	sensor_write(sd, 0xfc, 0x10);
	sensor_read(sd, 0x00, &val);
	SENSOR_ID |= (val);
	sensor_print("hc V4L2_IDENT_SENSOR = 0x%x\n", SENSOR_ID);

	while ((SENSOR_ID != V4L2_IDENT_SENSOR) && (cnt < 5)) {
		sensor_read(sd, 0x00, &val);
		SENSOR_ID |= (val);
		sensor_print("retry = %d, V4L2_IDENT_SENSOR = %x\n", cnt, SENSOR_ID);
		cnt++;
	}

	if (SENSOR_ID != V4L2_IDENT_SENSOR)
		return -ENODEV;
	sensor_print("detect succee \n");

	return 0;
}

static int sensor_init(struct v4l2_subdev *sd, u32 val)
{
	int ret;
	struct sensor_info *info = to_state(sd);

	sensor_print("sensor_init\n");

	/* Make sure it is a target sensor */
	ret = sensor_detect(sd);
	sensor_print("sensor_init ret=%d\n", ret);
	if (ret) {
		sensor_err("chip found is not an target chip.\n");
		return ret;
	}

	info->focus_status = 0;
	info->low_speed = 0;
	info->width = 640;
	info->height = 480;
	info->hflip = 0;
	info->vflip = 0;
	info->gain = 0;

	info->tpf.numerator = 1;
	info->tpf.denominator = 30; /* 30fps */

	return 0;
}

static long sensor_ioctl(struct v4l2_subdev *sd, unsigned int cmd, void *arg)
{
	int ret = 0;
	struct sensor_info *info = to_state(sd);

	switch (cmd) {
	case GET_CURRENT_WIN_CFG:
		if (info->current_wins != NULL) {
			memcpy(arg, info->current_wins,
				sizeof(*info->current_wins));
			ret = 0;
		} else {
			sensor_err("empty wins!\n");
			ret = -1;
		}
		break;
	case SET_FPS:
		ret = 0;
		break;
	case VIDIOC_VIN_SENSOR_EXP_GAIN:
		ret = sensor_s_exp_gain(sd, (struct sensor_exp_gain *)arg);
		break;
	case VIDIOC_VIN_SENSOR_CFG_REQ:
		sensor_cfg_req(sd, (struct sensor_config *)arg);
		break;
	default:
		return -EINVAL;
	}
	return ret;
}

/*
 * Store information about the video data format.
 */
static struct sensor_format_struct sensor_formats[] = {
	{
		.desc = "Raw RGB Bayer",
		.mbus_code = MEDIA_BUS_FMT_SRGGB10_1X10,
		.regs = sensor_fmt_raw,
		.regs_size = ARRAY_SIZE(sensor_fmt_raw),
		.bpp = 1
	},
};
#define N_FMTS ARRAY_SIZE(sensor_formats)

/*
 * Then there is the issue of window sizes.  Try to capture the info here.
 */

static struct sensor_win_size sensor_win_sizes[] = {
	{
	.width      = 640,
	.height     = 480,
	.hoffset    = 0,
	.voffset    = 0,
	.hts        = 742,
	.vts        = 510,
	.pclk       = 12*1000*1000,
	.mipi_bps   = 672*1000*1000,
	.fps_fixed  = 30,
	.bin_factor = 1,
	.intg_min   = 1<<4,
	.intg_max   = 510<<4,
	.gain_min   = 1<<4,  /* 16<<4, */
	.gain_max   = 64<<4, /* 64<<4, */
	.regs       = sensor_480p30_regs,
	.regs_size  = ARRAY_SIZE(sensor_480p30_regs),
	.set_size   = NULL,
	},
};

#define N_WIN_SIZES (ARRAY_SIZE(sensor_win_sizes))

static int sensor_g_mbus_config(struct v4l2_subdev *sd,
				struct v4l2_mbus_config *cfg)
{
	cfg->type = V4L2_MBUS_CSI2_DPHY;
	cfg->flags = 0 | V4L2_MBUS_CSI2_1_LANE | V4L2_MBUS_CSI2_CHANNEL_0;

	return 0;
}

static int sensor_g_ctrl(struct v4l2_ctrl *ctrl)
{
	struct sensor_info *info =
			container_of(ctrl->handler,
			struct sensor_info, handler);
	struct v4l2_subdev *sd = &info->sd;

	switch (ctrl->id) {
	case V4L2_CID_GAIN:
		return sensor_g_gain(sd, &ctrl->val);
	case V4L2_CID_EXPOSURE:
		return sensor_g_exp(sd, &ctrl->val);
	}
	return -EINVAL;
}

static int sensor_s_ctrl(struct v4l2_ctrl *ctrl)
{
	struct sensor_info *info =
			container_of(ctrl->handler,
			struct sensor_info, handler);
	struct v4l2_subdev *sd = &info->sd;

	switch (ctrl->id) {
	case V4L2_CID_GAIN:
		return sensor_s_gain(sd, ctrl->val);
	case V4L2_CID_EXPOSURE:
		return sensor_s_exp(sd, ctrl->val);
	}
	return -EINVAL;
}

static int sensor_reg_init(struct sensor_info *info)
{
	int ret;
	struct v4l2_subdev *sd = &info->sd;
	struct sensor_format_struct *sensor_fmt = info->fmt;
	struct sensor_win_size *wsize = info->current_wins;
	struct sensor_exp_gain exp_gain;

	ret = sensor_write_array(sd, sensor_default_regs,
				ARRAY_SIZE(sensor_default_regs));
	if (ret < 0) {
		sensor_err("write sensor_default_regs error\n");
		return ret;
	}

	sensor_print("sensor_reg_init\n");

	sensor_write_array(sd, sensor_fmt->regs, sensor_fmt->regs_size);

	if (wsize->regs)
		sensor_write_array(sd, wsize->regs, wsize->regs_size);

	if (wsize->set_size)
		wsize->set_size(sd);

	info->width = wsize->width;
	info->height = wsize->height;

	exp_gain.exp_val = 8000;
	exp_gain.gain_val = 1024;
	sensor_s_exp_gain(sd, &exp_gain);

	sensor_print("s_fmt set width = %d, height = %d\n", wsize->width,
				wsize->height);

	return 0;
}

static int sensor_s_stream(struct v4l2_subdev *sd, int enable)
{
	struct sensor_info *info = to_state(sd);

	sensor_print("%s on = %d, %d*%d fps: %d code: %x\n", __func__, enable,
			info->current_wins->width, info->current_wins->height,
			info->current_wins->fps_fixed, info->fmt->mbus_code);

	if (!enable)
		return 0;

	return sensor_reg_init(info);
}

/* ----------------------------------------------------------------------- */

static const struct v4l2_ctrl_ops sensor_ctrl_ops = {
	.g_volatile_ctrl = sensor_g_ctrl,
	.s_ctrl = sensor_s_ctrl,
};

static const struct v4l2_subdev_core_ops sensor_core_ops = {
	.reset = sensor_reset,
	.init = sensor_init,
	.s_power = sensor_power,
	.ioctl = sensor_ioctl,
#if IS_ENABLED(CONFIG_COMPAT)
	.compat_ioctl32 = sensor_compat_ioctl32,
#endif
};

static const struct v4l2_subdev_video_ops sensor_video_ops = {
	.s_stream = sensor_s_stream,
	.g_mbus_config = sensor_g_mbus_config,
};

static const struct v4l2_subdev_pad_ops sensor_pad_ops = {
	.enum_mbus_code = sensor_enum_mbus_code,
	.enum_frame_size = sensor_enum_frame_size,
	.enum_frame_interval = sensor_enum_frame_interval,
	.get_fmt = sensor_get_fmt,
	.set_fmt = sensor_set_fmt,
};

static const struct v4l2_subdev_ops sensor_ops = {
	.core = &sensor_core_ops,
	.video = &sensor_video_ops,
	.pad = &sensor_pad_ops,
};

/* ----------------------------------------------------------------------- */
static struct cci_driver cci_drv = {
		.name = SENSOR_NAME,
		.addr_width = CCI_BITS_8,
		.data_width = CCI_BITS_8,
};

static int sensor_init_controls(struct v4l2_subdev *sd,
					const struct v4l2_ctrl_ops *ops)
{
	struct sensor_info *info = to_state(sd);
	struct v4l2_ctrl_handler *handler = &info->handler;
	struct v4l2_ctrl *ctrl;
	int ret = 0;

	v4l2_ctrl_handler_init(handler, 2);

	v4l2_ctrl_new_std(handler, ops, V4L2_CID_GAIN, 1 * 1600,
				256 * 1600, 1, 1 * 1600);
	ctrl = v4l2_ctrl_new_std(handler, ops, V4L2_CID_EXPOSURE, 0,
				65536 * 16, 1, 0);
	if (ctrl != NULL)
		ctrl->flags |= V4L2_CTRL_FLAG_VOLATILE;

	if (handler->error) {
		ret = handler->error;
		v4l2_ctrl_handler_free(handler);
	}

	sd->ctrl_handler = handler;

	return ret;
}

static int sensor_probe(struct i2c_client *client,
			const struct i2c_device_id *id)
{
	struct v4l2_subdev *sd;
	struct sensor_info *info;

	info = kzalloc(sizeof(*info), GFP_KERNEL);
	if (info == NULL)
		return -ENOMEM;
	sd = &info->sd;

	cci_dev_probe_helper(sd, client, &sensor_ops, &cci_drv);
	sensor_init_controls(sd, &sensor_ctrl_ops);

	mutex_init(&info->lock);

	info->fmt = &sensor_formats[0];
	info->fmt_pt = &sensor_formats[0];
	info->win_pt = &sensor_win_sizes[0];
	info->fmt_num = N_FMTS;
	info->win_size_num = N_WIN_SIZES;
	info->sensor_field = V4L2_FIELD_NONE;
	info->stream_seq = MIPI_BEFORE_SENSOR;
	info->af_first_flag = 1;
	info->exp = 0;
	info->gain = 0;

	return 0;
}

static int sensor_remove(struct i2c_client *client)
{
	struct v4l2_subdev *sd;

	sd = cci_dev_remove_helper(client, &cci_drv);

	kfree(to_state(sd));
	return 0;
}

static const struct i2c_device_id sensor_id[] = {
	{SENSOR_NAME, 0},
	{}
};

MODULE_DEVICE_TABLE(i2c, sensor_id);

static struct i2c_driver sensor_driver = {
		.driver = {
				.owner = THIS_MODULE,
				.name = SENSOR_NAME,
				},
		.probe = sensor_probe,
		.remove = sensor_remove,
		.id_table = sensor_id,
};
static __init int init_sensor(void)
{
	int ret = 0;

	ret = cci_dev_init_helper(&sensor_driver);

	return ret;
}

static __exit void exit_sensor(void)
{
	cci_dev_exit_helper(&sensor_driver);
}

VIN_INIT_DRIVERS(init_sensor);
module_exit(exit_sensor);
