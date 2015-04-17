// move2wheelsControl.c
//
// This code moves two wheels using the RoboClaw by scanning user input 
// Controls include acceleration, top speed, and turning difference
//
// Author:	Alejandro Lepervanche		01/30/2015  Put this together
//          "        "                 02/24/2015  Tried timing to prevent losing control...
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "roboClawLib.c"
#include "roboClaw.h"
#include "getdata_r0.c"
#include "termlib.c"


int main ( void )
{
	// Initialize variables
   unsigned char data[FOUR];
   int exitFlag = 0;
   int topspeed, dspeed, speed1=0, speed2=0;
   int stop = 64, turn = 0, spdlimitflag=0;
	char input;
   
   // Open UART4 port
   int ser4 = UART4;
   
   // Delay (in micro seconds)
   usleep( 1000000 );  
   
   // SET SPEED BETWEEN 0 to 63, it is added to stop command
   // This is used only for the function forwardBackwardM1() and ..M2()
   
   // Set speed increment
   dspeed = 5;
   topspeed = 63;

   clearscreen();
   cursorposition(1,1);
   
   // Display prompt
   displayprompt();   

   while(!exitFlag){
      
      // Display Warnings
      cursorposition(1,16);
      if (spdlimitflag){
         printf("WARNING! Speed limit reached.");
         spdlimitflag = 0;
      }else {
         clear2eol();
         printf("Current Speeds = M1-->%d, M2-->%d\n",speed1+turn,speed2+-turn);
         clear2eol();
         printf("Current Turn Coefficient = %d, Actual turn difference is %d",turn, turn*2);         
      }
      
      // Scan for user input
      cursorposition(4,15);
      clear2eol();
      // Get command from USER
      scanf("%c", &input);
      //printf("Input value is:%c:\n", input);
      
      // Set values based on input
      if (input=='z'){
         // Exit While loop
         exitFlag = 1;
      } else if (input=='q'){
         // Stop motors
         speed1 = 0; speed2 = 0;
         turn = 0;
      } else if (input=='r'){
         // Exit While loop
         exitFlag = 1;
      } else if (input=='w'){
         // Increase speed
         speed1 += dspeed;
         speed2 += dspeed;
      } else if(input=='s'){
         // Decrease speed
         speed1 -= dspeed; 
         speed2 -= dspeed;
      }else if(input=='a'){
         // Increase turn
         turn += dspeed;
      } else if(input=='d'){
         // Decrease turn
         turn -= dspeed;
      }
      
      
      // Safety check to limit speed
      if ((speed1+turn <= topspeed) && (speed2+turn <= topspeed ) && (speed1-turn >= -topspeed) && (speed2-turn >= -topspeed)){
      } else {
         // Speed Limit Reached; Set speed limit flag
         spdlimitflag = 1;
         if (speed1 > 0 && speed2 > 0){
            speed1 = 60;  speed2 = 60;
            
         }else {
            speed1 = -60; speed2 = -60; 
         }
         turn = 0;
      }
      
      // Send control packet to motors
      forwardBackwardM1( stop + speed1 + -turn, ser4 );
      forwardBackwardM2( stop + speed2 + turn , ser4 );       
      
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
   
   clearscreen();
	return 0;

}// end main
