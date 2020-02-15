
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#define BUFSIZE 1024
#define PI 3.141592653

void calculateStats(float* average, float* stdevmean, char start, char end, float* data){
	int i;
	float avg,stdvm,num;

	num = (float)(end - start);
	avg=0.0;
	for (i=start;i<end;i++){
		avg+=data[i];

	}
	avg = avg / num;


	stdvm=0.0;
	for (i=start;i<end;i++){
		stdvm+=pow(avg-data[i],2);
	}

	stdvm = sqrt(stdvm/(num*(num-1.0)));
	*stdevmean=stdvm;
	*average=avg;
}



float calculateDelta(float delta0, float delta1, float delta2, float tempT1, float tempT2, float temperature){
	float temp;
	temp = delta0 + (temperature-tempT1)*(delta2 - delta1)/(tempT2 - tempT1);
	temp = temp*2.0*PI;
	return temp;
}


int main (int argc, char **argv){

FILE *fp_in, *fp_out,*fp1;
char outFileName[BUFSIZE],inFileName[BUFSIZE];
char printHeader;
char linebuff[BUFSIZE],buff[64],comments[64];
char* pch;
int i,j,k;
int col;
float c0_a[25],c2_a[25],s2_a[25],c4_a[25],s4_a[25];
float c0_p[25],c2_p[25],s2_p[25],c4_p[25],s4_p[25];
float c0_b[10],c2_b[10],s2_b[10],c4_b[10],s4_b[10];
float cv_a[25],cv_p[25];
float led_a[25],led_p[25];
float temp_a[25],temp_p[25];
float temp;
float I,M,C,S;
float avg,stdvm;
float delta,delta0,delta1,delta2,tempT1,tempT2;
float beta0;

float* MoverI=malloc(50*sizeof(float));
float* CoverI=malloc(50*sizeof(float));
float* SoverI=malloc(50*sizeof(float));

fp1=fopen("opticalParameters.dat","r");
if (!fp1) {
	printf("Optical parameters file does not exist\n");
	exit(1);
}
fscanf(fp1,"%s %s %s %s %s %s",buff,buff,buff,buff,buff,buff);
fscanf(fp1,"%f %f %f %f %f %f",&beta0,&delta0,&delta1,&delta2,&tempT1,&tempT2);
delta = calculateDelta(delta0, delta1, delta2, tempT1, tempT2,33.652);
printf("beta0 %f\t delta %f\n",beta0, delta);
fclose(fp1);



if (argc==3) {

	strcpy(inFileName,argv[1]);
	strcpy(outFileName,argv[2]);

	//check to see if outFileName exists
	fp_out=fopen(outFileName,"r");
	if (!fp_out){
		// file does not exist
		printf("out file does NOT exist. Print header\n");
		printHeader=1;
	} else {
		printf("out file exists. \n");
		printHeader=0;
		fclose(fp_out);
	}
	fp_out=fopen(outFileName,"a");
	if (!fp_out) {
		printf("unable to open file %s \n",outFileName);
		exit(1);
		}

	fp_in=fopen(inFileName,"r");
	if (!fp_in){
		// does not exit
		printf("Input file %s does not exist\n",inFileName);
		exit(1);

		}
//Read First line. Thisis the header. dont do anything with it
		fgets(linebuff,sizeof(linebuff),fp_in);
		col = 1;
		pch = strtok(linebuff,"\t");
		while (pch!=NULL) {
			strcpy(buff,pch);
			pch=strtok(NULL,"\t");
			col++;
		}
		if (col!=31) {printf("\nUnexpected column position in file \n"); exit(1);}


for (k=0;k<5;k++){
		//background at atmo
		fgets(linebuff,sizeof(linebuff),fp_in);
		col = 1;
		pch = strtok(linebuff,"\t");
		while (pch!=NULL) {
			strcpy(buff,pch);
			pch=strtok(NULL,"\t");
			if (col==2) strcpy(comments,buff);
			if (col==11) c0_b[2*k]=atof(buff);
			if (col==17) c2_b[2*k]=atof(buff);
			if (col==19) s2_b[2*k]=atof(buff);
			if (col==21) c4_b[2*k]=atof(buff);
			if (col==23) s4_b[2*k]=atof(buff);
			col++;
		}
		if (col!=31) {printf("\nUnexpected column position in file \n"); exit(1);}
		//background at atmo

	for (j=0;j<5;j++){
		// data at atmo
		fgets(linebuff,sizeof(linebuff),fp_in);
		col = 1;
		pch = strtok(linebuff,"\t");
		while (pch!=NULL) {
			strcpy(buff,pch);
			pch=strtok(NULL,"\t");
			if (col==6) cv_a[k*5+j]=atof(buff);
			if (col==8) temp_a[k*5+j]=atof(buff);
			if (col==9) led_a[k*5+j]=atof(buff);
			if (col==11) c0_a[k*5+j]=atof(buff);
			if (col==17) c2_a[k*5+j]=atof(buff);
			if (col==19) s2_a[k*5+j]=atof(buff);
			if (col==21) c4_a[k*5+j]=atof(buff);
			if (col==23) s4_a[k*5+j]=atof(buff);
			col++;
		}
		if (col!=31) {printf("\nUnexpected column position in file \n"); exit(1);}
		//data at atmo
		}
		//background at vac
		fgets(linebuff,sizeof(linebuff),fp_in);
		col = 1;
		pch = strtok(linebuff,"\t");
		while (pch!=NULL) {
			strcpy(buff,pch);
			pch=strtok(NULL,"\t");
			if (col==11) c0_b[2*k+1]=atof(buff);
			if (col==17) c2_b[2*k+1]=atof(buff);
			if (col==19) s2_b[2*k+1]=atof(buff);
			if (col==21) c4_b[2*k+1]=atof(buff);
			if (col==23) s4_b[2*k+1]=atof(buff);
			col++;
		}
		if (col!=31) {printf("\nUnexpected column position in file \n"); exit(1);}

	for (j=0;j<5;j++){
		// data at pressure
		fgets(linebuff,sizeof(linebuff),fp_in);
		col = 1;
		pch = strtok(linebuff,"\t");
		while (pch!=NULL) {
			strcpy(buff,pch);
			pch=strtok(NULL,"\t");
			if (col==6) cv_p[k*5+j]=atof(buff);
			if (col==8) temp_p[k*5+j]=atof(buff);
			if (col==9) led_p[k*5+j]=atof(buff);
			if (col==11) c0_p[k*5+j]=atof(buff);
			if (col==17) c2_p[k*5+j]=atof(buff);
			if (col==19) s2_p[k*5+j]=atof(buff);
			if (col==21) c4_p[k*5+j]=atof(buff);
			if (col==23) s4_p[k*5+j]=atof(buff);
			col++;
		}
		if (col!=31) {printf("\nUnexpected column position in file \n"); exit(1);}


		//data at pressure
		}
	}

	//compute average background
	temp=0.0;for (k=0;k<10;k++) temp+=c0_b[k];c0_b[0]=temp/10.0;  //c0_b[0] element 0 now holds the average.
	temp=0.0;for (k=0;k<10;k++) temp+=c2_b[k];c2_b[0]=temp/10.0;  //c0_b[0] element 0 now holds the average.
	temp=0.0;for (k=0;k<10;k++) temp+=s2_b[k];s2_b[0]=temp/10.0;  //c0_b[0] element 0 now holds the average.
	temp=0.0;for (k=0;k<10;k++) temp+=c4_b[k];c4_b[0]=temp/10.0;  //c0_b[0] element 0 now holds the average.
	temp=0.0;for (k=0;k<10;k++) temp+=s4_b[k];s4_b[0]=temp/10.0;  //c0_b[0] element 0 now holds the average.

//	printf("Background values\nC0\tC2\tS2\tC4\tS4\n");
//	printf("%E\t%E\t%E\t%E\t%E\n",c0_b[0],c2_b[0],s2_b[0],c4_b[0],s4_b[0]);

	// background correct each coefficient
	for (k=0;k<25;k++){
		c0_p[k]=c0_p[k]-c0_b[0];
		c2_p[k]=c2_p[k]-c2_b[0];
		s2_p[k]=s2_p[k]-s2_b[0];
		c4_p[k]=c4_p[k]-c4_b[0];
		s4_p[k]=s4_p[k]-s4_b[0];
//	printf("%f\t%f\t%f\t%f\t%f\t%f\n",cv_p[k],c0_p[k],c2_p[k],s2_p[k],c4_p[k],s4_p[k]);
		}
//	printf("\n");
	for (k=0;k<25;k++){
		c0_a[k]=c0_a[k]-c0_b[0];
		c2_a[k]=c2_a[k]-c2_b[0];
		s2_a[k]=s2_a[k]-s2_b[0];
		c4_a[k]=c4_a[k]-c4_b[0];
		s4_a[k]=s4_a[k]-s4_b[0];
//	printf("%f\t%f\t%f\t%f\t%f\t%f\n",cv_a[k],c0_a[k],c2_a[k],s2_a[k],c4_a[k],s4_a[k]);
	}

	if (printHeader) fprintf(fp_out,"SourceFile\tComment\tCVGuage\tLEDcurrent\tTemperature\tM/I\tC/I\tS/I\n");
	printf("\nCV\tLED\tTemp\tM/I\tC/I\tS/I\n");

	for (k=0;k<25;k++){
		delta = calculateDelta(delta0, delta1, delta2, tempT1, tempT2,temp_p[k]);
		I=2.0*c0_p[k]+2.0*(s4_p[k]*sin(4*beta0)-c4_p[k]*cos(4*beta0))*(1+cos(delta))/(1-cos(delta));
		M=4.0*(c4_p[k]*cos(4*beta0)-s4_p[k]*sin(4*beta0))/(1-cos(delta));
		C=4.0*(c4_p[k]*sin(4*beta0)+s4_p[k]*cos(4*beta0))/(1-cos(delta));
		S=2.0*sqrt(pow(s2_p[k],2)+pow(c2_p[k],2))/sin(delta);
		MoverI[k]=M/I;
		CoverI[k]=C/I;
		SoverI[k]=S/I;
	printf("%f\t%f\t%f\t%f\t%f\t%f\n",cv_p[k],led_p[k],temp_p[k],MoverI[k],CoverI[k],SoverI[k]);
	fprintf(fp_out,"%s\t%s\t%f\t%f\t%f\t%f\t%f\t%f\n",inFileName,comments,cv_p[k],led_p[k],temp_p[k],MoverI[k],CoverI[k],SoverI[k]);
	}
	printf("\n");
	for (k=0;k<25;k++){
		delta = calculateDelta(delta0, delta1, delta2, tempT1, tempT2,temp_a[k]);
		I=2.0*c0_a[k]+2.0*(s4_a[k]*sin(4*beta0)-c4_a[k]*cos(4*beta0))*(1+cos(delta))/(1-cos(delta));
		M=4.0*(c4_a[k]*cos(4*beta0)-s4_a[k]*sin(4*beta0))/(1-cos(delta));
		C=4.0*(c4_a[k]*sin(4*beta0)+s4_a[k]*cos(4*beta0))/(1-cos(delta));
		S=2.0*sqrt(pow(s2_a[k],2)+pow(c2_a[k],2))/sin(delta);
		MoverI[k+25]=M/I;
		CoverI[k+25]=C/I;
		SoverI[k+25]=S/I;
	printf("%f\t%f\t%f\t%f\t%f\t%f\n",cv_a[k],led_a[k],temp_a[k],MoverI[k+25],CoverI[k+25],SoverI[k+25]);
	fprintf(fp_out,"%s\t%s\t%f\t%f\t%f\t%f\t%f\t%f\n",inFileName,comments,cv_a[k],led_a[k],temp_a[k],MoverI[k+25],CoverI[k+25],SoverI[k+25]);
	}


	fclose(fp_in);
	fclose(fp_out);
	printf("%s\n",comments);


} else {
	printf("Useage ./analyzePolVac <input PolVac file> <output summary file> \n");
}

return 0;

}
