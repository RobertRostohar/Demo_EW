# Edge Impulse Example: stand-alone inferencing (Alif)

This builds and runs an exported impulse locally on your machine. 

## Prerequisites
1. Create an edge impulse account at [edgeimpulse.com](https://www.edgeimpulse.com/) [optional]
2. [Keil MDK 5.37](https://www.keil.com/download/product)

## Downloading your Edge Impulse model [optional]
Using an Edge Impulse project with the Alif beta enabled, navigate to the **Deployment** tab. Here you should see an `Ethos-U library` deployment option. Select this and click **build** to download a zip file of the contents. If your project does not have the ethos deployment option, contact [david@edgeimpulse.com](david@edgeimpulse.com)

Additionally, see the [image-example](https://github.com/edgeimpulse/example-standalone-inferencing-alif/tree/image-example) branch for an example of an already downloaded project.

## Add static features [optional]
Edge Impulse static examples show the minimal code to run inference, with no peripherals connected to provide data. Instead, we provide a static buffer of data obtained from a studio sample. Follow the steps described [here](https://docs.edgeimpulse.com/docs/running-your-impulse-locally-zephyr#running-the-impulse) to extract sample features from studio, and use these to populate the features array in `source/main.cpp`

For pre-configured example code with features and edge-impulse libraries already populated. See the [image-example](https://github.com/edgeimpulse/example-standalone-inferencing-alif/tree/image-example) branch of this repository.

## Build the firmware
1. Open project `example.uvprojx` in uVision
2. Choose one of the following targets:
    1. Alif E7 M55_HP: Alif E7 Cortex M55 core 0 (high performance) OR
    2. Alif E7 M55_HE: Alif E7 Cortex M55 core 1 (high efficiency) OR
    3. AVH_Corstone-300_U55: Arm Virtual Hardware Corstone-300 with Ethos U55
3. Build the target

## Flash your device
Once programmed, the firmware will run inference and print the results over the serial port.

## Serial out is on UART2, which is connected to pins P3_16,P3_17 (on connector J413)

- Baud 115200
- "8,N,1" (the typical settings for everything else, 8 bit, no parity, 1 stop bit)
- 1.8 Vcc

# Other details

## Known issues

With debugger attached, my device boots up directly into Bus_Fault (or possibly another fault).  This can especially happen when you entered Hard Fault before your last reset.

- Power cycle your board and reload your program

