/*
 */

#include <stdio.h>
#include <string.h>
#include "kenBoard.h"
#include "myGPIB_Instruments.h"

#define RS485GPIB_BRIDGE 0xE1


int main(int argc,char* argv[]){

int i,numDataPoints;
float myVolts,myAmps;

// open and configure the serial port


if (argc==2){
	initializeBoard();
	
	resetGPIBBridge(RS485GPIB_BRIDGE);

	delay(100);
	numDataPoints=atoi(argv[1]);
	printf("initialize Fluke8840\n");
	initializeF8840();

	delay(500);
	printf("initialize Keithly\n");
	initializeK485();

	delay(500);

	for (i=0;i<numDataPoints;i++){
	getReadingK485(&myAmps);
	delay(200);
	getReadingF8840(&myVolts);
	delay(200);
	printf("Volts= %e\t\tAmps= %e\n",myVolts,myAmps);
	}

	delay(200);
	printf("close bridge\n");
	resetGPIBBridge(RS485GPIB_BRIDGE);

	closeGPIBBridge(RS485GPIB_BRIDGE);
}

// exit normally
    return 0;
}

