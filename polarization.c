/*

program to recorde pmt current as a function of QWP angle.

records raw data to file
records/appends to a master record file

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
//#include "BK1696.h"
#include "grandvillePhillips.h"
#include "omegaCN7500.h"

#define BUFSIZE 512
#define CHANNEL 192
#define MOTOR 1
#define DIR 1
#define STEPSPERREV 400.0
#define PI 3.14159265


double fitFunction(double c0_,double c1_,double s1_,double c2_,double s2_,double c4_,double s4_,double angle_);


int main (int argc, char **argv)
{
	time_t rawtime;
	struct tm * timeinfo;
	FILE *fp,*fp2;
	char buffer[BUFSIZE],fileName[BUFSIZE],comments[BUFSIZE];
//,command[16],chardata[16];
	char sumFileName[BUFSIZE];
	char fileName2[BUFSIZE];
	char buffer2[BUFSIZE];
	float myTemp;
//	float myVolts,myAmps;
//	double PMTcurrent;
	double PMTcurrentSTDEV,avgPMTcurrent,LEDcurrent,PMTvoltage;
	double c0,c2,c4,s2,s4,c1,s1;
	double dc0,dc2,ds2,dc4,ds4,dc1,ds1;
	double mag2,mag4,angle2,angle4;
	double SStot,SSres,Rsquared;
	double angle;
	int nSamplesPerMeasurement,i,j,k,printHeader;
	int totalSteps, incStep,numDataPoints, delayTime;
	float x1,x2,x3,x4;  // for later expansion. these will be written to summary file.  things like pressure or temperature perhaps.

	// Make sure the correct number of arguments were supplied. If not,
	// prompt the user with the proper form for input. 
	delayTime = 150;
	if (argc == 6){
		totalSteps = atoi(argv[1]);
		incStep = atoi(argv[2]);
		delayTime = atoi(argv[3]);
		strcpy(comments,argv[4]);
		strcpy(sumFileName,argv[5]);
	} else{
	printf("Usage:\n");
	printf("sudo ./polarization <(int)totalSteps> <(int)stepIncrement> <(int) delayTime ms> <comments> <SummaryFileName.dat>\n");
	printf("must have even number of data points in one revolution.\n");

	return 1;
	}

	numDataPoints = totalSteps/incStep;

	// check to see if numDataPoints is even
	printf("numDataPoints %d\n",numDataPoints);
	if (numDataPoints%2!=0)
		{
		// it oddd
		printf("must have even number of data points. Is totalSteps/stepIncrement even?\n");
		return 1;
		}

	double* PMTcurrent = malloc(numDataPoints*sizeof(double));

	//initialze boards
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

	strftime(fileName,80,"/home/pi/Data/%F/POL%F_%H%M%S",timeinfo);  // this is the raw data file
	sprintf(buffer,"%s.dat",fileName);

	printf("\n%s\n",buffer);

	fp=fopen(buffer,"w");
	if (!fp) {
		printf("Unable to open file: %s\n",buffer);
		exit(1);
	}
	fprintf(fp,"#Filename:\t%s\n",buffer);

	strftime(fileName2,80,"/home/pi/Data/%F/",timeinfo); //INCLUDE

	buffer2[0]='\0';
	strcpy(buffer2,fileName2);

	strcat(buffer2,sumFileName);
	fp2=fopen(buffer2,"r");
	if (!fp2) {
		// file does not exist
		printf("file does not exist. print header\n");
		printHeader = 1;
//		fclose(fp2);
	} else {
		printf("file does exist. do not print header\n");
		printHeader = 0;
		fclose(fp2);
	}

	fp2=fopen(buffer2,"a");
	if (!fp2) {
		printf("Unable to open file: %s\n",buffer);
		exit(1);
	}

	printf("#SummaryFilename:\t%s\n",buffer2);

	fprintf(fp,"#Comments:\t%s\n",comments);

//	command[0]=13;
//	command[1]='\0';
//	write_rs485ASCII(command,chardata);
	delay(100);
//	initializeBK1696(CHANNEL);
	delay(10);
	//setVoltsBK1696(CHANNEL,setVolts);
	//delay(20);
	//setOutputBK1696(CHANNEL,BK1696ON);
	//delay(500);// allow power supply a chance to change


	delay(200);
//	getVoltsAmpsBK1696(CHANNEL,&x1,&myTemp);
x1=9.999;
	printf("BK volts %.2f\n",x1);
	fprintf(fp,"BK volts %.2f\n",x1);

	getIonGauge(&x4);
	printf("IonGauge %2.2E Torr \n",x4);
	fprintf(fp,"#IonGauge(Torr):\t%2.2E\n",x4);


	getConvectron(0,&x2);
	printf("CVGauge(0) %2.2E Torr\n", x2);
	fprintf(fp,"CVGauge(0)(Torr):\t%2.2E\n", x2);

	getPVCN7500(CN_TARGET,&x3);
	printf("MotorTemperature:\t%.2f\n",x3);
	fprintf(fp,"MotorTemperature:\t.2%f\n",x3);




	// Allocate some memory to store measurements for calculating
	// error bars.
	nSamplesPerMeasurement = 16;
	double* measurement = malloc(nSamplesPerMeasurement*sizeof(double));
	c1=0.0;
	s1=0.0;
	c0=0.0;
	c2=0.0;
	c4=0.0;
	s2=0.0;
	s4=0.0;
	dc0=0.0;
	dc1=0.0;
	ds1=0.0;
	dc2=0.0;
	dc4=0.0;
	ds2=0.0;
	ds4=0.0;
	k=0;

	LEDcurrent = 0.0;
			// grab several readings and average
			for (j=0;j<nSamplesPerMeasurement;j++){
				getUSB1208AnalogIn(3,&myTemp);
				measurement[j]=1000.0*(double)myTemp;
				LEDcurrent+=measurement[j];
				delay(15);
			}//end j

		LEDcurrent=LEDcurrent/(double)nSamplesPerMeasurement;

	printf("NEON Volts: \t%.2f V\n",LEDcurrent);
	fprintf(fp,"NEON volts: \t%.2f V\n",LEDcurrent);

	PMTvoltage = 0.0;
			// grab several readings and average
			for (j=0;j<nSamplesPerMeasurement;j++){
				getUSB1208AnalogIn(1,&myTemp);
				measurement[j]=1000.0*(double)myTemp;
				PMTvoltage+=measurement[j];
				delay(15);
			}//end j

		PMTvoltage=PMTvoltage/(double)nSamplesPerMeasurement;

	printf("PMT voltage: \t%.2f V\n",PMTvoltage);
	fprintf(fp,"PMT voltage: \t%.2f V\n",PMTvoltage);

	// Print the header for the information in the datafile
	printf("Step\tPMTcurrent\tPMTcurrentStdDev\n");
	fprintf(fp,"Step\tPMTcurrent\tPMTcurrentStdDev\n");

	for (i=0;i<totalSteps;i+=incStep){
		angle = (2*PI*(double)i)/STEPSPERREV;
		printf("%d\t",i);
		fprintf(fp,"%d\t",i);
		PMTcurrent[k] = 0.0;
			// grab several readings and average
			for (j=0;j<nSamplesPerMeasurement;j++){
				getUSB1208AnalogIn(0,&myTemp);
				measurement[j]=(double)myTemp;
				PMTcurrent[k]+=measurement[j];
				delay_uS(10);
			}//end j

		PMTcurrent[k]=PMTcurrent[k]/(double)nSamplesPerMeasurement;
		PMTcurrentSTDEV=stdDeviation(measurement,nSamplesPerMeasurement);

//		PMTcurrent[k]=1.5 + 1.5 * cos(4.0*(angle + 2.2));
		c0+=PMTcurrent[k];
		c1+=PMTcurrent[k] * cos(angle);
		s1+=PMTcurrent[k] * sin(angle);
		c2+=PMTcurrent[k] * cos(2.0*angle);
		s2+=PMTcurrent[k] * sin(2.0*angle);
		c4+=PMTcurrent[k] * cos(4.0*angle);
		s4+=PMTcurrent[k] * sin(4.0*angle);

		dc0+=pow(PMTcurrentSTDEV,2);
		dc1+=pow(PMTcurrentSTDEV * cos(angle),2);
		ds1+=pow(PMTcurrentSTDEV * sin(angle),2);
		dc2+=pow(PMTcurrentSTDEV * cos(2.0*angle),2);
		ds2+=pow(PMTcurrentSTDEV * sin(2.0*angle),2);
		dc4+=pow(PMTcurrentSTDEV * cos(4.0*angle),2);
		ds4+=pow(PMTcurrentSTDEV * sin(4.0*angle),2);

		stepMotor(MOTOR,DIR,incStep);
// this is the delay between steps.

		delay_uS(1000*delayTime);

		printf("%f\t",PMTcurrent[k]);
		fprintf(fp,"%f\t",PMTcurrent[k]);
		printf("%f\n",PMTcurrentSTDEV);
		fprintf(fp,"%f\n",PMTcurrentSTDEV);
	k++;
	}// end i


	c0 = c0/(double)numDataPoints;
	c1 = 2.0*c1/(double)numDataPoints;
	s1 = 2.0*s1/(double)numDataPoints;
	c2 = 2.0*c2/(double)numDataPoints;
	s2 = 2.0*s2/(double)numDataPoints;
	c4 = 2.0*c4/(double)numDataPoints;
	s4 = 2.0*s4/(double)numDataPoints;

	dc0=sqrt(dc0)/(double)numDataPoints;
	dc1=2.0*sqrt(dc1)/(double)numDataPoints;
	ds1=2.0*sqrt(ds1)/(double)numDataPoints;
	dc2=2.0*sqrt(dc2)/(double)numDataPoints;
	ds2=2.0*sqrt(ds2)/(double)numDataPoints;
	dc4=2.0*sqrt(dc4)/(double)numDataPoints;
	ds4=2.0*sqrt(ds4)/(double)numDataPoints;

	mag2 = sqrt(pow(c2,2)+pow(s2,2));
	mag4 = sqrt(pow(c4,2)+pow(s4,2));
	angle2 = atan2(s2,c2);
	angle4 = atan2(s4,c4);

	avgPMTcurrent = 0.0;
	for (k=0;k<numDataPoints;k++){
		avgPMTcurrent+=PMTcurrent[k];
	}
	avgPMTcurrent = avgPMTcurrent/(double)numDataPoints;

	SStot = 0.0;
	for (k=0;k<numDataPoints;k++){
		SStot+=pow((avgPMTcurrent-PMTcurrent[k]),2);
	}

	SSres = 0.0;
	k=0;
	for (i=0;i<totalSteps;i+=incStep){
		angle = (2*PI*(double)i)/STEPSPERREV;
		SSres+=pow((PMTcurrent[k]-fitFunction(c0,c1,s1,c2,s2,c4,s4,angle)),2);
		k++;
	}

	Rsquared = ((double) 1.0 - (SSres/SStot));


if (printHeader) {
//print header
	fprintf(fp2,"filename\tcomments\tStepSize\tDelayTime\tvolts\tCV-tron\tIonGauge\tTemperature\tNEON HV\tPMT HV\t");
	fprintf(fp2,"c0\tdc0\tc1\tdc1\ts1\tds1\tc2\tdc2\ts2\tds2\tc4\tdc4\ts4\tds4\t");
	fprintf(fp2,"mag2\tangle2\tmag4\tangle4\t");
	fprintf(fp2,"SSres\t R ²\n");
}


printf("volts\tCVtron\t\tIonG\t\tTemp\tmag2\tangle2\tmag4\tangle4\n");
printf("%.3f\t%.2E\t%.2E\t%.3f\t%.3g\t%.3g\t%.3g\t%.3g\n",x1,x2,x4,x3,mag2,angle2,mag4,angle4);


fprintf(fp2,"%s\t%s\t%d\t%d\t%f\t%.2E\t%.2E\t%f\t%f\t%f\t",buffer,comments,incStep,delayTime,x1,x2,x4,x3,LEDcurrent,PMTvoltage);

fprintf(fp2,"%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t",c0,c1,dc1,s1,ds1,dc0,c2,dc2,s2,ds2,c4,dc4,s4,ds4);

fprintf(fp2,"%f\t%f\t%f\t%f\t",mag2,angle2,mag4,angle4);

fprintf(fp2,"%f\t%f\n",SSres,Rsquared);



	printf("c0\tc2\ts2\tc4\ts4\n");
	printf("%.3g\t%.3g\t%.3g\t%.3g\t%.3g\n",c0,c2,s2,c4,s4);

//	printf("dc0\tdc2\tds2\tdc4\tds4\n");
//	printf("%.3g\t%.3g\t%.3g\t%.3g\t%.3g\n",dc0,dc2,ds2,dc4,ds4);

//	printf("\navg pmt current %f\n",avgPMTcurrent);
	printf("SStot %f\n",SStot);
	printf("SSres %f\n",SSres);
	printf("R² = %f\n",Rsquared);

//	fprintf(fp,"#Filename:\t%s\n",buffer);
//	fprintf(fp,"#Comments:\t%s\n",comments);


	closeUSB1208();
	closeBoard();
	free(measurement);
	fclose(fp);
	fclose(fp2);
	return 0;
}


double fitFunction(double c0_,double c1_,double s1_,double c2_,double s2_,double c4_,double s4_,double angle_){

	double tempD;
	tempD = c0_ + c1_ * cos(angle_) + s1_ * sin(angle_) + c2_ * cos(2*angle_) + s2_ * sin(2*angle_) + c4_ * cos(4*angle_) + s4_ * sin(4*angle_);
	return tempD;

}
