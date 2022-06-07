imgtool sign --version "1.0.0" --key ../bl2/root-RSA-3072_1.pem --public-key-format full --align 16 --pad --pad-header --boot-record NSPE --header-size 0x400 --slot-size 0xA0000 --security-counter 1 --confirm --dependencies "(0,"1.6.0"+0)" ./Objects/b_u585i_iot02a_tfm_ns.hex b_u585i_iot02a_tfm_ns_signed.bin
imgtool sign --version "1.0.0" --key ../bl2/root-RSA-3072_1.pem --public-key-format full --align 16 --pad --pad-header --boot-record NSPE --header-size 0x400 --slot-size 0xA0000 --security-counter 1           --dependencies "(0,"1.6.0"+0)" ./Objects/b_u585i_iot02a_tfm_ns.hex b_u585i_iot02a_tfm_ns_ota.bin

bin2hex.py --offset 0x0807A000 b_u585i_iot02a_tfm_ns_signed.bin b_u585i_iot02a_tfm_ns_signed.hex
