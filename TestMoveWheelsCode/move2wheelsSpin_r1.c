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

// Global Declarations
   // typedef union{
         // short val;
         // unsigned char byte[2];
      // } SH_CH2;
   // typedef struct{
         // SH_CH2 x; 
         // SH_CH2 y;
         // SH_CH2 z;
      // } SENSOR_XYZ; 
   // typedef union{
         // SENSOR_XYZ data;
         // unsigned char byt[6];
      // } SENSOR_BUF;
      

int main (void)
{
	unsigned char data[FOUR];
	int i, turn, speed, stop = 64;
   int maxMagnNorth = 0;
	unsigned char arr[FOUR];
	int ser4 = UART4;
   
   // Initialize 9 axis Sensor
   printf( "\nInitialize Sensor...\n");
   init9axis();
   
   // Moving 2 wheels from one UART  
   //usleep(2000000);  // Delay (in micro seconds) for user to walk to watch the motor moving
   
   // Set speed = 0 to 63
   turn = 20;
   
   // Accelerate Turn to turn
   for(i = 0; i <= ( turn ); i++){
      // Turns 1/2 turn
      forwardBackwardM1( stop + i, ser4 );
      forwardBackwardM2( stop - i, ser4 );      
      // Acceleration delay
      usleep( 40000 );
      // Get North Measurement and compare and save
      DEBUG_PRINT( "Get Magnetometer Data... Sample %d\n", i);
      getData();
      if( magn.data.x.val > maxMagnNorth){
         maxMagnNorth = magn.data.x.val;
         printf( "Max Magnitude is %d\n", maxMagnNorth);
      }
   }// end for loop
   
   // Collect Magnetic measurements while turning in place
   
   //usleep( 2000000 );            // <-- change to loop to gather data at 20000 us sample period
   for(i = 0; i <= 100; i++){   
      // Acceleration delay
      usleep( 30000 );
      // Get North Measurement and compare and save
      DEBUG_PRINT( "Get Magnetometer Data... Sample %d\n", i);
      getData();
      if( magn.data.x.val > maxMagnNorth){
         maxMagnNorth = magn.data.x.val;
         printf( "Max Magnitude is %d\n", maxMagnNorth);
      }
   }// end for loop
   
   // Deccelerate
   for(i = turn; i >= 0; i--){
      // Turns
      forwardBackwardM1( stop + i, ser4 );
      forwardBackwardM2( stop - i, ser4 );
      // Acceleration delay
      usleep( 20000 );   
   }// end for loop
	
   // Stop Motors
	forwardBackwardM1( stop, ser4 );
	forwardBackwardM2( stop, ser4 );
   
   close(ser4);
	return 0;

}// end main
