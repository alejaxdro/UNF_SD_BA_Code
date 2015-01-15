/* This code is used to test the peripherals connected to the Beaglebone Black
	It tests: UART2, I2C2(I2C1A1), SPI0

	Written by: Alejandro Lepervanche
		   Elizabeth Voelkel
		   Marissa Miller

	Change Log:
	11/08/2014
		Alejandro created this and added code written by him 
		in the past as a template.
	11/29/2014
      Alejandro finished wiring and building robot to test motor controller and motors. So test message will be modified to move and stop motors.
   
*/ 


#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>
//#include <stdbool.h> commented out to see if even needed. look for errors.

#include "termlib_r0.h" // Allows for objects and positioning on terminal
#include "RoboClawLib_r0.c"  // Allows for use of Motor Control Functions
//#include "serial.h"    // Allows for use of serial functions


void printMsg( unsigned char[], int ); 
void motorUp(); //motor forward
void motorBack(); //motor backwards
void motorStop(); //stop motors
void motorLeft(); //motors go left
void motorRight(); //motors go right

int port_MC  = 1;

void main(void)
{
   int byteSent = 0;
   // Build test messages to be sent
   // Roboclaw message to move motors
	unsigned char msg1[4] = {128, 12, 127, 11};//((ADDRESS+12+127) & 0x7F))
   // Roboclaw message to stop motors
	unsigned char msg2[4] = {128, 12, 64, 76};//((ADDRESS+12+64) & 0x7F))
   
   // Open ports; defined in Sensor_r#.h
	int port_MC  = UART4; // tx pin 13
   
   
	printf( "\n****************** Testing Hardware (Sensor Comms, INPUTs, and OUTPUTs*******************" );
	printf( "\nFile Handles:port_MC: %d \n", port_MC );
	
	// Wait 0.2 second
	usleep( 200000 );
   
   int simple =  95;
	// Move Motors
	byteSent = write( port_MC,  msg1, sizeof(msg1) );
//   write_n( msg1, port_MC );
// forwardM1(170, port_MC );


   // Run motors for 2 seconds
   usleep(2000000);
   
   
   // Stop Motors
   simple = 0;
	byteSent = write( port_MC,  msg2, sizeof(msg2) );
//	write( port_MC,  msg2, sizeof(msg2) );
// write_n( msg2, port_MC );

   
	printf( "\n********* Testing Sensor Communication Protocols *********" );
	printf( "\nMessages Sent:" );
   
	// Display Sending 3 Messages
   printMsg( msg1, sizeof(msg1) );
   printMsg( msg2, sizeof(msg2) );
   printf("Size of Message: msg1:%d: msg2:%d:\n",sizeof(msg1),sizeof(msg2));
   printf("Bytes actually sent:%d:\n",byteSent);
   

	// Close Serial Ports
	close(port_MC);
	
	printf( "\n\n*************************** End of Hardware Test **************************\n" );
}// end main

// *********************************** Functions ***************************
void printMsg( unsigned char msg[], int size ) 
{
   int count;
   
   // Display Sending 3 Messages
   printf( ": \nMsg:");
   for( count = 0; count < size; count++ )
   {
      printf( "%2X", msg[count] );
   }
   printf( ":\n");
}

void motorUp(){
    forwardBackwardM1(96, port_MC); 
    forwardBackwardM2(96, port_MC);
    return;
}

void motorBack(){
	forwardBackwardM1(96, port_MC);
	forwardBackwardM2(96, port_MC);
	return;
}

void motorStop(){
	forwardBackwardM1(64, port_MC);
	forwardBackwardM2(64, port_MC);
    return;
}

void motorLeft(){
   forwardBackwardM1(96, port_MC);
	forwardBackwardM2(32, port_MC);
   return;
}

void motorRight(){
    forwardBackwardM1(32, port_MC);
    forwardBackwardM2(96, port_MC);
    return;
}
