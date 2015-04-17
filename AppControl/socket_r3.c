// socket_r3.c 
// 
// Alejandro Lepervanche 
//
// Date: 3/09/2015         This code creates and provides functions for socket ip communication
//       3/12/2015         Current Message from App: "X:Y:Angle:Distance" This is converted to data!!
//       3/17/2015         Removing String to Number conversion and trying to add check for socket connected/disconnected
//                         Checks for clean break, but cannot handle softbreaks where no one is notified.
//       3/20/2015         Added parsing of message to convert to numbers
//       3/24/2015         Tried creating socket with Non-blocking calls. This did not work and adjusting timeout will have to be the solution.
//       3/26/2015         Rework parsing, had issues, has to be 100% reliable.
//       3/27/2015         Converted incoming data to Command Array with Angle and Magnitude with type integer
//       3/28/2015         Created function to convert string to int array. This works.. Don't change it.


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

// User-Defined Libraries
#include "roboClawLib.c"
#include "getdata_r1.c"
#include "termlib.c"


int main()
{
   char *ang_str, *dist_str, tempstr[64], buffer[256];
   signed int app_ang, app_dist;
   int cmdArray[2];
   int n, i = 0, rcvcnt = 0, exit = 0, retval, ret, errorn;
   int endMsg = 0, buildAng = 0, buildMag = 0;
   socklen_t len = sizeof (0);
      
   // Open Socket as Server, Listen for Client, and Accept and Bind for streamed communication
   newsockfd = openDataSocket();
   
   cmdArray[0] = 0;
   cmdArray[1] = 0;
   bzero( buffer,256 );
   bzero( tempstr,64 );
   
   // ************************ Main Loop ************************ //
   while( 1 ){//exit < 100 ){
      //usleep(1000);
      clear2eol();
      
      //printf("Read from App...      ");//    Loop #%d\n",exit);      
      
      // Socket is UP
      // This reads in socket input
      n = read( newsockfd, buffer, 1 );
      if ( n < 0 ) error( "ERROR reading from socket" );
      DEBUG_PRINT("Msg: %s\n", buffer );
      
      // Check for end of line of message from app
      if( buffer[0] == '\n' ){
         tempstr[rcvcnt] = '\0';
         //printf("Msg--> :%s:\n", tempstr );
         string2integerPair( tempstr, cmdArray );
         printf("COMMAND ARRAY: A %d, M %d\n", cmdArray[0], cmdArray[1]);
         rcvcnt = 0;
      }else{
        tempstr[rcvcnt] = buffer[0];
        rcvcnt++;
      }
      
      getData(); 
      
/*         // Socket is DOWN
      printf("Socket is DOWN! Retrying...\n");
      
      // Close Accepted Socket
      close(newsockfd);
      
      // Clear Buffer
      bzero( buffer,256 );
      bzero( tempstr,64 );         
      
      // Listen for connection from client socket and accept
      listen(socket_fd,5);
      newsockfd = accept(socket_fd, (struct sockaddr *) &cli_addr, &clilen);
      if (newsockfd < 0){
         error("ERROR on accept");
      }
*/         
      
      // This writes to socket output
      //n = send( newsockfd, "I got your message", 18, MSG_DONTWAIT);
     // if ( n < 0 ) error("ERROR writing to socket");
      
      exit++;
   }// end while
   
   
   // Close new socket and socket
   close(newsockfd);
   close(socket_fd);
   return 0; 
}
