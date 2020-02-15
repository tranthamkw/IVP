/*
 */

#include <stdio.h>
#include <string.h>
#include "kenBoard.h"
#include "RS485Devices.h"



int main(int argc,char* argv[])
{


	unsigned char command[64],returnData[64];
	unsigned short chan,gpib;
	int status;


// open and configure the serial port
	initializeBoard();


if (argc==4){

	strcpy((char*)command,argv[1]);
	chan=(unsigned short)strtol(argv[3],NULL,16);
	gpib=(unsigned short)strtol(argv[2],NULL,16);


	resetGPIBBridge(chan);
	printf("Sending IFC\n");
	delay(500);

	printf("RS485 Addres %02x = %d\n",chan,chan);
	printf("Sending %s\t",command);
	status = sendGPIBData(command,gpib,chan);
	printf("Status %d\n",status);

}else{
	printf("Program for manually sending machine specific data to a specific GPIB listener\n\n");
	printf("usage: sudo ./writegpibBridge \"command to send\" <GPIBaddress> <RS485AddressHEX>\n");
	printf("\ne.g.\n");
	printf("For a Keithley485 piccoameter, gpib address 10, attached to a bridge with address 0xE1:\n");
	printf("sudo ./writegpibBridge \"R4X\" 0A E1\n");
	printf("will set the scale to 20uA FS\n");
}

// exit normally
    return 0;
}

