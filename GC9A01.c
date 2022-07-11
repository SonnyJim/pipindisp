/***************************************************
//Web: http://www.buydisplay.com
EastRising Technology Co.,LTD
****************************************************/

#include <bcm2835.h>
#include <stdio.h>
#include "GC9A01.h"

char buffer[TFT_WIDTH * TFT_HEIGHT * 2];

void command(char cmd) {
    bcm2835_gpio_write(DC, LOW);
    bcm2835_spi_transfer(cmd);
}

void data(char cmd) {
    bcm2835_gpio_write(DC, HIGH);
    bcm2835_spi_transfer(cmd);
}

void GC9A01_begin()
{
    bcm2835_gpio_fsel(RST, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(DC, BCM2835_GPIO_FSEL_OUTP);

    bcm2835_spi_begin();
    bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);     //The default
    bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);                  //The default
    bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_64);  //The default
    bcm2835_spi_chipSelect(BCM2835_SPI_CS0);                     //The default
    bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);     //the default

    bcm2835_gpio_write(RST, HIGH);
    bcm2835_delay(10);
    bcm2835_gpio_write(RST, LOW);
    bcm2835_delay(10);
    bcm2835_gpio_write(RST, HIGH);
    bcm2835_delay(100);
   
 
  //************* Start Initial Sequence **********// 
  	command(0xEF);
 
	command(0xEB);
	data(0x14); 
	
        command(0xFE);			 
	command(0xEF); 

	command(0xEB);	
	data(0x14); 

	command(0x84);			
	data(0x40); 

	command(0x85);			
	data(0xFF); 

	command(0x86);			
	data(0xFF); 

	command(0x87);			
	data(0xFF);

	command(0x88);			
	data(0x0A);

	command(0x89);			
	data(0x21); 

	command(0x8A);			
	data(0x00); 

	command(0x8B);			
	data(0x80); 

	command(0x8C);			
	data(0x01); 

	command(0x8D);			
	data(0x01); 

	command(0x8E);			
	data(0xFF); 

	command(0x8F);			
	data(0xFF); 


	command(0xB6);
	data(0x00); 			
	data(0x00); 

	command(0x36);
	
	if(USE_HORIZONTAL==0)data(0x18);
	else if(USE_HORIZONTAL==1)data(0x28);
	else if(USE_HORIZONTAL==2)data(0x48);
	else data(0x88);
  


	command(0x3A);			
	data(0x05); 


	command(0x90);			
	data(0x08);
	data(0x08);
	data(0x08);
	data(0x08); 

	command(0xBD);			
	data(0x06);
	
	command(0xBC);			
	data(0x00);	

	command(0xFF);			
	data(0x60);
	data(0x01);
	data(0x04);

	command(0xC3);			
	data(0x13);
	command(0xC4);			
	data(0x13);

	command(0xC9);			
	data(0x22);

	command(0xBE);			
	data(0x11); 

	command(0xE1);			
	data(0x10);
	data(0x0E);

	command(0xDF);			
	data(0x21);
	data(0x0c);
	data(0x02);

	command(0xF0);   
    	data(0x45);
    	data(0x09);
 	data(0x08);
  	data(0x08);
 	data(0x26);
 	data(0x2A);

 	command(0xF1);    
 	data(0x43);
 	data(0x70);
 	data(0x72);
 	data(0x36);
 	data(0x37);  
 	data(0x6F);


 	command(0xF2);   
 	data(0x45);
 	data(0x09);
 	data(0x08);
 	data(0x08);
 	data(0x26);
 	data(0x2A);

 	command(0xF3);   
 	data(0x43);
 	data(0x70);
 	data(0x72);
 	data(0x36);
 	data(0x37); 
 	data(0x6F);

	command(0xED);	
	data(0x1B); 
	data(0x0B); 

	command(0xAE);			
	data(0x77);
	
	command(0xCD);			
	data(0x63);		


	command(0x70);			
	data(0x07);
	data(0x07);
	data(0x04);
	data(0x0E); 
	data(0x0F); 
	data(0x09);
	data(0x07);
	data(0x08);
	data(0x03);

	command(0xE8);			
	data(0x34);

	command(0x62);			
	data(0x18);
	data(0x0D);
	data(0x71);
	data(0xED);
	data(0x70); 
	data(0x70);
	data(0x18);
	data(0x0F);
	data(0x71);
	data(0xEF);
	data(0x70); 
	data(0x70);

	command(0x63);			
	data(0x18);
	data(0x11);
	data(0x71);
	data(0xF1);
	data(0x70); 
	data(0x70);
	data(0x18);
	data(0x13);
	data(0x71);
	data(0xF3);
	data(0x70); 
	data(0x70);

	command(0x64);			
	data(0x28);
	data(0x29);
	data(0xF1);
	data(0x01);
	data(0xF1);
	data(0x00);
	data(0x07);

	command(0x66);			
	data(0x3C);
	data(0x00);
	data(0xCD);
	data(0x67);
	data(0x45);
	data(0x45);
	data(0x10);
	data(0x00);
	data(0x00);
	data(0x00);

	command(0x67);			
	data(0x00);
	data(0x3C);
	data(0x00);
	data(0x00);
	data(0x00);
	data(0x01);
	data(0x54);
	data(0x10);
	data(0x32);
	data(0x98);

	command(0x74);			
	data(0x10);	
	data(0x85);	
	data(0x80);
	data(0x00); 
	data(0x00); 
	data(0x4E);
	data(0x00);					
	
        command(0x98);			
	data(0x3e);
	data(0x07);

	command(0x35);	
	command(0x21);     


  	command(0x11); 
    	bcm2835_delay(120);
  	command(0x29); 
	bcm2835_delay(20);

  
  GC9A01_clear();


}

