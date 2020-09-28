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
//#include <hal/nrf_gpio.h>
#include <drivers/gpio.h>
#include <sys/printk.h>
#include <inttypes.h>
#include <sys/util.h>

#define FLAGS_OR_ZERO(node)						\
	COND_CODE_1(DT_PHA_HAS_CELL(node, gpios, flags),		\
		    (DT_GPIO_FLAGS(node, gpios)),			\
		    (0))
	
static struct gpio_callback button_cb_data;


#define SLEEP_S 2U


#define CONSOLE_LABEL DT_LABEL(DT_CHOSEN(zephyr_console))
#define SHELL_UART DT_LABEL(DT_CHOSEN(zephyr_shell_uart))
#define MCUMGR DT_LABEL(DT_CHOSEN(zephyr_bt_mon_uart))
#define BT_MON_UART DT_LABEL(DT_CHOSEN(zephyr_bt_mon_uart))
#define BT_C2H_UART DT_LABEL(DT_CHOSEN(zephyr_bt_c2h_uart))
#define SRAM DT_LABEL(DT_CHOSEN(zephyr_sram))
#define FLASH DT_LABEL(DT_CHOSEN(zephyr_flash))
#define CODEPART DT_LABEL(DT_CHOSEN(zephyr_code_partition))

#define SW0_NODE	DT_ALIAS(sw0)

#if DT_NODE_HAS_STATUS(SW0_NODE, okay)
#define SW0_GPIO_LABEL	DT_GPIO_LABEL(SW0_NODE, gpios)
#define SW0_GPIO_PIN	DT_GPIO_PIN(SW0_NODE, gpios)
#define SW0_GPIO_FLAGS	(GPIO_INPUT | FLAGS_OR_ZERO(SW0_NODE))
#else
#error "Unsupported board: sw0 devicetree alias is not defined"
#define SW0_GPIO_LABEL	""
#define SW0_GPIO_PIN	0
#define SW0_GPIO_FLAGS	0
#endif

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

void button_pressed(struct device *dev, struct gpio_callback *cb,
		    uint32_t pins)
{
	//k_sched_lock();
	printk("Button pressed at %" PRIu32 "\n", k_cycle_get_32());
	k_sleep(K_SECONDS(SLEEP_S));
	sys_set_power_state(SYS_POWER_STATE_ACTIVE);
	//k_sched_unlock();
	
	

	// networking

	printk("\n Resuming system");
	k_busy_wait(SLEEP_S* USEC_PER_SEC);
	
}



void main(void)
{
	int ret;
	struct device *cons = device_get_binding(CONSOLE_LABEL);
	struct device *button;

	button = device_get_binding(SW0_GPIO_LABEL);
	if (button == NULL) {
		printk("Error: didn't find %s device\n", SW0_GPIO_LABEL);
		return;
	}
	//k_sched_lock();
	ret = gpio_pin_configure(button, SW0_GPIO_PIN, SW0_GPIO_FLAGS);
	if (ret != 0) {
		printk("Error %d: failed to configure %s pin %d\n",
		       ret, SW0_GPIO_LABEL, SW0_GPIO_PIN);
		return;
	}

	ret = gpio_pin_interrupt_configure(button,
					   SW0_GPIO_PIN,
					   GPIO_INT_EDGE_TO_ACTIVE);
	if (ret != 0) {
		printk("Error %d: failed to configure interrupt on %s pin %d\n",
			ret, SW0_GPIO_LABEL, SW0_GPIO_PIN);
		return;
	}

	gpio_init_callback(&button_cb_data, button_pressed, BIT(SW0_GPIO_PIN));
	gpio_add_callback(button, &button_cb_data);
	printk("Set up button at %s pin %d\n", SW0_GPIO_LABEL, SW0_GPIO_PIN);

	/* Configure to generate PORT event (wakeup) on button 1 press. */
	nrf_gpio_cfg_input(DT_GPIO_PIN(DT_NODELABEL(button0), gpios),
			   NRF_GPIO_PIN_PULLUP);
	nrf_gpio_cfg_sense_set(DT_GPIO_PIN(DT_NODELABEL(button0), gpios),
			       NRF_GPIO_PIN_SENSE_LOW);


	
	printk("\n Entering SYS_POWER_STATE_DEEP_SLEEP_1 mode");
	printk("\n press BUTTON1 to restart\n");
	k_sleep(K_SECONDS(SLEEP_S));

	sys_set_power_state(SYS_POWER_STATE_DEEP_SLEEP_1);
	//k_sched_unlock();
	// resume thread context (c4- power to resume thread)

	ret = device_set_power_state(cons, DEVICE_PM_ACTIVE_STATE, NULL, NULL);

	// networking

	printk("\n Resuming system");
	while (true) {
		/* spin to avoid fall-off behavior */
	}
}
