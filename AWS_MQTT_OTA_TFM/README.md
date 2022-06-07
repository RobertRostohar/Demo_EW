# Getting started with STM32U5 IoT Reference example

## Introduction

This project demonstrates how to use **Trusted Firmware for Cortex-M (TF-M)** based software development with hardware enforced security 
to build more secure updatable cloud connected applications.  
The project is pre-configured to run on the [ STM32U585 IoT Discovery Kit ](https://www.st.com/en/evaluation-tools/b-u585i-iot02a.html) 
which includes an [ STM32U5 ](https://www.st.com/en/microcontrollers-microprocessors/stm32u5-series.html) microcontroller.

The demo example connects to AWS IoT Core via the on-board WiFi module and uses the [CoreMQTT-Agent](https://github.com/FreeRTOS/coreMQTT-Agent) library 
to share a single MQTT connection among multiple tasks. These tasks publish environment and motion sensor data 
from a subset of the sensors available on the development board, and demonstrate use of the AWS IoT Device Shadow and Device Defender services.

The main part of the example is secure **Over-The-Air firmware update** of the secure or non-secure firmware using 
the Amazon AWS OTA firmware update process.

The original example is hosted on the GitHub repository at the following location:  
https://github.com/FreeRTOS/iot-reference-stm32u5

To clone the repository using HTTPS use the following Git Bash command:
```
git clone https://github.com/FreeRTOS/iot-reference-stm32u5.git --recurse-submodules
```

> Note: If the cloning fails make sure that git has support for long paths enabled, if not you can enable it by executing the following Git Bash command
(Git Bash has to be started in administrator mode):
```
git config --system core.longpaths true
```

For more information on the original example functionality refer to the [FreeRTOS STM32U5 IoT Reference documentation](https://github.com/FreeRTOS/iot-reference-stm32u5/blob/main/README.md).

# Running the example

## Required software (for Microsoft Windows)

  - [MDK-ARM v5.37 or later](https://www2.keil.com/mdk5)
  - [git](https://git-scm.com/download/win/)
  - [python v3.10 or later](https://www.python.org/downloads/windows/) (with pip)
  - [AWS CLI2](https://awscli.amazonaws.com/AWSCLIV2.msi)
  - [STM32CubeProgrammer v2.10.0 or later](https://www.st.com/en/development-tools/stm32cubeprog.html)

**MDK-ARM requirements**

- installed **Keil::STM32U5xx_DFP v2.0.0** pack

**Python requirements**

  - installed **imgtool v1.9.0**, **boto3 v1.22.0**, **requests**, **pyserial** modules  
    to install execute the following command in Git Bash shell:
    ```
    pip install imgtool==1.9.0 boto3==1.22.0 requests pyserial
    ```

**AWS CLI2 requirements**

  - **Amazon AWS Cloud account**, if you do not have one please follow the documentation to 
    [create and activate the Amazon AWS Cloud account](https://aws.amazon.com/premiumsupport/knowledge-center/create-and-activate-aws-account/)

  - Enable AWS CLI access following the [official documentation](https://docs.aws.amazon.com/cli/latest/userguide/cli-configure-quickstart.html) 
    or alternatively following the steps below:
    - [Log into Amazon AWS Cloud Console](https://console.aws.amazon.com/)
    - open **IAM** services
    - under **Access management** select **Users**
    - select desired user
    - select **Security credentials** tab
    - under **Access key** section click on the **Create access** key button
    - download the created **.csv** file or store your **Access key ID** and **Secret access key** locally
    - from Windows Command Shell execute the following command:
    ```
    aws configure
    ```
     - Fill in the required fields (first two as retrieved in previous steps): 
    ```
    AWS Access Key ID [****************xxxx]: Your access key ID
    AWS Secret Access Key [****************xxxx]: Your Secret access key
    Default region name [eu-central-1]: Your default region
    Default output format [json]: json
    ```

## Board and microcontroller setup

**Board setup**

  - Insure that the **5V_USB_STLK** and **JP3** jumpers are bridged and the remaining jumpers are not bridged
  - Check that the **BOOT0** DIP switch is in the **0** / right position (closest to the ST-LINK STLK CN8 USB connector)
  - Connect a **USB micro-B cable** between the **STLK** connector and your **Personal Computer**
  - Start the **STM32CubeProgrammer**
  - Check if **Firmware upgrade** is available, if it is then update the ST-LINK firmware to the latest available
  - Close the **STM32CubeProgrammer**

**WiFi module firmware update**

  - Download the [EMW3080 update tool](https://www.st.com/content/ccc/resource/technical/software/firmware/group1/48/a2/e8/27/7f/ae/4b/26/x-wifi-emw3080b/files/x-wifi-emw3080b.zip/jcr:content/translations/en.x-wifi-emw3080b.zip) from the STMicroelectronics website
  - Extract downloaded archive
  - Flip the **BOOT0** switch to the **1** / left position
  - Press the **RST** (black) button to reset the STM32U5 microcontroller
  - Drag and drop the **EMW3080updateV2.1.11RevC.bin** binary from the subfolder **V2.1.11\\SPI\\** of the previously 
    extracted **MW3080 update tool** package to the **DIS_U585AI** USB mass storage device
  - Wait for the mass storage device to disconnect and then reconnect
  - Return **BOOT0** to the **0** / right position
  - Press the **RST** button again
  - Connect a serial terminal program to the **ST-LINK Virtual COM** port
  - Press the blue **USER** button and wait for the firmware update to complete

  > Note: When TrustZone is enabled the USB device does not show mass storage device as **DIS_U585AI** but as **NOD_U585AI** which 
    cannot be used for firmware update. To update the EMW3080 firmware the TrustZone has to be disabled.

**Microcontroller (STM32U585) setup**

  - Start the **STM32CubeProgrammer**
  - Click on **Connect** button (ST-LINK, SWD)
  - Click on the **OB** Option Bytes tab and configure the following values for Option Bytes:

    Option Bytes **User Configuration**

    Name         | Value     
    :------------|:----------
    TZEN         | Checked
    SRAM2_RST    | Unchecked

    Option Bytes **Boot Configuration**

    Name         | Value     
    :------------|:----------
    SECBOOTADDR0 | 0x180280 (address = 0x0C014000)

  - Click on the **Apply** button to update all **OB** Option Bytes changes  
    (if it fails, then **Apply** each Option Byte change separately)

  > Note: These settings are required by the TF-M Bootloader (BL2)

## Programming the microcontroller

The layout of the microcontroller internal flash memory is described in the table below: 

| Offset     | Bank |  Pages  | Region Name                     | Size (Dec.) | Size (Hex.) | Image Suffix                               |
|------------|------|---------|---------------------------------|-------------|-------------|------------------------------------------- |
| 0x00000000 | 1    | 0, 7    | Scratch                         |    64 KB    |   0x10000   | N/A                                        |
| 0x00010000 | 1    | 8, 8    | BL2 - NV Counters               |     8 KB    |   0x02000   |                                            |
| 0x00012000 | 1    | 9, 9    | BL2 - NV Counters initial value |     8 KB    |   0x02000   | **b_u585i_iot02a_tfm_bl2.bin** image       |
| 0x00014000 | 1    | 10, 21  | BL2 - MCUBoot HDP Code          |    96 KB    |   0x18000   | **b_u585i_iot02a_tfm_bl2.bin** image       |
| 0x0002C000 | 1    | 22, 25  | BL2 - SPE Shared Code           |    28 KB    |   0x07000   | **b_u585i_iot02a_tfm_bl2.bin** image       |
| 0x00033000 | 1    | 25, 25  | OTP Write Protect               |     4 KB    |   0x01000   | N/A                                        |
| 0x00034000 | 1    | 26, 27  | NV counters area                |    16 KB    |   0x04000   | N/A                                        |
| 0x00038000 | 1    | 28, 29  | Secure Storage Area             |    16 KB    |   0x04000   | N/A                                        |
| 0x0003C000 | 1    | 30, 31  | Internal Trusted Storage Area   |    16 KB    |   0x04000   | N/A                                        |
| 0x00040000 | 1    | 32, 63  | Secure image     primary slot   |   256 KB    |   0x40000   | **b_u585i_iot02a_tfm_s_signed.bin** image  |
| 0x00080000 | 1-2  | 64, 16  | Non-secure image primary slot   |   640 KB    |   0xA0000   | **b_u585i_iot02a_tfm_ns_signed.bin** image |
| 0x00120000 | 2    | 17, 48  | Secure image     secondary slot |   256 KB    |   0x40000   | **b_u585i_iot02a_tfm_s_ota.bin** image     |
| 0x00160000 | 2    | 49, 127 | Non-secure image secondary slot |   640 KB    |   0xA0000   | **b_u585i_iot02a_tfm_ns_ota.bin** image    |

For additional information on protection mechanisms and images please check the original example [documentation](https://github.com/FreeRTOS/iot-reference-stm32u5/tree/main/Projects/b_u585i_iot02a_tfm).

> Note: The **_ota** image is the same as **_signed** image except it does not have a **confirm** flag set in the metadata header of the image

**Loading the Bootloader and TF-M secure image (using STM32CubeProgrammer)**

  - If device is not already connected then click on **Connect** button (ST-LINK, SWD)
  - Click on the **Download** icon (green down arrow) that opens **Erasing & Programming** tab
  - Under **Download - File Path** select **b_u585i_iot02a_tfm_bl2.hex** file located in **Projects\\bl2\\** subfolder
  - Click on the **Start Programm...** button
  - Wait for download and verify to finish
  - Under **Download - File Path** select **b_u585i_iot02a_tfm_s_signed.hex** file located in **Projects\\tfm\\** subfolder
  - Click on the **Start Programm...** button
  - Click on the **Disconnect** button

# User application

User application can be developed in the **MDK-ARM** and then downloaded and debugged directly on the microcontroller.

To test the application follow the steps below:
  - Open the **Projects\\app\\Project.uvprojx** in uVision IDE
  - Click on the **Build (F7)** button to build the application
  - After build finishes the application image will be **signed** (using the sign_image.bat script)
  - Click on the **Download (F8)** button to download the application to the flash memory of the microcontroller
  - Press the **RST** (black) button to reset the board and the microcontroller will start the application execution

**Provision Your Board for the Cloud connection**

To provision your board start the **provision.py** script from the Git Bash shell by executing the following command:
```
winpty python Tools/provision.py --interactive
```

proceed entering required data in serial terminal window:
```
Target device path: COM5
Connecting to target...
[ INFO ] Found credentials in shared credentials file: ~/.aws/credentials (credentials.py:load)
Interactive configuration mode: Press return to use defaults (displayed in brackets)
time_hwm[1651013601]: <return>
mqtt_port[8883]: <return>
wifi_ssid[]: wifi_ssid<return>
wifi_credential[]: wifi_password<return>
mqtt_endpoint[xxxxxxxxxxxxxx-ats.iot.xxxxxxxxx.amazonaws.com]: xxxxxxxxxxxxxx-ats.iot.xxxxxxxxx.amazonaws.com<return>
thing_name[xxxxxxxxxxxxxxxx]: Thing_B_U585I<return>
Commiting target configuration...
Generating a new public/private key pair
Generating a self-signed Certificate
Attaching thing: xxxxxxxxxxxxxxxx to principal: arn:aws:iot:xxxxxxxxx:xxxxxxxxxxxxxx:cert/xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
Attaching the "AllowAllDev" policy to the device certificate.
Importing root ca certificate: "Starfield Services Root Certificate Authority - G2"
Provisioning process complete. Resetting target device...
```
> Note: **mqtt_endpoint** can be found in the Amazon AWS Cloud console in **IoT Core** service under [Settings](https://console.aws.amazon.com/iot/home#/settings)

The **PROVISIONING** process does the following:
  - Sets a **Thing Name** on the device
  - Sets the **WiFi SSID** and **Wifi password** on the device (for connecting to the network via the WiFi)
  - Sets the **MQTT endpoint** on the device
  - Generates ECDSA device **key pair**
  - Generates a self-signed **certificate**
  - **Registers** the previously generated self-signed **certificate** with the **IoT Core** service on the Amazon AWS **Cloud**
  - **Registers** the **Thing Name** on the Amazon AWS **Cloud**
  - **Attaches** the **Certificate** to the thing on the Amazon AWS **Cloud**
  - **Registers** a **Policy** for the thing on the Amazon AWS **Cloud**
  - **Attaches** the **Policy** to the thing on the Amazon AWS **Cloud**

After provisioning is done the device will connect to the Amazon AWS Cloud and start publishing 
environment and motion sensor data.

To see the data that device is publishing to the Cloud you can:
  - Open **IoT Core** on the Amazon AWS Could Console
  - Under **Manage - All devices - Things** select your **Thing** (for example Thing_B_U585I)
  - Select the **Thing** (in example Thing_B_U585I)
  - Select **Activity** tab
  - Click on **MQTT test client** button
  - Under **Topic filter** write
    ```
    #
    ```
  - Click on **Subscribe** button

You should see sensor data as published by the device like shown below:
```
Think_B_U585I/motion_sensor_data
June 03, 2022, 15:00:25 (UTC+0200)

{
  "acceleration_mG": {
    "x": 5,
    "y": -23,
    "z": 1010
  },
  "gyro_mDPS": {
    "x": 490,
    "y": -420,
    "z": -560
  },
  "magnetometer_mGauss": {
    "x": -67,
    "y": 175,
    "z": 502
  }
}

Think_B_U585I/env_sensor_data
June 03, 2022, 15:00:25 (UTC+0200)

{
  "temp_0_c": 36.585812,
  "rh_pct": 9.76733,
  "temp_1_c": 37.139999,
  "baro_mbar": 1003.516602
}
```

## OTA Firmware Update process

The device uses digital signatures to verify the authenticity of the firmware updates sent Over-The-Air.
Images are signed by an authorized source who creates the image, and device can verify the signature of the image, 
using the corresponding public key of the source. 
Steps below shows how to setup and provision the code signing credentials so as to enable Cloud to digitally sign 
the image and the device to verify the image signature before boot.

Generate a Code Signing key:
1. In your working directory, use the following text to create a file named **cert_config.txt**. 
   Replace test_signer@amazon.com with your email address:
   ```
   [ req ]
   prompt             = no
   distinguished_name = my_dn
    
   [ my_dn ]
   commonName         = test_signer@amazon.com
    
   [ my_exts ]
   keyUsage           = digitalSignature
   extendedKeyUsage   = codeSigning
   ```

2. Create an ECDSA code-signing private key:
   ```
   openssl genpkey -algorithm EC -pkeyopt ec_paramgen_curve:P-256 -pkeyopt ec_param_enc:named_curve -outform PEM -out ecdsasigner-priv-key.pem
   ```

3. Generate the corresponding public key from the private key:
   ```
   openssl ec -inform pem -in ecdsasigner-priv-key.pem -pubout -outform pem -out ecdsasigner-pub-key.pem
   ```

4. Create an ECDSA code-signing certificate to be uploaded to the AWS Certificate Manager (ACM) service:
   ```
   openssl req -new -x509 -config cert_config.txt -extensions my_exts -nodes -days 365 -key ecdsasigner-priv-key.pem -out ecdsasigner.crt
   ```

5. Import the code-signing certificate and private key into AWS Certificate Manager (ACM):
   ```
   aws acm import-certificate --certificate fileb://ecdsasigner.crt --private-key fileb://ecdsasigner-priv-key.pem
   ```

   > Note: This command displays an ARN for your certificate. You will need this ARN when you create an OTA update job later.

6. Connect to the target device via a serial terminal.  
   On the command line prompt, type following command to import the OTA signing key:  

   ```
   pki import key ota_signer_pub
   ```

   Then copy paste the `ecdsasigner-pub-key.pem` into the terminal like shown below:  
   ```
   -----BEGIN PUBLIC KEY-----
   XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
   XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX=
   -----END PUBLIC KEY-----
   ```

   You should get information from the device as below:
   ```
   Success: Public Key loaded to label: 'ota_signer_pub'.
   ```

   > Note: `ota_signer_pub` is the label used to refer to the code signing key during verification of the firmware update.

7. Create a signing profile in AWS to sign the firmware image:
   ```
   aws signer put-signing-profile --profile-name <your signing profile name> --signing-material certificateArn=<certificate arn created in step 4> --platform AmazonFreeRTOS-Default --signing-parameters certname=ota_signer_pub
   ```

## Setup OTA S3 bucket, Service role and policies in AWS

1. S3 bucket is used to store the new firmware image to be updated. To create a new S3 bucket follow these steps here:  
   https://docs.aws.amazon.com/freertos/latest/userguide/dg-ota-bucket.html

2. Create a service role which grants permission for OTA service to access the firmware image:  
   https://docs.aws.amazon.com/freertos/latest/userguide/create-service-role.html

3. Create an OTA update policy using the documentation here:  
   https://docs.aws.amazon.com/freertos/latest/userguide/create-ota-user-policy.html

4. Add a policy for AWS IoT to access the code signing profile:  
   https://docs.aws.amazon.com/freertos/latest/userguide/code-sign-policy.html

## Create a code signed firmware update job

1. Bump up the version of the new firmware image to be updated.  

   In the application **sign_image.bat** script change the OTA image signing line by increasing **version** number and increment the **security counter**:  
   ```
   imgtool sign --version "1.0.0" --key ../bl2/root-RSA-3072_1.pem --public-key-format full --align 16 --pad --pad-header --boot-record NSPE --header-size 0x400 --slot-size 0xA0000 --security-counter 1           --dependencies "(0,"1.5.0"+0)" ./Objects/b_u585i_iot02a_tfm_ns.hex b_u585i_iot02a_tfm_ns_ota.bin
   ```

   for example as follows:
   ```
   imgtool sign --version "1.1.0" --key ../bl2/root-RSA-3072_1.pem --public-key-format full --align 16 --pad --pad-header --boot-record NSPE --header-size 0x400 --slot-size 0xA0000 --security-counter 2           --dependencies "(0,"1.5.0"+0)" ./Objects/b_u585i_iot02a_tfm_ns.hex b_u585i_iot02a_tfm_ns_ota.bin
   ```

2. In uVision click on the **Build (F7)** button to build the application that will be contain Bootloader metadata with new version and increased security counter and be signed (for Bootloader verification)

3. Create a new OTA Update job using Amazon AWS Cloud Console:

   - Open **IoT Core** on the Amazon AWS Could Console
   - Under **Manage - Remote actions - Jobs** click on the **Create** button
   - Under **Create job** select **Create FreeRTOS OTA update job** and click on the **Next** button
   - Under **OTA job properties** enter job name (for example FUOTA_B_U585I)
   - Under **OTA file configuration** set:
      - Under **Devices - Device to update** select your Thing
      - Under **Devices - Select the protocol for file transfer** select the **MQTT**
      - Under **File - Sign and choose your file** select the **Sign a new file for me**
      - Under **File - Code signing profile - Existing code signing profile** select the previously created **ota_signer_pub**
      - Under **File - File** select **Upload a new file.**
      - Under **File - File - File to upload** click on the **Choose file** and select the previously generated **b_u585i_iot02a_tfm_ns_ota.bin** file
      - Under **File upload location in S3 - S3 URL**  select the previously created s3 bucket (for example s3://s3-bucket-b-u585i)
      - Under **File upload location in S3 - Path name of file on device**  enter **non_secure image** (for secure image update **secure image**)
      - Under **IAM role - Role** select previously created role for the OTA (for example role-ota-update)
      - Click on the **Next** button
   - Under **OTA job configuration** select the **Your job will complete after deploying to the devices and groups that you chose (snapshot)** type
   - Click on the **Create job** button

#### Monitoring and Verification of firmware update

1. Once the job is created, in the terminal, you will see that OTA job is accepted and device starts downloading the selected image.
   ```
   <INF>  2701148 [OTAAgent  ] Job document was accepted. Attempting to begin the update. (ota.c:2206)
   <INF>  2701148 [OTAAgent  ] Job parsing success: OtaJobParseErr_t=OtaJobParseErrNone, Job name=AFR_OTA-fuota_b_u585i (ota.c:2328)
   <WRN>  2701148 [OTAAgent  ] Received an unhandled callback event from OTA Agent, event = 6 (ota_update_task.c:670)
   <INF>  2701149 [OTAAgent  ] Setting OTA data interface. (ota.c:938)
   <INF>  2701149 [OTAAgent  ] Current State=[CreatingFile], Event=[ReceivedJobDocument], New state=[CreatingFile] (ota.c:2846)
   ```

2. Once the full image has been downloaded, the OTA library verifies the image signature and activates the new image in the unused flash bank.
   ```
   <INF>  2725716 [OTAAgent  ] Received entire update and validated the signature. (ota.c:2666)
   ```

3. New image boots up and performs a self-test, here it checks the version is higher than previous. If so it sets the new image as valid.
   ```
   <INF>    11125 [OTAAgent  ] New image validation succeeded in self test mode. (ota_update_task.c:640)
   ```

4. Checking the job status, should show the job as **Completed**

  - Open **IoT Core** on the Amazon AWS Could Console
  - Under **Manage - Remote actions - Jobs** check status of your OTA job

## Restarting development after OTA update

After secure or non-secure application was updated via the OTA process the Bootloader has already corrected secure area 
size, hide protection, and write protection Option Bytes for security.

To return the device into state where it can be erased and re-programmed, the following procedure has to be executed:  
  - Connect a **USB micro-B cable** between the **STLK** connector and your **Personal Computer**
  - Flip the **BOOT0** switch to the **1** / left position
  - Press the **RST** (black) button to reset the STM32U5 microcontroller
  - Make sure serial terminal is not holding the Virtual COM Port of the ST-LINK
  - Start the **STM32CubeProgrammer**
  - Select the **UARTx** for connection instead of ST-LINK
  - Click on **Connect** button (UARTx)
  - Click on the **OB** Option Bytes tab and configure the following values for Option Bytes:

    Option Bytes **Boot Configuration**

    Name         | Value     
    :------------|:----------
    SECBOOTADDR0 | 0x180000 (address = 0x0C000000)

    Option Bytes **Secure Area 1**

    Name         | Value     
    :------------|:----------
    SECWM1_PSTRT | 0x0  (address = 0x08000000)
    SECWM1_PEND  | 0x7f (address = 0x080fe000)
    HDP1_PEND    | 0x0  (address = 0x08000000)
    HDP1EN       | Unchecked 

    Option Bytes **Write Protection 1**

    Name         | Value     
    :------------|:----------
    WRP1A_PSTRT  | 0x7f (address = 0x080fe000)
    WRP1A_PEND   | 0x0  (address = 0x08000000)
    UNLOCK_1A    | Checked

  - Click on the **Apply** button to update all **OB** Option Bytes changes  
    (if it fails, then **Apply** each Option Byte change separately)
  - Click on the **Disconnect** button
  - Return the **BOOT0** to the **0** / right position
  - Press the **RST** (black) button again
  - Select the **ST-LINK** for connection instead of UARTx
  - Click on the **Connect** button (ST-LINK)
  - Click on the **Download** icon (green down arrow) that opens **Erasing & Programming** tab
  - Click on **Full chip erase** under **Erase flash memory** tab

The device (STM32U5) is now in the initial state so procedure from **Microcontroller (STM32U585) setup** can be 
repeated to put initial version into the device, and also provisioning has to be repeated since 
all data on the device was erased.

> Note: **Full chip erase** should be done with **BOOT0** DIP switch in position **0** because 
  when internal bootloader is active the full chip erase procedure does not work
