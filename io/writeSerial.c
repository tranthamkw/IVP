/*


 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kenBoard.h"







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
//	write_rs485BYTE(command,j+2,chardata,&k);

	printf("\nOutput Command: %s\n",command);
	printf("Returned string:%s \n",chardata);

}

	closeBoard();

  return 0 ;
}
