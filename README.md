
Biscuit
=======

Biscuit is an open source firmware for TI CC2540 SoC £¨designed for Bluetooh 4.0 Low Energy applications).

This source code is designed and tested on BLE Mini hardware platform but it can be run on other CC2540 platforms with some modifications.


Features
========

1. BLE periperal role running Biscuit service (with 128-bit UUID) which allows BLE central role devices to connect with it (e.g. iOS Devices)
2. UART driver which can redirect data to/from UART for connecting external devices (e.g. Arduino)
3. I2C driver (Software implmentation), for controlling I2C components (e.g. EEPROM on BLE Mini)
4. Firmware upgradable via USB port (support TI CC2540 UBL 1.3)
5. USB CDC which provides a virtual UART via USB port for connecting to an external device with the USB port (e.g. PC Windows/Linux or Mac OSX)


Requirement
===========

1. CC2540 hardware (e.g. BLE Mini).
2. To compile the Biscuit source code, obtain a copy of IAR Embedded Workbench for 8051 version 8.10 or latest. Refer to TI CC254x development resources.
3. TI CC254x SDK version 1.2.1 - Due to the UART issue, currently not support SDK 1.3, we are trying to fix it with TI support.
4. UBL version 1.3 - We make use of the new USB bootloader so UBL 1.3 code is required.
5. Besides using the bootloader, if you want to program/debug the firmware with hardware (e.g. BLE Mini), you need a CC Debugger.


How to Compile
==============

1. Make sure you know how to do before trying to compile and load the firmware because incorrect configuration can cause the pre-loaded bootloader fails to function.

2. A detailed guideline will be provided soon.


More Information
================

1. Upgrade Firmware via USB Booloder - TI's reference design or BLE Mini have a button connected to P1_2 pin is for triggering the USB upgrade mode. Press and hold the button, connect to a Windows PC, it will show a Mass Storage Drive, you can delete the old firmware and drag the new firmware to it.

2. USB Boot Loader - USB boot loader which is different from typical implementation, please refer to TI's document "Universal Boot Loader for SOC-8051 by USB-MSD Developer's Guide.pdf" and their sample project "HostTestApp" in their latest BLE SDK v1.3

3. TI CC2540 Wiki (resources and other related information for the CC2540 SoC)
   http://processors.wiki.ti.com/index.php/Category:BluetoothLE


License
=======

Copyright (c) 2013 RedBearLab

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal 
in the Software without restriction, including without limitation the rights 
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
