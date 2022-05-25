# Demo_EW
Demo examples for Embedded World


## Prerequisites

Tools:
 - [CMSIS-Toolbox 0.10.1](https://github.com/Open-CMSIS-Pack/devtools/releases/tag/tools%2Ftoolbox%2F0.10.1)
 - [Keil MDK 5.37](https://www.keil.com/download/product)
 - Arm Compiler 6.18 (part of MDK)

CMSIS packs: see examples for list of required packs


## AWS MQTT Demo

Subdirectory: `AWS_MQTT_MutualAuth_Demo`

Demo for connecting to AWS cloud. See details in [README.md](AWS_MQTT_MutualAuth_Demo/README.md).

Note: Make sure to update the credentials as described.

Demo is available for the following targets:
 - `IP-Stack`: runs on IMXRT1050-EVKB board and uses FreeRTOS+TCP with Ethernet
 - `WiFi`:
   - runs on B-U585I-IOT02A board and uses WiFi interface with on-board WiFi module (default)
   - runs on IMXRT1050-EVKB board and uses WiFi interface with ESP8266 WiFi module (alternative A)
   - runs on IMXRT1050-EVKB board and uses WiFi interface with WizFi360 WiFi module (alternative B)
 - `AVH`: runs on Arm Virtual Hardware Targets (MPS3 platform for Corstone-300)

Required CMSIS packs are listed in the [Demo.csolution.yml](AWS_MQTT_MutualAuth_Demo/Demo.csolution.yml) file.

### Target: `IP-Stack`

Board: IMXRT1050-EVKB  
Connectivity: FreeRTOS+TCP with Ethernet

1. Use `csolution` to create `.cprj` project files  
`csolution convert -s Demo.csolution.yml -c Demo.Debug+IP-Stack`  
`csolution convert -s Demo.csolution.yml -c Demo.Release+IP-Stack`

2. Build a specific project
  - use `cbuild`  
  `cbuild Demo.Debug+IP-Stack.cprj`  
  `cbuild Demo.Release+IP-Stack.cprj`  
  - or use MDK and import `Demo.<build-type>+IP-Stack.cprj` and build with MDK  
  Note: due to current importer limitation it is necessary to manually add the following preprocessor define 
  `MBEDTLS_CONFIG_FILE=\"aws_mbedtls_config.h\"`

3. Run the demo
  - connect the board Debug USB to a PC (provides also power)
  - open terminal on PC and connect to board's serial port (Baud rate: 115200)
  - program the image to the target
    - when using MDK:
      - select CMSIS-DAP Debugger (Options for Target - Debug)
      - use Flash Download to program the image
  - reset the target and observe messages in the terminal

### Target: `WiFi`

Board: B-U585I-IOT02A  
Connectivity: WiFi interface with WiFi module

1. Use `csolution` to create `.cprj` project files  
`csolution convert -s Demo.csolution.yml -c Demo.Debug+WiFi`  
`csolution convert -s Demo.csolution.yml -c Demo.Release+WiFi`

2. Build a specific project
  - use `cbuild`  
  `cbuild Demo.Debug+WiFi.cprj`  
  `cbuild Demo.Release+WiFi.cprj`  
  - or use MDK and import `Demo.<build-type>+WiFi.cprj` and build with MDK  
  Note: due to current importer limitation it is necessary to manually add the following preprocessor define 
  `MBEDTLS_CONFIG_FILE=\"aws_mbedtls_config.h\"`

3. Run the demo
  - connect the board ST-Link USB to a PC (provides also power)
  - open terminal on PC and connect to board's serial port (Baud rate: 115200)
  - program the image to the target
    - when using MDK:
      - use Flash Download to program the image
  - reset the target and observe messages in the terminal

Demo can run also on IMXRT1050-EVKB bard with external WiFi module (ESP8266 or WizFi360). Modify the following project files:
 - [Demo.csolution.yml](AWS_MQTT_MutualAuth_Demo/Demo.csolution.yml):
   - change the target type `WiFi` to use `device: MIMXRT1052DVL6B`
 - [Demo.cproject.yml](AWS_MQTT_MutualAuth_Demo/Demo.cproject.yml):
   - include IMXRT1050-EVKB Board layer for `WiFi` target
   - exclude B-U585I-IOT02A Board layer for `WiFi` target
 - [Board/IMXRT1050-EVKB/Board.clayer.yml](AWS_MQTT_MutualAuth_Demo/Board/IMXRT1050-EVKB/Board.clayer.yml):
   - enable external WiFi module:  
     `- component: Keil::CMSIS Driver:WiFi:ESP8266&UART` or  
     `- component: Keil::CMSIS Driver:WiFi:WizFi360&UART`

### Target: `AVH`

Arm Virtual Hardware: MPS3 platform for Corstone-300  
Connectivity: Socket interface with VSocket

1. Use `csolution` to create `.cprj` project files  
`csolution convert -s Demo.csolution.yml -c Demo.Debug+AVH`  
`csolution convert -s Demo.csolution.yml -c Demo.Release+AVH`

2. Build a specific project
  - use `cbuild`  
  `cbuild Demo.Debug+AVH.cprj`  
  `cbuild Demo.Release+AVH.cprj`  
  - or use MDK and import `Demo.<build-type>+AVH.cprj` and build with MDK  
  Note: due to current importer limitation it is necessary to manually add the following preprocessor define 
  `MBEDTLS_CONFIG_FILE=\"aws_mbedtls_config.h\"`

3. Run the demo
  - within uVision (MDK):
    - select Models ARMv8-M Debugger (Options for Target - Debug)
    - click debugger settings and enter:
      - Command: $KARM\VHT\VHT_MPS3_Corstone_SSE-300.exe
  - when running standalone (via command line) and VHT executable in path):  
    `VHT_MPS3_Corstone_SSE-300 -C mps3_board.visualisation.disable-visualisation=1 -C mps3_board.telnetterminal0.start_telnet=0 -C mps3_board.uart0.out_file=- -a <image>`


## Edge Impulse example

Subdirectory: `EdgeImpulse`

Machine learning demo. See details in [README.md](EdgeImpulse/README.md).

