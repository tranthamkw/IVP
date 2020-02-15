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
#include "kenBoard.h"
#include "USB1208.h"
#include "mathTools.h"
#include "BK1696.h"
#define BUFSIZE 512
#define CHANNEL 5

void graphData(char* fileName);

int main (int argc, char **argv)
{
	time_t rawtime;
	struct tm * timeinfo;
	FILE *fp;
	char buffer[BUFSIZE],fileName[BUFSIZE],comments[BUFSIZE];

	float startVolts,endVolts,deltaVolts,ratio;
	float myVolts,myAmps,myTemp;
	float PMTcurrent,PMTcurrentSTDEV;
	int numSteps,nSamplesPerMeasurement,i,j;

	// Make sure the correct number of arguments were supplied. If not,
	// prompt the user with the proper form for input. 
	if (argc == 5){
		startVolts = atof(argv[1]);
		deltaVolts = atof(argv[2]);
		numSteps = atoi(argv[3]);
		strcpy(comments,argv[4]);
	} else{
	printf("Usage:\n");
	printf("sudo ./excitationfn <(float)startVolts> <(float)stepVolts> <(int)numSteps> <comments>\n");
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

	strftime(fileName,80,"/home/pi/Data/%F/EX%F_%H%M%S",timeinfo);

	sprintf(buffer,"%s.dat",fileName);
	printf("\n%s\n",buffer);

	fp=fopen(buffer,"w");
	if (!fp) {
		printf("Unable to open file: %s\n",buffer);
		exit(1);
	}

	fprintf(fp,"#Filename:\t%s\n",buffer);

	if (numSteps<1){
		printf("Step size too small, using 1\n");
		numSteps=1;
	}
	else if (numSteps > 500){
		printf("Num steps to large using 500\n");
		numSteps=500;
	}

	fprintf(fp,"#Comments:\t%s\n",comments);

	
/*	getIonGauge(&returnFloat);
	printf("IonGauge %2.2E Torr \n",returnFloat);
	fprintf(fp,"#IonGauge(Torr):\t%2.2E\n",returnFloat);

	getConvectron(GP_N2_CHAN,&returnFloat);
	printf("CVGauge(N2) %2.2E Torr\n", returnFloat);
	fprintf(fp,"#CVGauge(N2)(Torr):\t%2.2E\n", returnFloat);

	getConvectron(GP_HE_CHAN,&returnFloat);
	printf("CVGauge(He) %2.2E Torr\n", returnFloat);
	fprintf(fp,"#CVGauge(He)(Torr):\t%2.2E\n", returnFloat);

	getPVCN7500(CN_RESERVE,&returnFloat);
	fprintf(fp,"#CellTemp(Res):\t%f\n",returnFloat);
	getPVCN7500(CN_TARGET,&returnFloat);
	fprintf(fp,"#CellTemp(Targ):\t%f\n",returnFloat);
*/

	// Print the header for the information in the datafile
	printf("VoltsLED\tPMTcurrent\tPMTcurrentStdDev\tLEDcurrent\tLEDStdev\n");
	fprintf(fp,"VoltsLED\tPMTcurrent\tPMTcurrentStdDev\tLEDcurrent\tLEDStdev\n");

	// Allocate some memory to store measurements for calculating
	// error bars.
	nSamplesPerMeasurement = 32;
	double* measurement = malloc(nSamplesPerMeasurement*sizeof(double));
	endVolts = (float)numSteps * deltaVolts + startVolts;
//	deltaVolts=endVolts - startVolts;
	printf("deltavolts %.3f\n",deltaVolts);
	printf("startvolts %.3f\n",startVolts);
	printf("endvolts %.3f\n",endVolts);
delay(100);
	for (i=0;i<numSteps;i++){
		ratio = (float)i;
		myVolts = startVolts + (ratio)*(deltaVolts);
		setVoltsBK1696(CHANNEL,myVolts);
		setOutputBK1696(CHANNEL,BK1696ON);
		delay(100);// allow power supply a chance to change

	j=getVoltsAmpsBK1696(CHANNEL,&myVolts,&myAmps);
//	printf("error info %d\n",i);
	printf("%.3f\t",myVolts);
	fprintf(fp,"%.3f\t",myVolts);

	PMTcurrent = 0.0;
		// grab several readings and average
		for (j=0;j<nSamplesPerMeasurement;j++){
		getUSB1208AnalogIn(0,&myTemp);
		measurement[j]=(float)myTemp;
			PMTcurrent+=measurement[j];
		delay(20);
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
		delay(20);
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

	return 0;
}

