
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#define BUFFSIZE 512
#define PI 3.141592653


int main (int argc, char **argv){

FILE *fp_in, *fp_out,*fp1;
char outFileName[BUFFSIZE],inFileName[BUFFSIZE];
char printHeader;
char buff[BUFFSIZE];
char comments[64];
int i,j,k;
char* pch;



if (argc==2) {

	strcpy(inFileName,argv[1]);

	fp_in=fopen(inFileName,"r");
	if (!fp_in){
		// does not exit
		printf("Input file %s does not exist\n",inFileName);
		exit(1);
		}
//Read First line. Thisis the header. dont do anything with it

	fgets(buff, sizeof(buff),fp_in);
	printf("%s\n",buff);
	pch = strtok(buff,"\t");
	while (pch!=NULL)
	{
		strcpy(comments,pch);
		printf("%s\t%s\n",pch,comments);

		pch=strtok(NULL,"\t");

	}

} else {
	printf("Useage ./analyzePolVac <input PolVac file> <output summary file> \n");
}

return 0;

}
