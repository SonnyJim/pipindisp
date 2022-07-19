/***************************************************
	pipindisp
	Software for GC9A01 displays

	Based on ER-TFTM1.28-1 by EastRising Technology Co.,LTD
****************************************************/
//TODO Check for already running, better error handling (non-existent files)
#include <bcm2835.h>
#include <stdio.h>
#include <unistd.h>
#include "GC9A01.h"
#include <signal.h>
#include <string.h>
#include <errno.h>

#define HEADER_SIZE 54
#define WORKING_DIR "/usr/local/share/pipindisp/data/"
#define FRAME_SIZE TFT_WIDTH * TFT_HEIGHT * PIXEL_SIZE

struct display
{
	char imagename[255];
	char filename[1024];
	int frame_count;
	int frame_current;
	int delay;
	int direction;
	int direction_current;
	uint8_t *buffer;
	int cs_pin; //Which pin enables the display
};

struct display disp1, disp2, disp3;
int running;
void video_play ();

void signal_cb_handler(int signum) 
{
	printf ("\nCaught signal %i\n", signum);
	running = 0;
}

int bcm_init ()
{
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
	return 1;
}

int display_init ()
{
	strcpy (disp1.imagename, "Pinball_bum");
	disp1.frame_count = 8;
	disp1.delay = 80;
	disp1.direction = 0;
	disp1.frame_current = 0;
	disp1.direction_current = 0;
	disp1.cs_pin = CS0;
	//TODO Perform some test to see if a device is actually connected
	GC9A01_clear();
	GC9A01_display();
	return 1;
}

int main(int argc, char **argv)
{
	signal(SIGINT, signal_cb_handler);
	if (!bcm_init ())
	{
		printf ("bcm_init() failed\n");
		return -1;
	}
	display_init ();
	   /* 
    sprintf (imagename, "Neon_swirl");
    frame_count = 29;
    sprintf (imagename, "Fire_swirl");
    frame_count = 50;
    sprintf (imagename, "Elvira");
    pingpong = 1;
    frame_count = 77;

    sprintf (imagename, "Fire_swirl");
    frame_count = 50;
    sprintf (imagename, "Pink_swirl");
    frame_count = 24;
    sprintf (imagename, "Pinball_bum");
    frame_count = 8;
    delay = 40;
    sprintf (imagename, "fractal");
    frame_count = 49;
    sprintf (imagename, "trippy");
    frame_count = 18;
    delay = 40;
    sprintf (imagename, "Oil_swirl");
    frame_count = 24;
    sprintf (imagename, "black_hole");
    frame_count = 180;
    sprintf (imagename, "Pinball_bum");
    frame_count = 8;
    delay = 60;
    
	*/
//sprintf (disp1.filename, "%s%s/image%03d.bmp", WORKING_DIR, disp1.imagename, disp1.frame_current);
	/*
	disp1.buffer = malloc (TFT_WIDTH * TFT_HEIGHT * PIXEL_SIZE * disp1.frame_count);
	if (disp1.buffer == NULL)
	{
		printf ("Error mallocing buffer for disp1\n");
		return -1;
	}
	*/
	video_play();

	printf ("Cleaning up..\n");
    	GC9A01_clear();
    	GC9A01_display();
  	bcm2835_spi_end();
   	bcm2835_close();
   	return 0;
}

void video_load_buffer_raw (uint8_t *buff)
{
	uint8_t *framedata;
	FILE *bmpfile;
	//Create a buffer to read our bitmap data into
	framedata = malloc(TFT_WIDTH * TFT_HEIGHT * PIXEL_SIZE);
	if (framedata == NULL)
	{
		printf ("Error mallocing bytes for framedata\n");
		exit(1);
	}

	for (int i=0;i < disp1.frame_count ;i++)
	{
		//Generate the filename and open it
    		sprintf (disp1.filename, "%s%s/image%03d.bmp", WORKING_DIR, disp1.imagename, i+1);
		bmpfile = fopen(disp1.filename, "r");
    		if (bmpfile == NULL) 
    		{
       			printf("Error couldn't open file for reading %s Error %i \n", disp1.filename, errno);
        		exit(1);
    		}	
		else
			printf ("Opening %s\n", disp1.filename);
		//Seek past the header
		fseek(bmpfile, HEADER_SIZE, SEEK_SET);
		//Read into the buffer
		fread(framedata, PIXEL_SIZE, TFT_WIDTH * TFT_HEIGHT, bmpfile);
		fclose(bmpfile);
		//Copy and translate the colour information into the display buffer
		GC9A01_bitmap24_buff (0,0, framedata, TFT_WIDTH, TFT_HEIGHT, buff + (FRAME_SIZE*i));
	}
	printf ("\n");
	free (framedata);
}

void video_playback_raw (uint8_t *buff)
{
	int i;
	for (i = 0; i < disp1.frame_count;i++)
	{
		GC9A01_display_buff(buff + (i*FRAME_SIZE), TFT_WIDTH * TFT_HEIGHT * 2);
		bcm2835_delay (disp1.delay);
	}
}

void video_play ()
{
	printf ("buffer size: %i\n", FRAME_SIZE * disp1.frame_count);
	disp1.buffer = malloc (FRAME_SIZE * disp1.frame_count);
	if (disp1.buffer == NULL)
	{
		printf ("Error mallocing %i bytes for disp1 buffer\n", FRAME_SIZE * disp1.frame_count);
		exit (1);
	}
	video_load_buffer_raw (disp1.buffer);
	printf ("Starting playback\n");
	running = 1;
	while (running)
		video_playback_raw (disp1.buffer);
}
 /*
int video_play ()
{
if (pingpong)
    	{
    		if (direction)
  	  	{
		    frame_num++;
    		    if (frame_num >= frame_count)
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
		if (frame_num > frame_count)
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
