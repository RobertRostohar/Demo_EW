Board: STMicroelectronics [B-U585I-IOT02A](https://www.st.com/en/evaluation-tools/b-u585i-iot02a.html)
------------------------------------------

Device: **STM32U585AII6QU**  
System Core Clock: **160 MHz**

This setup is configured using **STM32CubeMX**, an interactive tool provided by STMicroelectronics for device configuration.
Refer to ["Create Projects with STM32Cube HAL and STM32CubeMX"](https://www.keil.com/pack/doc/STM32Cube/html/index.html) for additional information.

For **STM32CubeMX** configuration settings please refer to [STM32CubeMX Configuration](STM32CubeMX/STM32CubeMX.pdf).

The Heap/stack settings and CMSIS-Driver assignments are configured in the configuration files of respective software components.

The example project can be re-configured to work on custom hardware.
Refer to ["Migrate STM32 Based Example Projects to Custom Hardware"](https://github.com/MDK-Packs/Documentation/tree/master/Porting_to_Custom_Hardware) for additional information.

### System Configuration

| System resource         | Setting
|:------------------------|:----------------------------------------
| Heap                    | 64 kB (configured in the startup file)
| Stack (MSP)             | 1 kB (configured in the startup file)

### STDIO mapping

**STDIO** is routed to Virtual COM port on the ST-Link (using USART1 peripheral)

### CMSIS-Driver mapping

| CMSIS-Driver  | Peripheral  | Physical connection
|:--------------|:------------|:------------------------------------
| Driver_SPI1   | SPI1        | Arduino UNO R3 connector (CN13)
| Driver_USART3 | USART3      | Arduino UNO R3 connector (CN13)
| Driver_USART4 | UART4       | Bluetooth Low Energy Wireless module
| Driver_WiFi0  | SPI2        | WiFi MXCHIP EMW3080 module

### CMSIS-Driver Virtual I/O mapping

| CMSIS-Driver VIO  | Physical resource
|:------------------|:-------------------------------
| vioBUTTON0        | Button USER (PC13)
| vioLED0           | LED RED (PH6)
| vioLED1           | LED GREEN (PH7)
| vioMotionGyro     | iNEMO 3D gyroscope (ISM330DLC)
| vioMotionAccelero | iNEMO 3D accelerometer (ISM330DLC)
| vioMotionMagneto  | High accuracy 3-axis magnetometer (IIS2MDC)
