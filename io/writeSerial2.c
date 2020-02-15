/*


 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wiringPi.h>
#include <wiringSerial.h>

void write_rs485ASCII(char* cmd, char* pszEcho);
int initialize_rs485(int baud, int pin);


int main (int argc, char* argv[]){

char chardata[64];
char command[64];
int j;

	initializeBoard();
	initialize_rs485(9600,25);
	//9600 is the default for most equipment
	//wiring pi gpio #25 controls write/listen

if (argc==2){
	strcpy(command,argv[1]);
	j = strlen(command);
// append a charage return.  This is for the BK 1696.  other devices may not want CR
	command[j]=13;
	command[j+1]='\0';
	write_rs485ASCII(command, chardata);
	printf("\n\n%s\n\n\n",command);
	printf("%s \n",chardata);

}
	closeBoard();
  return 0 ;
}


int initialize_rs485(int baud,int pin){
//	if (wiringPiSetup () == -1) 
//	  {
//	    fprintf (stdout, "Unable to start wiringPi: %s\n", strerror (errno)) ;
//	    return 1 ;
//	  }
	bd = baud;
	wp=pin;
  if ((fd = serialOpen ("/dev/ttyAMA0", bd)) < 0)
  {
    fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
    return 1 ;
  }
//printf("initialize 485 fd= %d\n",fd);
	pinMode(wp,OUTPUT);
	digitalWrite(wp,HIGH);
	return 0;
}//end initialize
void write_rs485ASCII(char* cmd, char* pszEcho){
	/* routine to write  ASCII data to the rs485 serial port. If a device
	requires a CR or LF in the instruction, it must be in the cmd array. it is not
	apended in this function. Because different devices have difffernt expectations*/

	int length,j,i;
	int a;
	char buffer[64];

	length=strlen(cmd);

	 /* get the string length.  Since this is a null-terminated c string, '0' is assumed to be the
	last element in the array*/
//	printf("command  %s",cmd);

	digitalWrite(wp,LOW);// sets the control signal WRITE to the RS 485 buss
//	delay(1);
	delayMicrosecondsHard(100);// minor wait to allow signals to settle
printf("command %s\n",cmd);
printf("Sent string ASCII chars \n");
//	serialPuts (fd, cmd);

	for (j=0;j<length;j++){
	printf("%d  ",cmd[j]);
		serialPutchar(fd,cmd[j]);
	}

printf("\nReturn string ASCII chars\n");
	// cannot read in from UART until done transmitting, so wait . . . 
	delayMicrosecondsHard(calcDelay(length));

	digitalWrite(wp,HIGH);// now set control to READ (i.e. LISTEN)

	delay(200); // wait some more so that the external device has time to transmitt.  Data fills the UART buffer.
	// this could be calculated based on what is expected.  usually, a delay of 30 works fine for 4 to 8 chars returned. 
	// it doesnt hurt if data sets in the UART buffer until it is read.
printf("command %s\n",cmd);

	i=0;
	    while (serialDataAvail (fd))
	    {
		a = serialGetchar(fd);
		//if (a>31){ // Since we are expecting ASCII in the response, ignore control characters
			buffer[i]=a;
		printf("%d   ",a);
		i++;
		//}
	    }

printf("i= %d  \n",i);
	buffer[i]='\0';  //because this is treated as a string, must manually
		// append the NULL terminator
	strncpy(pszEcho,buffer,i);
printf("\n");
}//end write_rs485



