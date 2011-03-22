#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>
#include "fm_si4708.h"

#define FM_DEV	"/dev/si4708"

int main(int argc, char **argv){
	int status = 0, rv = 0;
	char *argv_value = NULL;
	int argc_c, val, val_list[2];
	char *p, line[81];
	const char *delim = "#;,\\n";
	int fd = open(FM_DEV, O_RDONLY);
	while ((argc_c = getopt(argc, argv, "01a:b:j:ft:s:v:h")) != -1){
		switch(argc_c){
			case 'a' :
				// Audio Track
				argv_value = optarg;
				if (argv_value == NULL || !strlen(argv_value)){
					rv = ioctl(fd, FM_GET_AUDIOTRACK, &status);
					printf("[FM_GET_AUDIOTRACK] - rv = %d; status = %d\n", rv, status);
				}else{
					sscanf(argv_value, "%d", &val);
					rv = ioctl(fd, FM_SET_AUDIOTRACK, &val);
					printf("[FM_SET_AUDIOTRACK] - rv = %d\n", rv);
				}
				break;
			case 'b' :
				// Band
				argv_value = optarg;
				if (argv_value == NULL || !strlen(argv_value)){
					rv = ioctl(fd, FM_GET_BAND, &status);
					printf("[FM_GET_BAND] - rv = %d; status = %d\n", rv, status);
				}else{
					sscanf(argv_value, "%d", &val);
					rv = ioctl(fd, FM_SET_BAND, &val);
					printf("[FM_SET_BAND] - rv = %d\n", rv);
				}
				break;
			case 'j' :
				// Spacers
				argv_value = optarg;
				if (argv_value == NULL || !strlen(argv_value)){
					rv = ioctl(fd, FM_GET_SPACE, &status);
					printf("[FM_GET_SPACE] - rv = %d; status = %d\n", rv, status);
				}else{
					sscanf(argv_value, "%d", &val);
					rv = ioctl(fd, FM_SET_SPACE, &val);
					printf("[FM_SET_SPACE] - rv = %d\n", rv);
				}
				break;
			case 'v' :
				// Volume
				argv_value = optarg;
				if (argv_value == NULL || !strlen(argv_value)){
					rv = ioctl(fd, FM_GET_VOL, &status);
					printf("[FM_GET_VOL] - rv = %d; status = %d\n", rv, status);
				}else{
					sscanf(argv_value, "%d", &val);
					rv = ioctl(fd, FM_SET_VOL, &val);
					printf("[FM_SET_VOL] - rv = %d\n", rv);
				}
				break;
			case 'f' :
				rv = ioctl(fd, FM_GET_CURRENTFREQ, &status);
				printf("[FM_GETCURRENTFREQ] - rv = %d; status = %d\n", rv, status);
				break;
			case 't' :
				// Tune
				argv_value = optarg;
				if (argv_value == NULL || !strlen(argv_value)){
					printf("Tuning, needs a number!");
				}else{
					sscanf(argv_value, "%d", &val);
					rv = ioctl(fd, FM_TUNE, &val);
					printf("[FM_TUNE] - rv = %d\n", rv);
				}
				break;
			case 's' :
				// Seek
				argv_value = optarg;
				if (argv_value == NULL || !strlen(argv_value)){
					printf("Seek needs two parameters separated by #");
				}else{
					rv = 0;
					p = strtok(argv_value, delim);
					val_list[0] = val_list[1] = 0;
					while (p){
						if (rv < 2) sscanf(p, "%d", &val_list[rv++]);
						p = strtok(NULL, delim);
					}
					//printf("%d;%d\n", val_list[0], val_list[1]);
					rv = ioctl(fd, FM_SEEK, val_list);
					printf("[FM_SEEK] - rv = %d; next station: %d\n", rv, val_list[1]);
				}
				break;
			case '0':
				// Power off!
				rv = ioctl(fd, FM_NORMAL2STANDBY, &status);
				printf("[FM_NORMAL2STANDBY] - rv = %d; status = %d\n", rv, status);
				break;
			case '1':
				rv = ioctl(fd, FM_INIT2NORMAL, &status);
				printf("[FM_INIT2NORMAL] - rv = %d; status = %d\n", rv, status);
				break;
			case '?' :
				printf("Unknown option!");
				break;
			default :
				abort();
		}
	}
	sleep(100000); // sleep for a bit to keep the radio 'alive' - kudos TomG :D
	close(fd);
	return 0;
}

