/*
   notes and comments 
   useful information
   to follow

*/
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "kenBoard.h"
#include "BK1696.h"


int main (int argc, char* argv[]){

	float volts, amps;
	int chan;

if (argc==2) {
		chan=atoi(argv[1]);
	} else {
		printf("Usage:\n$ sudo ./readBK <RS485CHANNEL>\n");
		return 0;
	}

		initializeBoard();
		initializeBK1696(chan);
		getVoltsAmpsBK1696(chan,&volts,&amps);
		printf("BK  volts %.2f\tamps %.2f\n",volts,amps);

		return 0 ;

}
