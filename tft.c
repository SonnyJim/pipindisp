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
#include <unistd.h>
#include "GC9A01.h"
#include <sys/stat.h>
#include <signal.h>

#define HEADER_SIZE 54
#define WORKING_DIR "/usr/local/share/pipindisp/data/"

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

int running;

void video_play ();

void signal_cb_handler(int signum) 
{
	printf ("\nCaught signal %i\n", signum);
	running = 0;
}

int main(int argc, char **argv)
{
	signal(SIGINT, signal_cb_handler);
	if(!bcm2835_init() || geteuid() != 0)
	{
		printf("BCM2835 init failed\n");
		printf("Are you root?\n");
		return -1;
	}
	printf ("Dropping priviledges\n");
	printf ("Current GID %i UID %i\n", getgid(), getuid());
	if (setgid(1000) == -1) 
	{
		printf ("Failed to drop root\n");
		return -1;
	}
	if (setuid(1000) == -1) 
	{
		printf ("Failed to drop root\n");
		return -1;
	}
    	printf ("New GID %i UID %i\n", getgid(), getuid());
	
	GC9A01_begin();
	bcm2835_spi_set_speed_hz (60000000); //Set the SPI speed higher, 6Mhz seems to be the limit for these displays
    delay = 0;
    direction = 0;
   
    GC9A01_clear();
    GC9A01_display();
    sprintf (imagename, "trippy");
    total_frames = 18;
    delay = 40;
   /* 
    sprintf (imagename, "Neon_swirl");
    total_frames = 29;
    sprintf (imagename, "Fire_swirl");
    total_frames = 50;
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
   GC9A01_set_brightness (0xFF);
	video_play();

	printf ("Cleaning up..\n");
    GC9A01_clear();
    GC9A01_display();
  bcm2835_spi_end();
   bcm2835_close();
   return 0;
}

long video_get_framesize (char* filename)
{
	struct stat st;
	if (stat(filename, &st) != 0)
	{
		printf ("Error reading filesize of %s\n", filename);
		return 0;
	}
	return (st.st_size - HEADER_SIZE);
}


void video_load_buffer(uint8_t *buffer, long framesize)
{
	printf ("Filling buffer\n");
	
	for (int i=0;i < total_frames;i++)
	{
    		sprintf (filename, "%s%s/image%03d.bmp", WORKING_DIR, imagename, frame_num);
		pFile = fopen(filename, "r");
    		if (pFile == NULL) 
    		{
       			printf("Error couldn't open file for reading %s\n", filename);
        		return;
    		}	
		fseek(pFile, HEADER_SIZE, 0);
		fread(buffer + (framesize*i), pixelSize, TFT_WIDTH * TFT_HEIGHT, pFile);
		fclose(pFile);
		frame_num++;
		printf (".");
	}
	printf ("\n");

}

void video_playback (uint8_t *buffer, long framesize)
{
   	while (running)
	{
		for (int i = 0; i < total_frames;i++)
		{
			GC9A01_bitmap24(0, 0, buffer + (framesize*i), 240, 240);
			GC9A01_display();
		}
	}
}

void video_play ()
{
	uint8_t *buffer;
	sprintf (filename, "%s%s/image%03d.bmp", WORKING_DIR, imagename, 1);
	long framesize =  video_get_framesize(filename);
	printf ("Frame size: %li\n", framesize);
	printf ("buffer size: %li\n", framesize * total_frames);
	buffer = malloc (framesize * total_frames);
	video_load_buffer (buffer, framesize);
	printf ("Starting playback\n");
	running = 1;
	video_playback (buffer, framesize);
 }
 /*
int video_play ()
{
	clock_t begin, end;
    while (1)
    {
	if (frame_num == 1)
		begin = clock();
    	sprintf (filename, "%s%s/image%03d.bmp", WORKING_DIR, imagename, frame_num);
     	pFile = fopen(filename, "r");
    	if (pFile == NULL) 
    	{
       		printf("file not exist\n");
		printf (filename);
        	return 0;
    	}
	fseek(pFile, HEADER_SIZE, 0);
	fread(bmpBuffer, pixelSize, TFT_WIDTH * TFT_HEIGHT, pFile);
	fclose(pFile);
	//GC9A01_setdisplay (0);
	GC9A01_bitmap24(0, 0, bmpBuffer, 240, 240);
	GC9A01_display();
	//bcm2835_delay (delay);
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
		{
			frame_num = 1;
		end = clock();
		double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
		printf("%f\n", time_spent);
		}
	}
   }
 
}
*/
