/***************************************************
//Web: http://www.buydisplay.com
EastRising Technology Co.,LTD
Examples for ER-TFTM1.28-1
Display is Hardward SPI 4-Wire SPI Interface 
Tested and worked with: 
Works with Raspberry pi
****************************************************/
//TODO Check for already running
//
#include <bcm2835.h>
#include <stdio.h>
#include <unistd.h>
#include "GC9A01.h"
#include <sys/stat.h>
#include <signal.h>
#include <string.h>
#include <errno.h>

#define HEADER_SIZE 54
#define WORKING_DIR "/usr/local/share/pipindisp/data/"

FILE *pFile ;
/* 1 pixel of 888 bitmap = 3 bytes */
const size_t pixelSize = 3;
unsigned char bmpBuffer[TFT_WIDTH * TFT_HEIGHT * 3];
char filename[400];
char imagename[200];
int frame_num = 1;
int direction = 1;
int pingpong = 0;
int total_frames = 77;
int delay;

struct display
{
	char imagename[255];
	char filename[1024];
	long framesize;
	int frame_count;
	int frame_current;
	int delay;
	uint8_t *buffer;
	int ce_pin; //Which pin to toggle to enable the display
};

struct display disp1;

int running;

void video_play ();
long video_get_framesize (char* filename);

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
	bcm2835_spi_set_speed_hz (40000000); //Set the SPI speed higher, 6Mhz seems to be the limit for these displays
    delay = 0;
    direction = 0;
   
    GC9A01_clear();
    GC9A01_display();
    sprintf (imagename, "Neon_swirl");
    total_frames = 29;
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
   /*
   strcpy (disp1.imagename, "trippy");
   disp1.frame_count = 18;
   disp1.delay = 0;
   disp1.frame_current = 0;
   sprintf (disp1.filename, "%s%s/image%03d.bmp", WORKING_DIR, disp1.imagename, disp1.frame_current);
   //disp1.framesize = video_get_framesize (disp1.filename);
   disp1.buffer = malloc (TFT_WIDTH * TFT_HEIGHT * pixelSize * disp1.frame_count);
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

void video_load_buffer_raw (uint8_t *buff, long framesize)
{
	uint8_t *framedata;
	FILE *bmpfile;

	framedata = malloc(TFT_WIDTH * TFT_HEIGHT * pixelSize);
	if (framedata == NULL)
	{
		printf ("Error malloc\n");
		exit(0);
	}

	for (int i=0;i < total_frames ;i++)
	{
    		sprintf (filename, "%s%s/image%03d.bmp", WORKING_DIR, imagename, i+1);
		bmpfile = fopen(filename, "r");
    		if (bmpfile == NULL) 
    		{
       			printf("Error couldn't open file for reading %s Error %i \n", filename, errno);
        		return;
    		}	
		else
			printf ("Opening %s\n", filename);
		fseek(bmpfile, HEADER_SIZE, 0);
		fread(framedata, pixelSize, TFT_WIDTH * TFT_HEIGHT, bmpfile);
		fclose(bmpfile);
		GC9A01_bitmap24_buff (0,0, framedata, TFT_WIDTH, TFT_HEIGHT, buff + (framesize*i));
		printf (".");
	}
	printf ("\n");
	free (framedata);


}

void video_playback_raw (uint8_t *buff, long framesize)
{
	int i;

		for (i = 0; i < total_frames;i++)
		{
			GC9A01_display_buff(buff + (i*framesize), TFT_WIDTH * TFT_HEIGHT * 2);
			bcm2835_delay (disp1.delay);
		}
}

void video_play ()
{
	sprintf (filename, "%s%s/image001.bmp", WORKING_DIR, imagename);
	long framesize = TFT_WIDTH * TFT_HEIGHT * pixelSize;
	printf ("Frame size: %li\n", framesize);
	printf ("buffer size: %li\n", framesize * total_frames);
	disp1.buffer = malloc (framesize * total_frames * 2);
	video_load_buffer_raw (disp1.buffer, framesize);
	printf ("Starting playback\n");
	running = 1;
	while (running)
		video_playback_raw (disp1.buffer, framesize);
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
