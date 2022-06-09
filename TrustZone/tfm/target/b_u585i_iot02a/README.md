# TF-M for B-U585I-IOT02A

Prebuilt TF-M and BL2 for STMicroelectronics B-U585I-IOT02A Discovery Kit.

Repository: https://git.trustedfirmware.org/TF-M/trusted-firmware-m.git  
Tag: TF-Mv1.6.0

Modified memory layout (see [patch](flash_layout.patch)):
 - BL2 size: 96 kB (instead of 84 kB)
 - Non-secure image size: 640 kB (instead of 512 kB)

Disabled Crypto Hardware Accelerator (see [patch](crypto_hw_accelerator.patch))

Toolchain: GNU Arm Embedded Toolchain 10.3-2021.10

Configuration and Build commands:

`> mkdir build && cd build`

`> cmake .. -DTFM_PLATFORM=stm/b_u585i_iot02a -DTFM_TOOLCHAIN_FILE=../toolchain_GNUARM.cmake -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug -DTFM_DEV_MODE=1 -DTFM_ISOLATION_LEVEL=2`

`> cmake --build ./  -- install`
