/*
 */

#include <stdio.h>
#include <string.h>
#include "kenBoard.h"
#include "RS485Devices.h"



int main(int argc,char* argv[])
{


	unsigned char command[64],returnData[64];
	unsigned short chan,gpib,terminator;
	int status;

// open and configure the serial port
	initializeBoard();


if (argc==4){

//	strcpy((char*)command,argv[1]);
	gpib=(unsigned short)strtol(argv[2],NULL,16);
	chan=(unsigned short)strtol(argv[3],NULL,16);
	terminator=(unsigned short) strtol(argv[1],NULL,16);

//	resetGPIBBridge(chan);
//	printf("Sending IFC\n");
//	delay(500);

	printf("RS485 Address %02x = %d\n",chan,chan);
	printf("GPIB Address %02x = %d\n",gpib,gpib);

	status=listenGPIBData(returnData,terminator,gpib,chan);
	printf("Status %d\n",status);

	printf("Return data: %s\n",returnData);

}else{
	printf("usage: sudo ./readgpibBridge <terminatorCharToListenforHEX> <GPIBaddressHEX>  <RS485AddressHEX>\n");
}

// exit normally
    return 0;
}

