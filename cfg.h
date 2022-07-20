/*
 * =====================================================================================
 *
 *       Filename:  cfg.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  20/07/22 21:53:39
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Dr. Fritz Mehner (mn), mehner@fh-swf.de
 *        Company:  FH Südwestfalen, Iserlohn
 *
 * =====================================================================================
 */
#define DEFAULT_CFG_FILE "/etc/pipindisp.cfg"
struct config
{
	char imagename[255];
	int16_t frame_count;
	int delay;
};

#define NUM_DISPLAYS 3
struct config cfg[NUM_DISPLAYS];
int cfg_load ();
