/*
 * This is a template file to show all of the necessary components to add
 * to new data collection files that are created. 
*/
#include "USB1208.h"

int main (int argc, char* argv[]){

//signed short svalue;
int chan;
int value;

chan = 0;
	if (argc==3) {
		chan=atoi(argv[1]);
		value=atoi(argv[2]);
	} else {
		printf("Usage:\n$ sudo ./setUSB_Aout <chan 0,1>  <value 0 -1024>\n");
		return 0;
	}


	initializeUSB1208();


	setUSB1208AnalogOut(chan,value);


	closeUSB1208();
	return 0 ;
}
