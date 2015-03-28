// move2wheelsNorth.c
//
// This code moves two wheels using the RoboClaw by scanning user input 
// Controls include acceleration, top speed, and turning difference
//
// Author:	Alejandro Lepervanche		01/30/2015  Put this together
//          "        "                 02/14/2015  Added speed and turn checks to limit speed
//          "        "                 02/25/2015  Adding sensor data to turn robot north
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
      

int main ( void )
{
	// Initialize variables
   int exitFlag = 0;
   int topspeed, dspeed, speed1=0, speed2=0;
   int stop = 64, turn = 0, spdlimitflag=0;
   int i, maxNorth, north;
   double angle;
   
   // Open UART4 port
   int ser4 = UART4;

   // Initialize 9 axis Sensor
   DEBUG_PRINT( "\nInitialize Sensor...\n");
   init9axis();
   
   // Delay (in micro seconds)
   usleep( 1000000 );  
   
   // Spin Robot, samples magnetic field and finds maximum magnetic field
   printf( "Scanning area for Maximum Magnetic Field...\n");
   maxNorth = SpinRobotAndFindMaxNorth( ser4 );
	printf("Max Magnetic Field found: %d\n\n", maxNorth );
   
   
   // SET SPEED BETWEEN 0 to 63, it is added to stop command
   // This is used only for the function forwardBackwardM1() and ..M2()
   
   // Set speed parameters
   dspeed = 2;
   topspeed = 40;
   DEBUG_PRINT( "Speed Settings: \n dspeed = %d \ntopspeed = %d\n", dspeed, topspeed );
   
   // Set North magnitude
   north = maxNorth - 300; //2700;//3100;
   DEBUG_PRINT( "North Magnitude for FindNorth Control = %d\n", north );
   
   while(!exitFlag){
      
      // Display Warnings
      cursorposition(1,16);
      if (spdlimitflag){
         DEBUG_PRINT("WARNING! Speed limit reached.");
         spdlimitflag = 0;
      }
      
      // Update current data points
      getData();
      

      
      // Display Speeds/Turns
      printf("Current Speeds = M1-->%d, M2-->%d\n",speed1+turn,speed2+-turn);
      printf("Current Turn Coefficient = %d, Actual turn difference is %d\n\n",turn, turn*2);         
      
      // Safety check to limit speed
      if ((speed1+turn <= topspeed) || (speed2+turn <= topspeed ) || (speed1-turn <= topspeed) || (speed2-turn <= topspeed)){
      } else {
         // Speed Limit Reached; Set speed limit flag
         spdlimitflag = 1;
         if (speed1 > 0 && speed2 > 0){
            speed1 = 0;  speed2 = 0; turn = 0;
         }else{
            speed1 = 0; speed2 = 0; turn = 0;
         }
         turn = 0;
      }
      
      usleep( 100000 );  
      // Send control packet to motors
      forwardBackwardM1( stop + speed1 + turn, ser4 );
      forwardBackwardM2( stop + speed2 + -turn , ser4 );       
      
      // Use this to exit Loop
      //exitFlag = 1;
   }// end while(!exitFlag)

   
   // Stops both motors; This is for Safety.
	forwardBackwardM1( 64, ser4 );
	forwardBackwardM2( 64, ser4 );
   
   // Wait 1 second
   usleep( 1000000 );
   
   // Close UART4 device  
   close( ser4 );
	return 0;

}// end main


int calculateTurnRate() {
   
      // Find Angle from north to set turn
      if( magn.data.x.val < maxNorth){
         // Calculate offset north
         angle = calcNorthOffset( maxNorth, magn.data.x.val, magn.data.y.val, magn.data.z.val );
         
         // Decide which way to turn
         if(angle > 40){
            // Increase turn
            DEBUG_PRINT( "\nIncrease Turn: Right?\n\n");
            turn += dspeed;
         }else if( angle < -40){
            // Decrease turn
            DEBUG_PRINT( "\nDecrease Turn: Right?\n\n");
            turn -= dspeed;
         }else{
            turn = 0;
         }
            
      }else{
         // Stop Motors, North is in Range
         DEBUG_PRINT( "\nDecrease Turn: Left?\n\n");
         turn = 0;
      }
}