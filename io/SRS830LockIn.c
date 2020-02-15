/*
 */

#include <stdio.h>
#include <string.h>
#include "kenBoard.h"
#include "RS485Devices.h"

#define CHANNEL 0xC4




void 
	unsigned char command[64];
	unsigned char echoData[128];
	strcpy((char*)command,"*IDN?");

	writeRS485to232Bridge(command,echoData,CHANNEL);
	printf("Return: %s\n",echoData);


}
