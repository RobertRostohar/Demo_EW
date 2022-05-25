@echo off
VHT_Corstone_SSE-300_Ethos-U55 -C ethosu.num_macs=256 -C mps3_board.visualisation.disable-visualisation=1 -C mps3_board.telnetterminal0.start_telnet=0 -C mps3_board.uart0.out_file=- -C mps3_board.uart0.shutdown_on_eot=1 -a Objects\example.axf %*
