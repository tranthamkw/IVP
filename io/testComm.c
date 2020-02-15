/*

 */

#include <stdio.h>
#include <string.h>
#include "kenBoard.h"
#include "RS485Devices.h"


int main(int argc,char* argv[])
{


	
	char command[64];
	char echoData[128];
	unsigned short i,k;  
	unsigned short temp,chan;
	float volts, stdev;
	unsigned int steps;



// open and configure the serial port
	initializeBoard(); 
	
	
	setAnalogRecorderPeriod(0xD0, 16);
	getAnalogRecorderPeriod(0xD0,&steps);
	printf("ADC data period once every %d mS\n",16*steps);
	
	setServoPosition(0xA0,0,8);
	setServoPosition(0xA0,1,8);


	readRS485AnalogSlave(0xD0,0,5.0,&volts,&stdev);
	printf("Channel 0 volts %f +/- %f\n",volts,stdev);

	readRS485AnalogSlave(0xD0,1,5.0,&volts,&stdev);
	printf("Channel 1 volts %f +/- %f\n",volts,stdev);
	


	setAnalogRecorderPeriod(0xD0, 32);
	getAnalogRecorderPeriod(0xD0,&steps);
	printf("ADC data period once every %d mS\n",16*steps);

/*
	setStepperMotorSpeed(0xB0,50);
	getStepperMotorSpeed(0xB0,&steps);
	printf("Stepper motor speed %d\n",steps);
	moveStepperMotor(0xB0,234,1);

*/
/*
	strcpy(command,"READ?");  //KEITHLEY INSTRUMENTS INC.,MODEL 6485
	printf("Read Keitly Sending %s\t",command);
	i=strlen(command);
	writeRS232Bridge(command,echoData,0xC0);
	printf("Read Keithley Return: %s\n",echoData);
*/
	strcpy(command,"SOUT000");  //B&K 1696 POWER SUPPLY TURN ON OUTPUT
	printf("Sending %s\t",command);
	i=strlen(command);
		writeRS232Bridge(command,echoData,0xC0);
	printf("Return: %s\n",echoData);

	strcpy(command,"VOLT00034"); //B&K 1696 POWER SUPPLY SET VOLTS TO 3.4
	printf("Sending %s\t",command);
	i=strlen(command);
		writeRS232Bridge(command,echoData,0xC0);
	printf("Return: %s\n",echoData);
	
/*
	setStepperMotorSpeed(0xB0,180);
	moveStepperMotor(0xB0,100,0);
*/
	strcpy(command,"GETD00"); //B&K 1696 POWER SUPPLY RETURN ACTUAL VOLTS AND AMPS
	printf("Sending %s\t",command);
	i=strlen(command);
		writeRS232Bridge(command,echoData,0xC0);
	printf("Return: %s\n",echoData);

	setServoPosition(0xA0,0,0);
	setServoPosition(0xA0,1,0);

/*	
	getStepperMotorSpeed(0xB0,&steps);
	printf("Stepper motor speed %d\n",steps);

	getStepperMotorStepsRev(0xB0,&steps);
	printf("Stepper motor steps/rev %d\n",steps);
	
	getStepperMotorSteps(0xB0,&steps);
	printf("steps %d\n",steps);
	if (steps==0) {
	moveStepperMotor(0xB0,50,0);	
	} else printf("motor still in motion\n");
strcpy(command,"CURR:RANG 2e-9");
	printf("Sending %s\n",command);
	i=strlen(command);
	
writeRS232Bridge(command,echoData,0xC0);
	printf("Return: %s\n",echoData);

for (k=0; k<32; k++){
readRS485AnalogSlave(0xD0,0,5.0,&volts,&stdev);
	printf("%d\tChannel 0 volts %f +/- %f\t\t",k,volts,stdev);
	strcpy(command,"READ?");  //KEITHLEY INSTRUMENTS INC.,MODEL 6485
	i=strlen(command);
	writeRS232Bridge(command,echoData,0xC0);
	printf("Keithley: %s\n",echoData);

}
*/

       printf("OK.\n");

// exit normally
    return 0;
}
