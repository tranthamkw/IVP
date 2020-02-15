/*
 * This is a template file to show all of the necessary components to add
 * to new data collection files that are created. 
*/
#include "kenBoard.h"

int main (int argc, char* argv[]){

//signed short svalue;
unsigned int chan, rData;



chan = 0;
/*	if (argc==2) {
		chan=atoi(argv[1]);
	} else {
		printf("Usage:\n$ sudo ./<programName> chan ...\n");
		return 0;
	}

*/


	initializeBoard();
for (chan=0;chan<8;chan++){

	printf("Ch%d\t",chan);
}
printf("\n");

for (chan=0;chan<8;chan++){

	getADC(chan,&rData);

	printf("%d\t",rData);
}
printf("\n");

	closeBoard();


	return 0 ;
}
