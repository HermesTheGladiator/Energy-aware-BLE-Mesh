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

//Extracting label from devicetree
// DST given in zephyrproject/zephyr/build/zephyr/zephyr.dst
#define led0 DT_NODELABEL(led0)
#define led1 DT_NODELABEL(led1)
#define led2 DT_NODELABEL(led2)
#define led3 DT_NODELABEL(led3)

#define BUTTONS DT_NODELABEL(buttons)
#define button0 DT_NODELABEL(button0)
#define button1 DT_NODELABEL(button1)
#define button2 DT_NODELABEL(button2)
#define button3 DT_NODELABEL(button3)

#define pwm0 DT_NODELABEL(pwm0)

#define CONSOLE_LABEL DT_LABEL(DT_CHOSEN(zephyr_console))
#define SHELL_UART DT_LABEL(DT_CHOSEN(zephyr_shell_uart))
#define MCUMGR DT_LABEL(DT_CHOSEN(zephyr_bt_mon_uart))
#define BT_MON_UART DT_LABEL(DT_CHOSEN(zephyr_bt_mon_uart))
#define BT_C2H_UART DT_LABEL(DT_CHOSEN(zephyr_bt_c2h_uart))
#define SRAM DT_LABEL(DT_CHOSEN(zephyr_sram))
#define FLASH DT_LABEL(DT_CHOSEN(zephyr_flash))
#define CODEPART DT_LABEL(DT_CHOSEN(zephyr_code_partition))


#define DELAY 10U

/* 
Disabling unrequired components after PRE_KERNEL_1
execution, that is in PRE_KERNEL_2
 */
static int disable_comps(struct device *dev)
{
	/*
	struct device *pwm = device_get_binding(DT_LABEL(pwm0));
	struct device *led0 = device_get_binding(DT_LABEL(led0));
	struct device *led1= device_get_binding(DT_LABEL(led1));
	struct device *led2 = device_get_binding(DT_LABEL(led2));
	struct device *led3 = device_get_binding(DT_LABEL(led3));

	struct device *button0 = device_get_binding(DT_LABEL(button0));
	struct device *button1 = device_get_binding(DT_LABEL(button1));
	struct device *button2 = device_get_binding(DT_LABEL(button2));
	struct device *button3 = device_get_binding(DT_LABEL(button3));
	struct device *button4= device_get_binding(DT_LABEL(button4));
	
	rc = device_set_power_state(led0, DEVICE_PM_ACTIVE_STATE, NULL, NULL);
	printk("disabling zephyre_dts --> led0");
	rc = device_set_power_state(led1, DEVICE_PM_ACTIVE_STATE, NULL, NULL);
	printk("disabling zephyre_dts --> led1");
	rc = device_set_power_state(led2, DEVICE_PM_ACTIVE_STATE, NULL, NULL);
	printk("disabling zephyre_dts --> led2");
	rc = device_set_power_state(led3, DEVICE_PM_ACTIVE_STATE, NULL, NULL);
	printk("disabling zephyre_dts --> led3");

	struct device *cons = device_get_binding(CONSOLE_LABEL);
	int rc;
	rc = device_set_power_state(cons, DEVICE_PM_OFF_STATE, NULL, NULL);
	printk("zephyr--> zephyr_console OFF");
	*/
	struct device *shell_uart = device_get_binding(SHELL_UART);
	struct device *mcumgr = device_get_binding(MCUMGR);
	struct device *bt_mon_uart = device_get_binding(BT_MON_UART);
	struct device *bt_c2h_uart = device_get_binding(BT_C2H_UART);
	//struct device *sram = device_get_binding(SRAM);
	struct device *flash = device_get_binding(BT_C2H_UART);
	int rc;

	rc = device_set_power_state(shell_uart, DEVICE_PM_OFF_STATE, NULL, NULL);
	printk("\nzephyr --> Shell UART OFF");

	rc = device_set_power_state(mcumgr, DEVICE_PM_OFF_STATE, NULL, NULL);
	printk("\nzephyr --> MCU MANAGER OFF");

	rc = device_set_power_state(bt_mon_uart, DEVICE_PM_OFF_STATE, NULL, NULL);
	printk("\nzephyr --> BT MON UART OFF");

	rc = device_set_power_state(bt_c2h_uart, DEVICE_PM_OFF_STATE, NULL, NULL);
	printk("\nzephyr --> BT C2H UART OFF");

	//rc = device_set_power_state(sram, DEVICE_PM_OFF_STATE, NULL, NULL);
	//printk("\nzephyr --> SRAM OFF");

	rc = device_set_power_state(flash, DEVICE_PM_OFF_STATE, NULL, NULL);
	printk("\nzephyr --> FLASH OFF");

	
	

	return 0;
}

SYS_INIT(disable_comps, PRE_KERNEL_2, 0);



void main(void)
{
	while(true){

		
		printk("### After Board Init --> Suspending for %d secs",DELAY);
		k_sleep(K_SECONDS(DELAY));

		struct device *cons = device_get_binding(CONSOLE_LABEL);


		// Profiling power consumption for ACTIVE STATE (Constant Latency mode)
		// device_state defined in enum power_states --> SYS_POWER_STATE_ACTIVE
		printk("\n <-- Forcing SYS_POWER_STATE_ACTIVE state --->");
		sys_set_power_state(SYS_POWER_STATE_ACTIVE);
		printk("\n SYS_POWER_STATE_ACTIVE --> Suspending for %d secs",DELAY);
		k_sleep(K_SECONDS(DELAY));

		// Profiling power consumption for Deep Sleep state 
		printk("\n <-- Forcing SYS_POWER_STATE_SLEEP_1 state --->");
		sys_set_power_state(SYS_POWER_STATE_DEEP_SLEEP_1);
		printk("\n SYS_POWER_STATE_SLEEP_1 --> Suspending for %d secs",DELAY);
		k_sleep(K_SECONDS(DELAY));

		/*
		// Profiling power consumption for Deep Sleep state 
		printk("\n <-- Forcing SYS_POWER_STATE_DEEP_SLEEP_1 state --->");
		sys_set_power_state(SYS_POWER_STATE_DEEP_SLEEP_1);
		printk("\n SYS_POWER_STATE_SLEEP_1 --> Suspending for %d secs",DELAY);
		k_sleep(K_SECONDS(DELAY));
		/*

		/*
		// Profiling power consumption for Low Power state 
		printk("\n <-- Forcing DEVICE_PM_LOW_POWER_STATE state --->");
		device_set_power_state(cons, DEVICE_PM_LOW_POWER_STATE, NULL, NULL);
		printk("\n %s --> Suspending for %d secs",CONSOLE_LABEL,DELAY);
		k_sleep(K_SECONDS(DELAY));
		*/

		// Profiling power consumption for POWER OFF STATE 
		printk("\n <-- Forcing DEVICE_PM_OFF_STATE state --->");
		device_set_power_state(cons, DEVICE_PM_OFF_STATE, NULL, NULL);
		printk("\n %s --> Suspending for %d secs",CONSOLE_LABEL,DELAY);
		k_sleep(K_SECONDS(DELAY));


		//restoring active state
		printk("\n <-- Forcing DEVICE_PM_ACTIVE_STATE state --->\n");
		sys_set_power_state(SYS_POWER_STATE_ACTIVE);
		k_sleep(K_SECONDS(DELAY));


	}
}