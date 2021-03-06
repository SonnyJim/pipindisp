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
#include <time.h>
#include <sys/time.h>
#include "cfg.h"

#define HEADER_SIZE 54 //size of the BMP header
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
	long long last_service; //time in ms when the display was updated last
};

struct display disp1, disp2, disp3;
int running;
void video_play (struct display disp);

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
	bcm2835_spi_set_speed_hz (61000000); //Set the SPI speed higher, 6Mhz seems to be the limit for these displays
	return 1;
}

int display_init (struct display *disp)
{
	strcpy (disp->imagename, "");
	disp->frame_count = 0;
	disp->delay = 0;
	disp->direction = 0;
	disp->frame_current = 0;
	disp->direction_current = 0;
	disp->cs_pin = CS0;
	disp->last_service = 0;
	//TODO Perform some test to see if a device is actually connected
	GC9A01_clear();
	GC9A01_display();
	return 1;
}

void display_set (struct display *disp)
{
	strcpy (disp->imagename, cfg[0].imagename);
	disp->frame_count = cfg[0].frame_count;
	disp->delay = cfg[0].delay;
}

int main(int argc, char **argv)
{
	signal(SIGINT, signal_cb_handler);

	if (cfg_load () != 0)
	{
		fprintf (stderr, "Error loading config file\n");
		return -1;
	}
	if (!bcm_init ())
	{
		printf ("bcm_init() failed\n");
		return -1;
	}
	display_init (&disp1);
	display_set (&disp1);
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
	video_play(disp1);

	printf ("Cleaning up..\n");
    	GC9A01_clear();
    	GC9A01_display();
  	bcm2835_spi_end();
   	bcm2835_close();
   	return 0;
}

void video_load_buffer_raw (struct display *disp)
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

	for (int i=0;i < disp->frame_count ;i++)
	{
		//Generate the filename and open it
    		sprintf (disp->filename, "%s%s/image%03d.bmp", WORKING_DIR, disp->imagename, i+1);
		bmpfile = fopen(disp->filename, "r");
    		if (bmpfile == NULL) 
    		{
       			printf("Error couldn't open file for reading %s Error %i \n", disp->filename, errno);
        		exit(1);
    		}	
		else
			printf ("Opening %s\n", disp->filename);
		//Seek past the header
		fseek(bmpfile, HEADER_SIZE, SEEK_SET);
		//Read into the buffer
		fread(framedata, PIXEL_SIZE, TFT_WIDTH * TFT_HEIGHT, bmpfile);
		fclose(bmpfile);
		//Translate the colour information and copy it into the display buffer
		GC9A01_bitmap24_buff (0,0, framedata, TFT_WIDTH, TFT_HEIGHT, disp->buffer + (FRAME_SIZE*i));
	}
	printf ("\n");
	free (framedata);
}

void video_playback_frame (struct display *disp)
{
	GC9A01_display_buff(disp->buffer + (disp->frame_current*FRAME_SIZE), TFT_WIDTH * TFT_HEIGHT * 2);
	disp->frame_current++;
	if (disp->frame_current >= disp->frame_count)
		disp->frame_current = 0;
}

long long current_timestamp ()
{
	struct timeval tv;
	gettimeofday(&tv,NULL);
	return (((long long)tv.tv_sec)*1000)+(tv.tv_usec/1000);
}

void video_play (struct display disp)
{
	printf ("buffer size: %i\n", FRAME_SIZE * disp.frame_count);
	disp.buffer = malloc (FRAME_SIZE * disp.frame_count);
	if (disp.buffer == NULL)
	{
		printf ("Error mallocing %i bytes for disp buffer\n", FRAME_SIZE * disp.frame_count);
		exit (1);
	}
	video_load_buffer_raw (&disp);
	printf ("Starting playback\n");
	running = 1;
	while (running)
	{
	
		if (current_timestamp() > disp.last_service + disp.delay)
		{
			video_playback_frame (&disp);
			disp.last_service = current_timestamp();
		}
	}
}
