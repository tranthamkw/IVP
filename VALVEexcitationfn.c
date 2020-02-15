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
#include <sys/time.h>
#include <string.h>
#include <wiringPi.h>
#include "kenBoard.h"
#include "USB1208.h"
#include "mathTools.h"
#include "BK1696.h"
#include "grandvillePhillips.h"
#define BUFSIZE 512
#define CHANNEL 5


uint64_t getTimeStamp(void){
	struct timeval tv;
	gettimeofday(&tv,NULL);
	return (((tv.tv_sec*(uint64_t)1000000) + tv.tv_usec)/1000);
}




int main (int argc, char **argv)
{
	time_t rawtime;
	uint64_t startTime;
	struct tm * timeinfo;
	FILE *fp;
	char buffer[BUFSIZE],fileName[BUFSIZE],comments[BUFSIZE];

	float myTemp;
	float PMTcurrent,PMTcurrentSTDEV;
	int numSteps,nSamplesPerMeasurement,i,j;

	// Make sure the correct number of arguments were supplied. If not,
	// prompt the user with the proper form for input. 
	if (argc == 3){
		//startVolts = atof(argv[1]);
		//deltaVolts = atof(argv[2]);
		numSteps = atoi(argv[1]);
		strcpy(comments,argv[2]);
	} else{
	printf("Usage:\n");
	printf("sudo ./excitationfn <num measurements> <comments>\n");
	return 1;
	}

	// set up USB interface
	initializeBoard();
	initializeUSB1208();

	// get file name.  use format "EX"+$DATE+$TIME+".dat"
	time(&rawtime);
	timeinfo=localtime(&rawtime);

	struct stat st = {0};
	strftime(fileName,80,"/home/pi/Data/%F",timeinfo); //INCLUDE
	if (stat(fileName, &st) == -1){
		mkdir(fileName,S_IRWXU | S_IRWXG | S_IRWXO );
	}

	strftime(fileName,80,"/home/pi/Data/%F/VACx%F_%H%M%S",timeinfo);

	sprintf(buffer,"%s.dat",fileName);
	printf("\n%s\n",buffer);

	fp=fopen(buffer,"w");
	if (!fp) {
		printf("Unable to open file: %s\n",buffer);
		exit(1);
	}

	fprintf(fp,"#Filename:\t%s\n",buffer);

	fprintf(fp,"#Comments:\t%s\n",comments);

	
/*


	getConvectron(GP_HE_CHAN,&returnFloat);
	printf("CVGauge(He) %2.2E Torr\n", returnFloat);
	fprintf(fp,"#CVGauge(He)(Torr):\t%2.2E\n", returnFloat);

	getPVCN7500(CN_RESERVE,&returnFloat);
	fprintf(fp,"#CellTemp(Res):\t%f\n",returnFloat);
	getPVCN7500(CN_TARGET,&returnFloat);
	fprintf(fp,"#CellTemp(Targ):\t%f\n",returnFloat);
*/

	// Print the header for the information in the datafile
	printf("idx\tIG Pressure\tCV Pressure\tPMTcurrent\tPMTcurrentStdDev\tLEDcurrent\tLEDStdev\n");
	fprintf(fp,"time mS\tIG Pressure\tCV Pressure\tPMTcurrent\tPMTcurrentStdDev\tLEDcurrent\tLEDStdev\n");

	// Allocate some memory to store measurements for calculating
	// error bars.
	nSamplesPerMeasurement = 8;
	double* measurement = malloc(nSamplesPerMeasurement*sizeof(double));

	delay(10);

	startTime=getTimeStamp();

	for (i=0;i<numSteps;i++){
		if (i==50) {
			setValveBoard(0,0);//closes both
			setValveBoard(1,0);
			setValveBoard(0,1);//open vac
		}
		fprintf(fp,"%lu\t",(unsigned long int)(getTimeStamp()-startTime));
		printf("%d\t",i);
		getIonGauge(&myTemp);
		printf("%g\t",myTemp);
		fprintf(fp,"%g\t",myTemp);

		getConvectron(0,&myTemp);
		printf("%g\t", myTemp);
		fprintf(fp,"%g\t", myTemp);

		PMTcurrent = 0.0;
		// grab several readings and average
		for (j=0;j<nSamplesPerMeasurement;j++){
			getUSB1208AnalogIn(0,&myTemp);
			measurement[j]=(float)myTemp;
			PMTcurrent+=measurement[j];
//			delay(1);
		}//end j

		PMTcurrent=PMTcurrent/(float)nSamplesPerMeasurement;
		PMTcurrentSTDEV=stdDeviation(measurement,nSamplesPerMeasurement);

		printf("%f\t",PMTcurrent);
		fprintf(fp,"%f\t",PMTcurrent);
		printf("%f\t",PMTcurrentSTDEV);
		fprintf(fp,"%f\t",PMTcurrentSTDEV);
	
		PMTcurrent = 0.0;
		// grab several readings and average
		for (j=0;j<nSamplesPerMeasurement;j++){
			getUSB1208AnalogIn(3,&myTemp);
			measurement[j]=100.0*(float)myTemp;
			PMTcurrent+=measurement[j];
//			delay(1);
		}//end j

		PMTcurrent=PMTcurrent/(float)nSamplesPerMeasurement;
		PMTcurrentSTDEV=stdDeviation(measurement,nSamplesPerMeasurement);

		printf("%f\t",PMTcurrent);
		fprintf(fp,"%f\t",PMTcurrent);
		printf("%f\n",PMTcurrentSTDEV);
		fprintf(fp,"%f\n",PMTcurrentSTDEV);
	}// end i

	closeUSB1208();
	closeBoard();
	free(measurement);
	fclose(fp);

			setValveBoard(0,0);//closes both
			setValveBoard(1,0);
			setValveBoard(1,1);//open atmo
	return 0;
}

