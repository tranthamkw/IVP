/*
   notes and comments 
   useful information
   to follow

*/
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "kenBoard.h"
#include "omegaCN7500.h"
#include "grandvillePhillips.h"
#include "USB1208.h"
#include "BK1696.h"

/*
#define K617 0
#define PROBELASER 2
#define REFLASER 3
#define HOURSINDAY 24
#define MINUTESINHOUR 60
#define BUFSIZE 1024
*/

int main (int argc, char* argv[]){
		char chardata[16];
		char command[16];
		int i;
		float myTemp;
		float volts, amps;
		FILE *fp;



		fp=fopen("/home/pi/MachineStatus.txt","w");

		if (!fp) {
			printf("unable to open file status.dat\n");
			exit(1);
		}


		initializeUSB1208();
		initializeBoard();

/*
		initializeBK1696(0xC0);


		getVoltsAmpsBK1696(0xC0,&volts,&amps);
		printf("LED DRIVE BK  volts %.2f\tamps %.2f\n",volts,amps);
		fprintf(fp,"LED DRIVE BK  volts %.2f\tamps %.2f\n",volts,amps);
*/
		i=getPVCN7500(CN_TARGET,&myTemp);//i holds error information. if anyone cares.
		printf("Motor Temperature(C) = %.1f\n",myTemp);
		fprintf(fp,"Motor Temperature(C) = %.1f\n",myTemp);

		i=getPVCN7500(6,&myTemp);//i holds error information. if anyone cares.
		printf("Retarder Temperature(C) = %.1f\n",myTemp);
		fprintf(fp,"Retarder Temperature(C) = %.1f\n",myTemp);





//		fprintf(fp,"%.2f\t",myTemp);
		printf("error info: %d\n",i);
		getConvectron(0,&myTemp);
		printf("LED Chamber pressure(Torr)= %2.2E\n",myTemp);
		fprintf(fp,"LED Chamber pressure(Torr)= %2.2E\n",myTemp);
//		fprintf(fp,"%2.2E\t",myTemp);

//		getConvectron(1,&myTemp);
//		printf("Ch1 %2.2E\n",myTemp);
//		fprintf(fp,"%2.2E\t",myTemp);

		getIonGauge(&myTemp);
		printf("IonGauge %2.2E\n",myTemp);
		fprintf(fp,"Main Chamber IonGauge (Torr) %2.2E\n",myTemp);
//		fprintf(fp,"%2.2E\t",myTemp);

		getUSB1208AnalogIn(0,&myTemp);
		printf("PMT Photocurrent = Keithly485= %.2f\n",myTemp);//the is no way to read the scale, or order of magnitude. This
		fprintf(fp,"PMT Photocurrent = Keithly485= %.2f\n",myTemp);//the is no way to read the scale, or order of magnitude. This

		getUSB1208AnalogIn(3,&myTemp);
		printf("LED current (mA) %.2f\n",100*myTemp);//the is no way to read the scale, or order of magnitude. This
		fprintf(fp,"LED current (mA) %.2f\n",100*myTemp);//the is no way to read the scale, or order of magnitude. This

		getUSB1208AnalogIn(1,&myTemp);
		printf("PMT HV (V) %.2f\n",1000*myTemp);//the is no way to read the scale, or order of magnitude. This
		fprintf(fp,"PMT HV (V) %.2f\n",1000*myTemp);//the is no way to read the scale, or order of magnitude. This

		fclose(fp);
		closeBoard();
		closeUSB1208();
		return 0 ;

}
