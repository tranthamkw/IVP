/*

Uses the RasPi GPIO ports to bit-band a stepper motor driver.  

*/

#include <stdio.h>
#include "kenBoard.h"
#include "RS485Devices.h"

int main (int argc, char *argv[]){

	unsigned short on,value;
	int i,dtime;

	if (argc==3){
		on = atoi(argv[1]); // get steps from command line
		dtime = atoi(argv[2]);
	} else {

	printf("Ussage:  ~$sudo ./setVac <(0,1)> <delayTime int sec>\n");

	return -1;
	}

	initializeBoard();
//	initializeUSB1208();
// all off
	printf("closing both valves\n");

//	setValveBoard(0,0);
//	setValveBoard(1,0);

	setRS485DigitalOut(0xA3,3);
	delay(500);

	printf("setting vacuum valve %d\n",on);
//	setValveBoard(0,on);
	value = (on & 0x01)<<1 | !on;
	setRS485DigitalOut(0xA3,value);
	delay(500);
	printf("setting atmo valve %d\n",!on);
//	setValveBoard(1,!on);
	setRS485DigitalOut(0xA3,value);
	delay(500);

	printf("waiting..");

	for (i=0;i<dtime;i++) {
		delay(1000);
		printf("%d ",i);
		fflush(stdout);
	}

	printf("\n");

	closeBoard();
//	closeUSB1208();

	return 0;
}

