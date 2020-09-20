.. _nrf-power-profile:

nRF52832 Power Profiling
#####################

Overview
********

In this sample, the use of power management library and DeviceTree APIs is demonstrated for profiling the power consumption of nRF52-DK board. 
The functional behaviour is:

* Power-Off all subsystem (UART,MCU Manager) after SYS_INIT()
* Suspend main() thread after board initialization
* Force system CPU in ACTIVE state and suspend main() thread for 10 seconds
* Set device Zephyr_Console into Low-power mode and suspend main() thread for 10 seconds
* Force device into SYSTEM OFF State and suspend for 10 seconds
* Override Power Manager Policy to revoke ACTIVE STATE


Device Tree APIs
****************
The Zephyr project provides peripheral-specific SoC Interface to perform power-managment related operations. The list of peripherals are specific in 
:code:`~/zephyrproject/zephyr/build/zephyr/zephyr.dts`.

Building, Flashing and Logging
******************************
For building and flashing the sample, copy/extract the power_profiling directory into the given directory and run the following commands:

.. code-block::

   cd ~/zephyrproject/zephyr
   west build -p auto -b nrf52dk_nrf52832 samples/boards/nrf/power_profiling/
   west flash

To view the :code:`printk()` commands as log messages, open another terminal and execute the following command:

.. code-block::

   JLinkRTTLogger -Device NRF52840_XXAA -RTTChannel 1 -if SWD -Speed 4000 ~/rtt.log

In another terminal, execute the following command:

.. code-block::

   nc localhost 19021



Sample Output
=================
nRF52 JLink RTT Viewer output
-----------------

.. code-block:: console

   SEGGER J-Link V6.82b - Real time terminal output
   J-Link OB-SAM3U128-V2-NordicSemi compiled Mar 17 2020 14:43:00 V1.0, SN=682885846
   Process: JLinkRTTLoggerExe
   *** Booting Zephyr OS build zephyr-v2.3.0-1342-g75949f470f56  ***
   After Board Init--> Suspending for 10 secs
   <-- Forcing SYS_POWER_STATE_ACTIVE state --->
   SYS_POWER_STATE_ACTIVE --> Suspending for 10 secs
   <-- Forcing DEVICE_PM_LOW_POWER_STATE state --->
   UART_0 --> Suspending for 10 secs
   <-- Forcing DEVICE_PM_OFF_STATE state --->
   UART_0 --> Suspending for 10 secs
   <-- Forcing DEVICE_PM_ACTIVE_STATE state --->


