/***************************************************
//Web: http://www.buydisplay.com
EastRising Technology Co.,LTD
Examples for ER-TFTM1.28-1
Display is Hardward SPI 4-Wire SPI Interface 
Tested and worked with: 
Works with Raspberry pi
****************************************************/

#include <bcm2835.h>
#include <stdio.h>
#include "GC9A01.h"

#define WORKING_DIR "/usr/local/share/pipindisp/data/"
int main(int argc, char **argv)
{
    FILE *pFile ;
    /* 1 pixel of 888 bitmap = 3 bytes */
    size_t pixelSize = 3;
    unsigned char bmpBuffer[TFT_WIDTH * TFT_HEIGHT * 3];
    char filename[400];
    char imagename[200];
    int frame_num = 1;
    int direction = 1;
    int pingpong = 0;
    int total_frames = 77;
    int delay;
    if(!bcm2835_init())
    {
	printf("BCM2835 init failed\n");
        return -1;
    }

  
    GC9A01_begin();
    bcm2835_spi_set_speed_hz (40000000); //Set the SPI speed higher
    delay = 0;
    direction = 0;
   
   /* 
    sprintf (imagename, "Neon_swirl");
    total_frames = 29;
   */ 
    
    sprintf (imagename, "Fire_swirl");
    total_frames = 50;
    /*
    sprintf (imagename, "Elvira");
    pingpong = 1;
    total_frames = 77;

    sprintf (imagename, "Fire_swirl");
    total_frames = 50;
    sprintf (imagename, "Pink_swirl");
    total_frames = 24;
    sprintf (imagename, "Pinball_bum");
    total_frames = 8;
    delay = 40;
    sprintf (imagename, "fractal");
    total_frames = 49;
    sprintf (imagename, "trippy");
    total_frames = 18;
    delay = 40;
    sprintf (imagename, "Oil_swirl");
    total_frames = 24;
    sprintf (imagename, "black_hole");
    total_frames = 180;
    sprintf (imagename, "Pinball_bum");
    total_frames = 8;
    delay = 60;
    
*/
    while (1)
    {
    	sprintf (filename, "%s%s/image%03d.bmp", WORKING_DIR, imagename, frame_num);
     	pFile = fopen(filename, "r");
    	if (pFile == NULL) 
    	{
       		printf("file not exist\n");
		printf (filename);
        	return 0;
    	}
	fseek(pFile, 54, 0);
	fread(bmpBuffer, pixelSize, TFT_WIDTH * TFT_HEIGHT, pFile);
	fclose(pFile);
	//GC9A01_setdisplay (0);
	GC9A01_bitmap24(0, 0, bmpBuffer, 240, 240);
	GC9A01_display();
	bcm2835_delay (delay);
    	if (pingpong)
    	{
    		if (direction)
  	  	{
		    frame_num++;
    		    if (frame_num >= total_frames)
			direction = 0;
  	  	}
		else
		{
			frame_num--;
			if (frame_num <= 1)
				direction = 1;
		}
    	}
    	else
    	{
    		frame_num++;
		if (frame_num > total_frames)
			frame_num = 1;
	}
   }
   bcm2835_spi_end();
   bcm2835_close();
   return 0;
}

