.. _nrf-system-off-sample:

nRF52832 Deep Sleep Exit Demo using GPIO BSP Interrupt
#####################

Overview
********
In this sample, a wakeup procedure from SYS_POWER_STATE_DEEP_SLEEP_1 is demonstrated using GPIO pin interrupt. First, the nR

Requirements
************

This application uses nRF51 DK or nRF52 DK board for the demo.

Building, Flashing and Running
******************************

.. zephyr-app-commands::
   :zephyr-app: samples/boards/nrf52/system_off
   :board: nrf52dk_nrf52832
   :goals: build flash
   :compact:

Running:

1. Open UART terminal.
2. Power Cycle Device.
3. Device will demonstrate two activity levels which can be measured.
4. Device will demonstrate long sleep at minimal non-off power.
5. Device will turn itself off using deep sleep state 1.  Press Button 1
   to wake the device and restart the application as if it had been
   powered back on.

Sample Output
=================
nRF52 core output
-----------------

.. code-block:: console

   *** Booting Zephyr OS build v2.3.0-rc1-204-g5f2eb85f728d  ***

   nrf52dk_nrf52832 system off demo
   Busy-wait 2 s
   Busy-wait 2 s with UART off
   Sleep 2 s
   Sleep 2 s with UART off
   Entering system off; press BUTTON1 to restart
