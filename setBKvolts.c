/*
   notes and comments 
   useful information
   to follow

*/
#include <stdlib.h>
#include <stdio.h>
#include "kenBoard.h"
#include "BK1696.h"



int main (int argc, char* argv[]){

	int i,j;
	float myVolts,myAmps;
	int chan;

	initializeBoard();

if (argc==3){
	chan=atoi(argv[1]);
	i=initializeBK1696(chan);

	i=getVoltsAmpsBK1696(chan,&myVolts,&myAmps);

	printf("Volts %.2f\tAmps %.2f\n",myVolts,myAmps);

	myVolts=atof(argv[2]);
	j=0;
	do {
	i=setVoltsBK1696(chan,myVolts);
	j++;
		delay(100);
		if (i<0) printf("retry set volts...%d\n",j);
		fflush(stdout);
	} while ((j < 8) & (i < 0));

	setOutputBK1696(chan,BK1696ON);

	delay(100);// allow power supply a chance to change

	i=getVoltsAmpsBK1696(chan,&myVolts,&myAmps);
	if (i==0) printf("Volts %.2f\tAmps %.2f\n",myVolts,myAmps);

}else{
printf("usage: ./setBKvolts <RS485_channel> <floatingpointvalue>\n");
}
	closeBoard();

	return 0 ;
}
