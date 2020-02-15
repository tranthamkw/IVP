/*
   Program to record LED excitation function. This is accomplished by incrementing the LED driving voltage and
recording the PMT photo current.

RasPi connected to USB 1208LS.

ammeter analog out connected to USB Analog in Ch 0

LED connected to BK1696 on channel 5
 */

#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <wiringPi.h>
//#include "kenBoard.h"
#include "USB1208.h"
#include "mathTools.h"
//#include "BK1696.h"
#define BUFSIZE 512



int main (int argc, char **argv)
{
	time_t rawtime;
	struct tm * timeinfo;
	FILE *fp;
	char buffer[BUFSIZE],fileName[BUFSIZE],comments[BUFSIZE];

	float chan0,chan1,chan2,chan3,myTemp;

	unsigned int numPoints, delayPoints, i,j;

	// Make sure the correct number of arguments were supplied. If not,
	// prompt the user with the proper form for input. 
	if (argc == 4){
		numPoints = atoi(argv[1]);
		delayPoints = atoi(argv[2]);
		strcpy(comments,argv[3]);
	} else{
	printf("Usage:\n");
	printf("sudo ./yxRecorder <numDatapoints> <delayBtwnPoints> <comments>\n");
	printf("\n Bipolar 2.5V on channels 0 to 3 USB1208\n");
	return 1;
	}

	// set up USB interface
//	initializeBoard();
	initializeUSB1208();

	// get file name.  use format "EX"+$DATE+$TIME+".dat"
	time(&rawtime);
	timeinfo=localtime(&rawtime);

	struct stat st = {0};
	strftime(fileName,80,"/home/pi/Data/%F",timeinfo); //INCLUDE
	if (stat(fileName, &st) == -1){
		mkdir(fileName,S_IRWXU | S_IRWXG | S_IRWXO );
	}

	strftime(fileName,80,"/home/pi/Data/%F/XY%F_%H%M%S",timeinfo);

	sprintf(buffer,"%s.dat",fileName);
	printf("\n%s\n",buffer);

	fp=fopen(buffer,"w");
	if (!fp) {
		printf("Unable to open file: %s\n",buffer);
		exit(1);
	}

	fprintf(fp,"#Filename:\t%s\n",buffer);

	if (numPoints<10){
		printf("Num points too small, using 10\n");
		numPoints=10;
	}
	else if (numPoints > 5000){
		printf("Num points to large, using 5000\n");
		numPoints=5000;
	}
	if (delayPoints<2){
		printf("delay size too small, using 2\n");
		delayPoints=2;
	}
	else if (delayPoints > 500){
		printf("delay to large using, 500\n");
		delayPoints=500;
	}

	fprintf(fp,"#Comments:\t%s\n",comments);


	// Print the header for the information in the datafile
	printf("IDX\tChan0\tChan1\tChan2\tChan3\n");
	fprintf(fp,"IDX\tChan0\tChan1\tChan2\tChan3\n");

	i=0;
	while (i<numPoints){

		// grab several readings and average
		chan0=0.0;
		chan1=0.0;
		chan2=0.0;
		chan3=0.0;
		for (j=0;j<10;j++){
			getUSB1208AnalogIn(0,&myTemp);
			chan0+=myTemp;
			getUSB1208AnalogIn(1,&myTemp);
			chan1+=myTemp;
			getUSB1208AnalogIn(2,&myTemp);
			chan2+=myTemp;
			getUSB1208AnalogIn(3,&myTemp);
			chan3+=myTemp;
			//delay(1);
		}//end j

	printf("%d\t%f\t%f\t%f\t%f\n",i,chan0,chan1,chan2,chan3);
	fprintf(fp,"%d\t%f\t%f\t%f\t%f\n",i,chan0,chan1,chan2,chan3);
	delay(delayPoints);
	i++;
	}// end i

	closeUSB1208();
//	closeBoard();
	fclose(fp);

	return 0;
}

