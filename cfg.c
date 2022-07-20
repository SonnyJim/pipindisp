#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "cfg.h"

const char delim[] = ",";
int cfg_load (void)
{
	FILE *cfg_file;
	char cfg_line[1024];
	int i = 0;
    fprintf (stdout, "Attempting to load configuration file %s\n", DEFAULT_CFG_FILE);
    cfg_file = fopen (DEFAULT_CFG_FILE, "r");
    
    if (cfg_file == NULL)
    {
    	fprintf (stderr, "Error %s\n", strerror(errno));
        return errno;
    }
   
    while (fgets (cfg_line, 1024, cfg_file) != NULL)
    {
        //Ignore remarks
        if (cfg_line[0] != '#')
	{
	
		strcpy (cfg[i].imagename, strtok (cfg_line, delim));
		fprintf (stdout, "%s\n", cfg[i].imagename);
		cfg[i].frame_count= atoi(strtok (NULL, delim));
		cfg[i].delay = atoi(strtok (NULL, delim));
		fprintf (stdout, "Disp%i |%s |%i |%i\n",i, cfg[i].imagename, cfg[i].frame_count, cfg[i].delay);
		i++;
		if (i > NUM_DISPLAYS)
			return 1;
	}
    }
    fclose (cfg_file);
    return 0;
}
