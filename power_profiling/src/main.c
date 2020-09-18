/*
 * Copyright (c) 2019 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <zephyr.h>
#include <device.h>
#include <kernel.h>
#include <init.h>
#include <power/power.h>
#include <hal/nrf_gpio.h>

//#include "board.h"

#define DELAY 10U

/* Prevent deep sleep (system off) from being entered on long timeouts
 * or `K_FOREVER` due to the default residency policy.
 *
 * This has to be done before anything tries to sleep, which means
 * before the threading system starts up between PRE_KERNEL_2 and
 * POST_KERNEL.  Do it at the start of PRE_KERNEL_2.
 */
static int disable_ds_1(struct device *dev)
{
	ARG_UNUSED(dev);

	sys_pm_ctrl_disable_state(SYS_POWER_STATE_DEEP_SLEEP_1);
	return 0;
}

SYS_INIT(disable_ds_1, PRE_KERNEL_2, 0);



void main(void)
{
	//board_init(); //can be only used in SDK
	printk("After Board Init--> Suspending for %d secs\n",DELAY);
	k_sleep(K_SECONDS(DELAY));
	printk("Wakeup\n");

}