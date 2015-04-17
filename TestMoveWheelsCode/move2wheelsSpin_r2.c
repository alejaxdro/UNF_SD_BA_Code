// move2wheels.c
// 
// This code moves wheels forward and backwards for a couple seconds each
// It uses UART2 to send packet serial data to RoboClaw Motor Controller
//
// Author:  Alejandro     01/19/2015
//                        03/27/2015   Adding getdata as to spin in a circle and display 
//
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <errno.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <math.h>

// User Defined Libraries
#include "roboClawLib.c"
#include "getdata_r0.c"
#include "termlib.c"


int main (void)
{
   int stop = 64;
   int maxNorth;
	int ser4 = UART4;
   
   // Initialize 9 axis Sensor
   printf( "\nInitialize Sensor...\n");
   init9axis();
   
   // Spin Robot, samples magnetic field and finds maximum magnetic field
   printf( "Scanning area for Maximum Magnetic Field...\n");
   
   maxNorth = SpinRobotAndFindMaxNorth( ser4 );
	printf("Max Magnetic Field found: %d\n\n", maxNorth );
   
   // Stop Motors Just In Case
	forwardBackwardM1( stop, ser4 );
	forwardBackwardM2( stop, ser4 );
   
   close(ser4);
	return 0;
}// end main