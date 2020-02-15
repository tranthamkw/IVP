/*

template GPIB functions to talk to a FLUKE8840 and a KEITHLEY485

 */



int getReadingK485(float* amps);
int getReadingF8840(float* volts);

int initializeF8840(void);

int initializeK485(void);
