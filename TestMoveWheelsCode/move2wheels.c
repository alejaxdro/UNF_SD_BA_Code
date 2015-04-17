// move2wheels.c
// 
// This code moves wheels forward and backwards for a couple seconds each
// It uses UART2 to send packet serial data to RoboClaw Motor Controller
//
// Author:  Alejandro         01/19/2015
//

#include <stdio.h>
#include <stdlib.h>
#include "roboClawLib.c"
#include "RoboClaw.h"
#include <stdbool.h>

int main (void)
{
	unsigned char data[FOUR];
	unsigned char arr[FOUR];
   int i;
	int ser4 = UART4;
	//int ser2 = UART2;

   // Moving 2 wheels from one UART   
   
//   usleep(2000000);  // Delay (in micro seconds) for user to walk to watch the motor moving
   
   // Runs forward 1/2 speed
//	forwardBackwardM1(96, ser4);
//	forwardBackwardM2(96, ser4);
   
//  usleep(1000000);   
   
   // Stops
//	forwardBackwardM1(64, ser4);
//	forwardBackwardM2(64, ser4);
   
//   usleep(1000000);
   while(i<10){
      i++;
   // Runs backward 1/2 speed
	forwardBackwardM1(32, ser4);
	forwardBackwardM2(32, ser4);
	
	usleep(50000);
	}
   // Stops
	forwardBackwardM1(64, ser4);
	forwardBackwardM2(64, ser4);
   
   close(ser4);
   //close(ser2);
	return 0;

}// end main
