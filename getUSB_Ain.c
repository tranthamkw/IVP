/*
 * This is a template file to show all of the necessary components to add
 * to new data collection files that are created. 
*/
#include "USB1208.h"

int main (int argc, char* argv[]){

//signed short svalue;
int chan;
float myTemp;


chan = 0;
/*	if (argc==2) {
		chan=atoi(argv[1]);
	} else {
		printf("Usage:\n$ sudo ./<programName> chan ...\n");
		return 0;
	}

*/


	initializeUSB1208();
for (chan=0;chan<4;chan++){
	getUSB1208AnalogIn(chan,&myTemp);
	printf("Channel %d:\t %.3f Volts\n\n",chan,myTemp);
}
	closeUSB1208();


	return 0 ;
}
