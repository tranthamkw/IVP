/*
closes atmo valve
opens vacuum valuve until pressure is less than some value.


*/

#include <stdio.h>
#include "kenBoard.h"
#include "grandvillePhillips.h"

void controlValve (int control);

int main (int argc, char *argv[]){

	float targetPressure;
	float currentPressure,lasterror;
	float error,integral,delta;
	int i,ctrl, dlay, lastctrl;

dlay=60;
	if (argc==3){
		targetPressure = atof(argv[1]); // get steps from command line
		dlay = atoi(argv[2]);
	} else {
	if (argc==2){
		targetPressure = atof(argv[1]);
	} else {
	printf("Ussage:  ~$sudo ./setVac <floatSetPressureinTorr>\n OR \n");
	printf("Ussage:  ~$sudo ./setVac <floatSetPressureinTorr> <int delay time>\n");

	return -1;
	}
	}

	initializeBoard();

	printf("closing both valves\n");
	setValveBoard(0,0);
	setValveBoard(1,0);

	getConvectron(0,&currentPressure);
	printf("Current P = %f\n",currentPressure);

	if (targetPressure<5.0) {
		printf("Open vacuum valve and wait %d s\n",dlay);
		setValveBoard(0,1); //just open vacuum valve and be done
		for (i=0;i<dlay;i++) {  // wait time to recheck the pressure
			delay(1000);
			getConvectron(0,&currentPressure);
			printf("P=%f ",currentPressure);
			printf(" %d ",i);
			fflush(stdout);
		}

	} else {


	error =(currentPressure - targetPressure)/currentPressure;
	lasterror = error;
	lastctrl=0;
	ctrl=0;
	integral=0;
	delta=0;
	i=0;
	printf("idx\tP\terr\tint\tdel\tctrl\n");
	while (error > 0.015){
		ctrl=((int)(100.0*error + 0.3*integral + 500.0*delta));
		ctrl=(ctrl+lastctrl)/2;
		lastctrl=ctrl;
		if (i%10==0) printf("%d\t%f\t%f\t%f\t%f\t%d\n",i,currentPressure,error,integral,delta,ctrl);
		fflush(stdout);
		controlValve(ctrl);
		getConvectron(0,&currentPressure);
		error =(currentPressure - targetPressure)/currentPressure;
		delta = ((error-lasterror) + 2*delta)/3;
		lasterror=error;
		integral+=(error+lasterror)/2;
	//	if (i%20) integral++;
	i++;
	}
	setValveBoard(0,0); //close vacuum valve
	printf("%d\t%f\t%f\t%f\t%f\t%d\n",i,currentPressure,error,integral,delta,ctrl);
	printf("\n");
	}

	closeBoard();
//	closeUSB1208();

	return 0;
}

void controlValve (int control){

if (control>100){
	control = 100;
	setValveBoard(0,1);
	delay(control);
	} else {
	if (control<20) control = 20;
	setValveBoard(0,1);
	delay(control);
	setValveBoard(0,0);
	delay(100-control);
}
}
