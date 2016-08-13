###Directory file description###

|-Libraries	 (STM32F10x Standard peripheral library files) 
	|-CMSIS
	|-STM32F10x_StdPeriph_Driver
  
|-Project
	|-mdk: MDK project files
		|-list
		|-output
		|-NFC Watch SDK V1.1.uvopt
		|-NFC Watch SDK V1.1.uvproj	(MDK project file)
	|-sample
		|-logo.cpp				(function about how to show logo on the boot.)
		|-p2p.cpp				(function about reveice and transmit data between Initiator and target.)
		|-rw Mifare tag.cpp		(function about read and write Mifare Tag)
		|-surface.cpp			(function about display time on the OLED)
	|-src
		|-battery
			|-battery.h			(This file provides all battery information functions.)
		|-bsp
			|-bsp.c				(funsction of configuration of hardware peripheral)
			|-bsp.h				(Hardware Configuration)
			|-tim.h				(functions about Timeout judgment.)
		|-OLED
			|-font.h			(font library)
			|-image.h			(image library)
			|-oled.a			(oled library)
			|-oled.h			(oled interface functions)
		|-PN532
			|-PN532_HSU
				|-emdev.h		(function interfaces about USART2's transmit and receive.)
				|-PN532_HSU.h	(pn532 hardware HSU interface)
			|-lisence.txt
			|-llcp.h			(llcp interface)
			|-mac_link.h		(mac_link interface)
			|-PN532.a			(pn532 library)
			|-PN532.h			(pn532 function interfaces)
			|-PN532_debug.h		(debug interface)
			|-PN532Interface.h	(pn532 interface)
			|-snep.h			(snep interface)
		|-cortexm3_macro.s		(boot file(not used))
		|-main.cpp				(main function)
		|-stm32f10x_conf.h		(peripheral configurate)
		|-stm32f10x_it.c		(interrupt functions)
		|-stm32f10x_it.h		(interrupt function interfaces)
		|-stm32f10x_lib.h		(not used)
		|-stm32f10x_vector.c	(define vector table(not used))
		
|-clean.bat						(Clear generation file)
|-readme.txt