void GC9A01_clear() {
    int i;
    for(i = 0; i < sizeof(buffer); i++)
    {
        buffer[i] = 0;
    }
}

void GC9A01_draw_point(int x, int y, uint16_t hwColor) {
    if(x >= TFT_WIDTH || y >= TFT_HEIGHT)
    {
        return;
    }
    buffer[x * 2 + y * TFT_WIDTH * 2] = hwColor >> 8;
    buffer[x * 2 + y * TFT_WIDTH * 2 + 1] = hwColor;
}

void GC9A01_char1616(uint8_t x, uint8_t y, uint8_t chChar, uint16_t hwColor) {
    uint8_t i, j;
    uint8_t chTemp = 0, y0 = y;

    for (i = 0; i < 32; i ++) {
        chTemp = Font1612[chChar - 0x30][i];
        for (j = 0; j < 8; j ++) {
            if (chTemp & 0x80) {
                GC9A01_draw_point(x, y, hwColor);
            } else {
                GC9A01_draw_point(x, y, 0);
            }
            chTemp <<= 1;
            y++;
            if ((y - y0) == 16) {
                y = y0;
                x++;
                break;
            }
        }
    }
}

void GC9A01_char3216(uint8_t x, uint8_t y, uint8_t chChar, uint16_t hwColor) {
    uint8_t i, j;
    uint8_t chTemp = 0, y0 = y; 

    for (i = 0; i < 64; i++) {
        chTemp = Font3216[chChar - 0x30][i];
        for (j = 0; j < 8; j++) {
            if (chTemp & 0x80) {
                GC9A01_draw_point(x, y, hwColor);
            } else {
                GC9A01_draw_point(x, y, 0);
            }

            chTemp <<= 1;
            y++;
            if ((y - y0) == 32) {
                y = y0;
                x++;
                break;
            }
        }
    }
}

void GC9A01_char(uint8_t x, uint8_t y, char acsii, char size, char mode, uint16_t hwColor) {
    uint8_t i, j, y0=y;
    char temp;
    uint8_t ch = acsii - ' ';
    for(i = 0;i<size;i++) {
        if(size == 12)
        {
            if(mode)temp=Font1206[ch][i];
            else temp = ~Font1206[ch][i];
        }
        else 
        {            
            if(mode)temp=Font1608[ch][i];
            else temp = ~Font1608[ch][i];
        }
        for(j =0;j<8;j++)
        {
            if(temp & 0x80) GC9A01_draw_point(x, y, hwColor);
            else GC9A01_draw_point(x, y, 0);
            temp <<=1;
            y++;
            if((y-y0)==size)
            {
                y = y0;
                x ++;
                break;
            }
        }
    }
}

void GC9A01_string(uint8_t x, uint8_t y, const char *pString, uint8_t Size, uint8_t Mode, uint16_t hwColor) {
    while (*pString != '\0') {       
        if (x > (TFT_WIDTH - Size / 2)) {
            x = 0;
            y += Size;
            if (y > (TFT_HEIGHT - Size)) {
                y = x = 0;
            }
        }
        
        GC9A01_char(x, y, *pString, Size, Mode, hwColor);
        x += Size / 2;
        pString ++;
    }
}

void GC9A01_mono_bitmap(uint8_t x, uint8_t y, const uint8_t *pBmp, char chWidth, char chHeight, uint16_t hwColor) {
    uint8_t i, j, byteWidth = (chWidth + 7) / 8;
    for(j = 0; j < chHeight; j++) {
        for(i = 0; i <chWidth; i ++) {
            if(*(pBmp + j * byteWidth + i / 8) & (128 >> (i & 7))) {
                GC9A01_draw_point(x + i, y + j, hwColor);
            }
        }
    }        
}

void GC9A01_bitmap24(uint8_t x, uint8_t y, uint8_t *pBmp, char chWidth, char chHeight) {
    uint8_t i, j;
    uint16_t hwColor;
    uint32_t temp;

    for(j = 0; j < chHeight; j++) {
        for(i = 0; i < chWidth; i ++) {
            temp = *(unsigned int*)(pBmp + i * 3 + j * 3 * chWidth);
            hwColor = RGB(((temp >> 16) & 0xFF),
                          ((temp >> 8) & 0xFF),
                           (temp & 0xFF));
            GC9A01_draw_point(x + i, y + chHeight - 1 - j, hwColor);
        }
    }
}

void GC9A01_display() {
 
    command(0x2a);
    data(0);
    data(0);
    data((TFT_WIDTH-1) >> 8);
    data(TFT_WIDTH-1);

    command(0x2b);
    data(0);
    data(0);
    data((TFT_HEIGHT-1) >> 8);
    data(TFT_HEIGHT-1);

    command(0x2C);
    bcm2835_gpio_write(DC, HIGH);
    bcm2835_spi_transfern(buffer, sizeof(buffer));
}

void GC9A01_clear_screen(uint16_t hwColor) {
    uint16_t i, j;
    for(i = 0; i < TFT_HEIGHT; i++) {
        for(j = 0; j < TFT_WIDTH; j ++) {
            GC9A01_draw_point(j, i, hwColor);
        }
    }
}

