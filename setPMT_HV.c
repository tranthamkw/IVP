/*
 * This is a template file to show all of the necessary components to add
 * to new data collection files that are created. 
*/
#include "USB1208.h"
#include <wiringPi.h>


int main (int argc, char* argv[]){

//signed short svalue;
int chan;
float value;

chan = 1;
	if (argc==2) {
		value=atof(argv[1]);
		//chan=atoi(argv[2]);
	} else {
		printf("Usage:\n$ sudo ./setPMT_HV  <float volts> \n");
		return 0;
	}
if ((value<1500)&(chan<2)) {

	value = 0.9135*value + 1.44;
	initializeUSB1208();
	setUSB1208AnalogOut(chan,(int) value);
//	printf("Setting HV...");
//	for (i=0;i<6;i++){
//	printf("%d...",i);
//	fflush(stdout);
//	delay(500);
//	}
//	getUSB1208AnalogIn(1,&value);
//	printf("HV=%.2f\n",(1000*value));

	closeUSB1208();


} else {
printf("error: volts >1500 or chan >1 \n");
}



	return 0 ;
}
