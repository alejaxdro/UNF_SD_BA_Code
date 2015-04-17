// move2wheels.c
// 
// This code moves wheels forward and backwards for a couple seconds each
// It uses UART2 to send packet serial data to RoboClaw Motor Controller
// This is different from move2wheels because acceleration is trying to be accomplished
//
// Author:  Alejandro         01/19/2015
//

#include <stdio.h>
#include <stdlib.h>
#include "roboClawLib.c"
#include "RoboClaw.h"
#include <stdbool.h>

int main ( void )
{
	unsigned char data[FOUR];
	unsigned char arr[FOUR];
   int i, topSpeed, speed, stop = 64;
	//int ser1 = UART1;
	int ser4 = UART4;
   
   // Moving 2 wheels from one UART   
   
   usleep( 2000000 );  // Delay (in micro seconds) for user to walk to watch the motor moving
   
   // Set speed = 0 to 63
   speed = 30;
   
   // Accelerate to speed
   for(i = 0; i <= ( speed ); i++){
      // Runs forward 1/2 speed
      forwardBackwardM1( stop + i, ser4 );
      forwardBackwardM2( stop + i, ser4 );      
      // Acceleration delay
      usleep( 40000 );
   }// end for loop
   
   topSpeed = i;
   // Running at
   usleep( 1000000 );  
   
   // Deccelerate
   for(i = speed; i >= 0; i--){
      // Runs forward 1/2 speed
      forwardBackwardM1( stop + i, ser4 );
      forwardBackwardM2( stop + i, ser4 );
      // Acceleration delay
      usleep( 30000 );   
   }// end for loop

   
   // Stops both motors
	forwardBackwardM1( 64, ser4 );
	forwardBackwardM2( 64, ser4 );
   
   usleep( 1000000 );
   
   // // Runs backward 1/2 speed
	// forwardBackwardM1( 32, ser4 );
	// forwardBackwardM2( 32, ser4 );
	
	// usleep(1000000);
	
   // // Stops
	// forwardBackwardM1( 64, ser4 );
	// forwardBackwardM2( 64, ser4 );
   
   //close( ser1 );
   close( ser4 );
	return 0;

}// end main
