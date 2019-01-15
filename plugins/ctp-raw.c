/*
 * tslib driver for ctp touchscreens
 * Copyright (C) 2009 Michael Olbrich, Pengutronix e.K.
 * some inspiration from the old xf86-input-ctp xorg input driver.
 *
 * This file is placed under the LGPL.  Please see the file COPYING for more
 * details.
 *
 * SPDX-License-Identifier: LGPL-2.1
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <inttypes.h>
#include <sys/ioctl.h>

#include "config.h"
#include "tslib-private.h"

#define CONTROL_RELEASED 0xEB90EB90

struct tslib_ctp {
	struct tslib_module_info module;
    struct ts_sample sample;
};

static inline int _is_control_pressed(int control_val) {
	return control_val >= 0;
}
static inline int _is_control_released(int control_val) {
	return (uint32_t)control_val == CONTROL_RELEASED;
}
static inline void _get_control_point(int control_val, int* x, int* y) {
	*x = ((uint32_t)control_val) & 0xffff;
	*y = ((uint32_t)control_val) >> 16;
}

static int ctp_init_device(struct tsdev *dev)
{
	int fd = dev->fd;
	int max_counter = 10000;
	int control_val;

	if (-1 == fd) {
		return -EINVAL;
	}

	// clear all buffer from ioctl
	while (max_counter--) {
		control_val = ioctl(fd, 9, 0);
		if (!_is_control_pressed(control_val)
		    && !_is_control_released(control_val)) {
			break;
		}
	}

	return fd;
}

static int ctp_read(struct tslib_module_info *inf, struct ts_sample *samp,
					int nr)
{
	struct tslib_ctp *ctp = (struct tslib_ctp *)inf;
	struct tsdev *ts = inf->dev;
	struct ts_sample *current = &ctp->sample;
	int control_val;
	int result = 0;

	while (result < nr) {
		control_val = ioctl(ts->fd, 9, 0);

		if (_is_control_released(control_val)) {
			/* release. No coords follow. Use old values */
            current->pressure = 0;
            gettimeofday(&current->tv, NULL);
		} else if (_is_control_pressed(control_val)){
			/* read coords */
			_get_control_point(control_val, &current->x, &current->y);
            current->pressure = 1;
            gettimeofday(&current->tv, NULL);
		} else {
		    /* invalid */
		    break;
		}
        memcpy(samp, &ctp->sample, sizeof(struct ts_sample));

#ifdef DEBUG
		fprintf(stderr,
			"RAW---------------------------> %d %d %d\n",
			samp->x, samp->y, samp->pressure);
#endif /*DEBUG*/
		++samp;
		++result;
	}
	return result;
}

static const struct tslib_ops ctp_ops = {
		.read	= ctp_read,
};

TSAPI struct tslib_module_info *ctp_mod_init(struct tsdev *dev,
											 __attribute__ ((unused)) const char *params)
{
	struct tslib_ctp *m;

	if (ctp_init_device(dev) <= 0)
		return NULL;

	m = malloc(sizeof(struct tslib_ctp));
	if (m == NULL)
		return NULL;

	m->module.ops = &ctp_ops;
	return (struct tslib_module_info *)m;
}

#ifndef TSLIB_STATIC_CTP_MODULE
TSLIB_MODULE_INIT(ctp_mod_init);
#endif
