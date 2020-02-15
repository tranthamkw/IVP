/*


 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kenBoard.h"
#include "RS485Devices.h"
#include "myGPIB_Instruments.h"


#define RS485GPIB_BRIDGE_ADDRESS 0xE1
#define KEITHLEY485_GPIB_ADDRESS 0x0A
// = 10. GPIB addresses are usually set with DIP switches on the back of the unit
#define FLUKE8840_GPIB_ADDRESS 0x06


int getReadingK485(float* amps){
	unsigned char chardata[64];
	float tempA=0.0;
	unsigned int i;
	unsigned int len;

	int status = listenGPIBData(chardata, 0x0A, KEITHLEY485_GPIB_ADDRESS, RS485GPIB_BRIDGE_ADDRESS);

	if (status==0) {// no errors
		// format of K485 return string.  See page 3-14 of the manual
		//  NDCA±1.2345E-9<CR><LF>
		// initialize function turns off prefix, so just atof this
//		len = strlen(chardata);
//		for (i=0;i<len;i++) chardata[i]=chardata[i+1];
		tempA = atof(chardata);
//		printf("k485returnstring %s\t",chardata);
//		printf("atof conversion: %f\n",tempA);
		}else{

		status = chardata[0];
		}

	*amps = tempA;

	return status;
}

int getReadingF8840(float* volts){

	unsigned char chardata[64];
	float tempA=0.0;
	unsigned int i;

	int status = listenGPIBData(chardata, 0x0A, FLUKE8840_GPIB_ADDRESS, RS485GPIB_BRIDGE_ADDRESS);

	if (status==0) {// no errors
		// format of F8840 is numeric with suffix turned off.  See page 3-25 of the manual
		tempA=atof(chardata);
//		printf("F8840returnstring %s\t",chardata);
//		printf("atof conversion: %f\n",tempA);

		}else{
		status = chardata[0];
		}
	
	*volts=tempA;
	return status;
}

int initializeF8840(void){
	unsigned char cmdData[16];
	strcpy(cmdData,"F1R4Y0");
// Page 3-7 of manual, FLUKE 8840a
// F1 function VDC
// R4 200V full scale
// Y0 dont send suffix
	int status = sendGPIBData(cmdData,FLUKE8840_GPIB_ADDRESS,RS485GPIB_BRIDGE_ADDRESS);

return status;
}


int initializeK485(void){
	unsigned char cmdData[16];
	strcpy(cmdData,"G1R0X");
// G1 reading without prefix
// set range to 2uA full scale. the rest of the power-on defaults are fine.
// see page 3-10
// R0 is autorange.  I dont know if the machine will 'talk' or return valid data when it is in the process of autoranging.

	int status = sendGPIBData(cmdData,KEITHLEY485_GPIB_ADDRESS,RS485GPIB_BRIDGE_ADDRESS);

return status;

}
