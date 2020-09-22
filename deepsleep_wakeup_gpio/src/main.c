/*
 * Copyright (c) 2019 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <zephyr.h>
#include <device.h>
#include <init.h>
#include <power/power.h>
#include <hal/nrf_gpio.h>


#define BUSY_WAIT_S 2U
#define SLEEP_S 2U


#define CONSOLE_LABEL DT_LABEL(DT_CHOSEN(zephyr_console))
#define SHELL_UART DT_LABEL(DT_CHOSEN(zephyr_shell_uart))
#define MCUMGR DT_LABEL(DT_CHOSEN(zephyr_bt_mon_uart))
#define BT_MON_UART DT_LABEL(DT_CHOSEN(zephyr_bt_mon_uart))
#define BT_C2H_UART DT_LABEL(DT_CHOSEN(zephyr_bt_c2h_uart))
#define SRAM DT_LABEL(DT_CHOSEN(zephyr_sram))
#define FLASH DT_LABEL(DT_CHOSEN(zephyr_flash))
#define CODEPART DT_LABEL(DT_CHOSEN(zephyr_code_partition))

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

	struct device *shell_uart = device_get_binding(SHELL_UART);
	struct device *mcumgr = device_get_binding(MCUMGR);
	struct device *bt_mon_uart = device_get_binding(BT_MON_UART);
	struct device *bt_c2h_uart = device_get_binding(BT_C2H_UART);
	//struct device *sram = device_get_binding(SRAM);
	struct device *flash = device_get_binding(BT_C2H_UART);
	int rc;

	rc = device_set_power_state(shell_uart, DEVICE_PM_OFF_STATE, NULL, NULL);
	printk("\n --> Shell UART OFF");

	rc = device_set_power_state(mcumgr, DEVICE_PM_OFF_STATE, NULL, NULL);
	printk("\n --> MCU MANAGER OFF");

	rc = device_set_power_state(bt_mon_uart, DEVICE_PM_OFF_STATE, NULL, NULL);
	printk("\n --> BT MON UART OFF");

	rc = device_set_power_state(bt_c2h_uart, DEVICE_PM_OFF_STATE, NULL, NULL);
	printk("\n --> BT C2H UART OFF");

	//rc = device_set_power_state(sram, DEVICE_PM_OFF_STATE, NULL, NULL);
	//printk("\nzephyr --> SRAM OFF");

	rc = device_set_power_state(flash, DEVICE_PM_OFF_STATE, NULL, NULL);
	printk("\n --> FLASH OFF");

	sys_pm_ctrl_disable_state(SYS_POWER_STATE_DEEP_SLEEP_1);
	return 0;
}

SYS_INIT(disable_ds_1, PRE_KERNEL_2, 0);

void main(void)
{
	int rc;
	struct device *cons = device_get_binding(CONSOLE_LABEL);


	/* Configure to generate PORT event (wakeup) on button 1 press. */
	nrf_gpio_cfg_input(DT_GPIO_PIN(DT_NODELABEL(button0), gpios),
			   NRF_GPIO_PIN_PULLUP);
	nrf_gpio_cfg_sense_set(DT_GPIO_PIN(DT_NODELABEL(button0), gpios),
			       NRF_GPIO_PIN_SENSE_LOW);

	printk("\n Entering SYS_POWER_STATE_DEEP_SLEEP_1 mode");
	printk("\n press BUTTON1 to restart\n");
	k_sleep(K_MSEC(1));
	rc = sys_set_power_state(SYS_POWER_STATE_DEEP_SLEEP_1, NULL, NULL);
	rc = device_set_power_state(cons, DEVICE_PM_ACTIVE_STATE, NULL, NULL);

	printk("\n Resuming system");
	while (true) {
		/* spin to avoid fall-off behavior */
	}
}
