the flowing parts of OLED has changed:

font.h : add truetype.ttf font.

oled.c : add three functions
	 
	 /*new showchar function, can show new font type*/
	 uint8_t OLED_Showchar(uint8_t x, uint8_t y, uint8_t chr, enum font_t font, uint8_t size, uint8_t mode)
	
	 /*new shownum function,can show new font type*/
	 void OLED_Shownum(uint8_t x, uint8_t y, uint32_t num, uint8_t len, enum font_t font, uint8_t size)
	
	 /*new showstring function, can show new font type */
	 void OLED_Showstring(uint8_t x, uint8_t y, const char *p, enum font_t font, uint8_t size)

oled.h : add an enum type variable and reference of function OLED_Shownum and OLED_Showstring

The reason why we add three new functions to show both truetype font and normal font rather than
 modify the original function is to avoid modifying the original function that we have used.

In the future, you can use the new added function to show number and string in both normal and other fonts.