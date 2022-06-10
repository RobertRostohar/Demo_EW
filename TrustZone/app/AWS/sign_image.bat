:: Sign Image
::   Input:
::     <name>: hex image to be signed
::   Output:
::     <name>_signed.bin: signed binary image (confirmed)
::     <name>_ota.bin: signed binary for OTA (not confirmed)
::     <name>_signed.hex: signed hex image
::   Run from uVision: sign_image.bat $L@L.hex

@if .%1==. (
 echo Image not specified!
 exit /B 1
)

imgtool sign --version "1.0.0" --key ../../bl2/root-RSA-3072_1.pem --public-key-format full --align 16 --pad --pad-header --boot-record NSPE --header-size 0x400 --slot-size 0xA0000 --security-counter 1 --confirm --dependencies "(0,"1.6.0"+0)" %1 %~n1_signed.bin
imgtool sign --version "1.0.0" --key ../../bl2/root-RSA-3072_1.pem --public-key-format full --align 16 --pad --pad-header --boot-record NSPE --header-size 0x400 --slot-size 0xA0000 --security-counter 1           --dependencies "(0,"1.6.0"+0)" %1 %~n1_ota.bin

bin2hex.py --offset 0x0807A000 %~n1_signed.bin %~n1_signed.hex
