/*

uses the RasPi GPIO ports to bit-band a stepper motor driver.  

uses two ports (defined below) to set the direction and number of stepts.
usage:
~$ sudo ./stepmotor 100 1
steps the motor 100 steps in direction 1.
~$ sudo ./stepmotor 500 0
steps the motor 500 steps in direction 0.

compile
~$  gcc -o stepmotor stepmotor.c -l wiringPi

*/

#include <stdio.h>
#include <sys/stat.h>
#include <time.h>
#include "kenBoard.h"

int main (int argc, char *argv[]){
	unsigned short motor;
	unsigned int numSteps;
	char sumFileName[128],fileName2[128],buffer2[128];
	time_t rawtime;
	struct tm * timeinfo;
	FILE *fp2;

	switch (argc){
	case 2: 
		motor = (unsigned short) atoi(argv[1]); // which steper motor
		break;
	case 3:
		motor = (unsigned short) atoi(argv[1]); // which steper motor
		strcpy(sumFileName,argv[2]);
		// get file name.  use format "EX"+$DATE+$TIME+".dat"
		time(&rawtime);
		timeinfo=localtime(&rawtime);
		struct stat st = {0};
		strftime(fileName2,80,"/home/pi/Data/%F",timeinfo); //INCLUDE
		if (stat(fileName2, &st) == -1){
			mkdir(fileName2,S_IRWXU | S_IRWXG | S_IRWXO );
		}
		strftime(fileName2,80,"/home/pi/Data/%F/",timeinfo); //INCLUDE
		buffer2[0]='\0';
		strcpy(buffer2,fileName2);
		strcat(buffer2,sumFileName);
		fp2=fopen(buffer2,"a");
		if (!fp2) {
			printf("Unable to open file: %s\n",buffer2);
			exit(1);
		}
		break;
	default:
		printf("Usage:  ~$sudo ./homemotor <motor(0,1,2)> \n");
		motor = 3;// not part of the switch statment, so nuthing happens
		return -1;
	}//




 
	initializeBoard();
//	initializeUSB1208();

	numSteps=homeMotor(motor);
	closeBoard();
	if (argc==3) fprintf(fp2,"Home num steps\t%d\n",numSteps);
//	closeUSB1208();
	if (argc==3) fclose(fp2);

	return 0;
}

