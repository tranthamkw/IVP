#include "mathTools.h"

double stdDeviation(double* value, int numValues){
	double stdDev, sum, avg;

	// First Calculate the Average value
	sum = 0.0;
	int i;
	for(i=0; i < numValues;i++){ 
		sum += value[i];
	}
	avg = sum / (double) numValues;

	// Then calculate the Standard Deviation of the mean
	sum = 0.0;
	for(i=0; i < numValues;i++){
		sum += pow(avg - value[i],2);
	}
	stdDev = sqrt(sum/(numValues*(numValues-1)));

	return stdDev;
}

// Calculate percent uncertainty squared
float pu2(float value, float uncertainty){
	return pow(uncertainty/value,2);
}
