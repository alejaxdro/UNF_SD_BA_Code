// appControl_r0.c
//
// This code controls the turning of the Blue Armadillo by reading control data from App 
// Controls include command Angle and Magnitude
//
// Author:	Alejandro Lepervanche		
// Date:
//          03/28/2015        This code brings socketTest and Move2wheels together to control the robot through the app
//          03/29/2015        Unknown Error is not allowing receiving.
//          03/31/2015        Resolved by reading buffer and discarding data
//                            Adding turn control
//          04/1/2015         Success in controlling turn using app command angle.
//                            Added 5 stage acceleration/deceleration control
//          04/02/2015        R.U.G.S is a success!!
//          04/03/2015        
//          04/04/2015        Sounds from SD card

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <math.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

// User Defined Libraries
#include "roboClawLib.c"
#include "getdata_r2.c"
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
   int topspeed, dspeed, speed=0, speed1=0, speed2=0;
   int stop = 64, turn = 0, spdlimitflag=0;
   int i=0, maxNorth = 0, north;
   double angle;
   
   // Socket Variables
   char tempstr[64], buffer[1024], clrBuffer[1024];
   int cmdArray[2];
   int n, rcvcnt = 0, errorn;
   int endMsg = 0, buildAng = 0, buildMag = 0;
   socklen_t len = sizeof (0);
   int current_time, timeout;
   
   // Open Socket as Server, Listen for Client, and Accept and Bind for streamed communication
   newsockfd = openDataSocket();
   
   // Open UART4 port
   int ser4 = UART4;
   
   // Initialize 9 axis Sensor
   DEBUG_PRINT( "\nInitialize Sensor...\n");
   init9axis();
   
   // Delay (in micro seconds)
   //usleep( 1000000 );  
   
   // Spin Robot, samples magnetic field and finds maximum magnetic field
   DEBUG_PRINT( "Scanning area for Maximum Magnetic Field...\n");
   maxNorth = SpinRobotAndFindMaxNorth( ser4 );
	DEBUG_PRINT("Max Magnetic Field found: %d\n\n", maxNorth );

   // Set North magnitude
   north = maxNorth - (maxNorth/7); //2700;//3100;
   DEBUG_PRINT("North Magnitude for FindNorth Control = %d\n", north );
   
   // SET SPEED BETWEEN 0 to 63, it is added to stop command
   // This is used only for the function forwardBackwardM1() and ..M2()
   
   // Set speed and direction parameters
   dspeed = 2;
   topspeed = 50;
   DEBUG_PRINT("Speed Settings: \n dspeed = %d \ntopspeed = %d\n", dspeed, topspeed );
   cmdArray[0] = 0;
   cmdArray[1] = 0;
   bzero( clrBuffer, 1024 );
   bzero( buffer, 256 );
   bzero( tempstr, 64 );
   timeout = (int)time(NULL);
   while(!exitFlag){
      // Get current epoch time as int.
      current_time = (int)time(NULL);
      
      // Get App commandAngle here
      // This reads in socket input
      n = recv( newsockfd, buffer, 1, 0 );
      if ( n < 0 ) error( "ERROR reading from socket" );
//      printf("Msg: %s\n", buffer );
      
      // Check for end of line of message from app
      if( buffer[0] == '\n' ) {
         // Check for music command and play
         // P# and use # to pick sound file off SD card
         
         //playsound( tempstr );
         
         
         // Adds end of line to tempstr to make it a string
         tempstr[rcvcnt] = '\0';
         recv( newsockfd, clrBuffer, 1024, 0 );
//         printf("Msg--> :%s:\n", tempstr );
         // Receive A###M###P#\n and convert to 2 integers, angle and magnitude
         string2integerPair( tempstr, cmdArray );
         //printf("COMMAND ARRAY: A %3d, M %3d\n", cmdArray[0], cmdArray[1]);
         if( cmdArray[1] == 0 ){
            E_Stop = 1;
         }else{
            E_Stop = 0;
         }
         // Delay Loop
         if( i = 350 ){
            // Update current data points
            getData();
            // Calculate TURN RATE from Command angle and adjust by adding to current turn difference
            if( cmdArray[0] != 0 ){
               turn += calculateTurnRate( north, turn, cmdArray[0] );
            }
            // Calculate SPEED from Command and adjust by adding to current speed    
            
            DEBUG_PRINT("Speed Enabled <--main-- at time: %d\n", current_time);
            if( speedEnable == 1 ){
               speed += calculateSpeed( topspeed, speed, turn, cmdArray[1] );
               speed1 = speed;
               speed2 = speed;
            }
            
            // Display Speeds/Turns
            //printf("Current Speeds = M1-->%d, M2-->%d\n", speed1+turn, speed2-turn);
            //printf("Current turn: %d\n", turn );
            
            // Safety check to limit speed
            if ((speed1+turn <= topspeed) && (speed2+turn <= topspeed ) && (speed1-turn <= topspeed) && (speed2-turn <= topspeed)){
            } else {
               // Speed Limit Reached; Set speed limit flag
               spdlimitflag = 1;
               speed1 = 0;
               speed2 = 0;
               speed = 0;
               turn   = 0;
            }
            
            // Send control packet to motors ********************************** MOTOR CONTROL STATEMENTS
            if( E_Stop == 0 ){
               timeout = (int)time(NULL);
               // Add and send control variables
               forwardBackwardM1( ( stop + speed1 + turn ), ser4 );
               forwardBackwardM2( ( stop + speed2 + -turn ), ser4 );    
            }else{
               // Get current epoch time as int.
               DEBUG_PRINT("Current Time: %d, Timeout: %d\n", current_time, timeout);
               if( current_time - timeout > 20 ){
                  close(newsockfd);
                  // Listen for connection from client socket and accept
                  listen(socket_fd,5);
                  clilen = sizeof(cli_addr);
                  newsockfd = accept(socket_fd, (struct sockaddr *) &cli_addr, &clilen);
                  if (newsockfd < 0){
                     error("ERROR on accept");
                  }
                  timeout = (int)time(NULL);
                  printf("Connected to ClientSocket. %d\n", newsockfd);
               }                  
               // Stops both motors; This is for Safety.
               forwardBackwardM1( stop, ser4 );
               forwardBackwardM2( stop, ser4 );
            }
            i = 0;
         }
         i++;
         rcvcnt = 0;
      }else{
        tempstr[rcvcnt] = buffer[0];
        rcvcnt++;
      }
      
      
      
      // This writes to socket output
      //n = send( newsockfd, "I got your message", 18, MSG_DONTWAIT);
      // if ( n < 0 ) error("ERROR writing to socket");
      
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
   
   // Close new socket and socket
   close(newsockfd);
   close(socket_fd);
	return 0;

}// end main