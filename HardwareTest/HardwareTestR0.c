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


void main(void)
{
   // Initialize RTC Variables
   unsigned char year = 0x14; unsigned char month = 0x11; 
   unsigned char day = 0x07; unsigned char date = 0x08;
   unsigned char hrs = 0x67; unsigned char min = 0x00; unsigned char sec = 0x00;
   
   // Build test messages to be sent
	unsigned char msg1[6] = {ADDRESS, 0x02, 0x7F};
      // Roboclaw message to move motors
	unsigned char msg2a[6] = {ADDRESS, 8, 30, 0x26}; //((ADDRESS+8+30) & 0x7F))
   
   // Roboclaw message to stop motors
	unsigned char msg2b[6] = {ADDRESS, 8, 0, 0x08}; //((ADDRESS+8+0) & 0x7F))
  	unsigned char msg3[20] = {0x80, 0x00,  0x81, 0x00, 0x82, 0x67, 0x83, 0x07, 0x84, 0x08, 0x85, 0x11, 0x86, 0x14};
   
   // Open ports; defined in Sensor_r#.h
	int port_9Ax = I2C1; // 
	int port_MC  = UART4; // tx pin 13
	int port_RTC = SPI0;
   
   // Initialize Error Checking and Buffer Variables
	int successFlag = 1; // 1 is Success; 0 is Failure
	unsigned int ser_charin_count,ser_charout_count;
	unsigned int size_rxbuf = 128;
	unsigned int count, i;
	unsigned char ser_rxbuf[size_rxbuf];
   
	printf( "\n****************** Testing Hardware (Sensor Comms, INPUTs, and OUTPUTs*******************" );
	printf( "\nFile Handles: port_9Ax: %d port_MC: %d port_RTC: %d\n", port_9Ax, port_MC, port_RTC );
	
	// Wait 0.2 second
	usleep( 200000 );
   
	// Write Bytes
//	write( port_9Ax, msg1, sizeof(msg1) );
//	write( port_MC,  msg2a, sizeof(msg2a) );
   forwardM1(170, port_MC );
   // Run motors for 2 seconds
   usleep(2000000);
//	write( port_MC,  msg2b, sizeof(msg2b) );
//	write( port_RTC, msg3, sizeof(msg3) );
   
	printf( "\n********* Testing Sensor Communication Protocols *********" );
	printf( "\nMessages Sent:" );
   
	// Display Sending 3 Messages
   printMsg( msg1,  sizeof(msg1)  );
   printMsg( msg2a, sizeof(msg2a) );
   printMsg( msg2b, sizeof(msg2b) );
   printMsg( msg3,  sizeof(msg3)  ); 
   printf("Size of Message: %d",sizeof(msg2a));
   
  
	// Wait 0.5 second
	usleep( 500000 );

/*
	// Read Buffers SPI 0  -- RTC **********************************************************************
	ser_charin_count = read( port_RTC, ser_rxbuf, size_rxbuf ); //port,buffer*,size of buffer
	printf( "\n%d Char(s) Received from RTC:", ser_charin_count );  
	// Error Check for Read()
	if( ser_charin_count > 0 && ser_charin_count != 0xFFFFFFFF )
   {
	// Display Received Bytes
		for( count = 0; count <= 8; count++ )
      {
			printf( "%2X", ser_rxbuf[count] );
			if( (count % 4) == 0 )
				printf( ":");
		}
	}else
   {
		printf( "Error - No Bytes Read errno is %d", errno);
      successFlag = 1;
	}
   printf("\n\n");


	// Read Buffers UART 2 **********************************************************************
	ser_charin_count = read( ser2, ser_rxbuf, size_rxbuf ); //port,buffer*,size of buffer
	printf( "\n%d Char(s) Received from UART2:", ser_charin_count );  
	// Error Check for Read()
	if( ser_charin_count > 0 && ser_charin_count != 0xFFFFFFFF )
   {
	// Display Received Bytes
		for( count = 0; count <= 2; count++ ){
			printf( "%2X", ser_rxbuf[count] );
//			if( (count % 4) == 0 )
//				printf( ":");
		}
	}else
   {
		printf( "Error - No Bytes Read errno is %d", errno);
      successFlag = 0;
	}
   
	// Read Buffers UART 4 **********************************************************************
	ser_charin_count = read( port_MC, ser_rxbuf, size_rxbuf ); //port,buffer*,size of buffer
	printf( "\n%d Char(s) Received from UART4:", ser_charin_count );  
	// Error Check for Read()
	if( ser_charin_count > 0 && ser_charin_count != 0xFFFFFFFF ){
	// Display Received Bytes
	for( count = 0; count <= 2; count++ )
   {
      printf( "%2X", ser_rxbuf[count] );
	   if( (count % 4) == 0 )
			printf( ":");
		}
	}else
   {
		printf( "Error - No Bytes Read errno is %d", errno);
      successFlag = 0;
	}
*/
	// Close Serial Ports
	close(port_9Ax);
	close(port_MC);
	close(port_RTC);
	
	printf( "\n\n*************************** End of Hardware Test **************************\n" );
}// end main