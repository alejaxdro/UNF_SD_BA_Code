// socket.c 
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

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>

// User-Defined Libraries
#include "getdata_r0.c"
#include "termlib.c"

// Global Variables
int socket_fd, newsockfd;
int portno;//, socket_fd, newsockfd;
socklen_t clilen;
struct sockaddr_in serv_addr, cli_addr;

// Declare User-Defined Functions
int openDataSocket( void );
int * string2integerPair( char *tempString, int arrayInts[2] );

void error(const char *msg)
{
    perror(msg);
    exit(1);
}


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

// **************************** User-Defined Functions ****************************

int * string2integerPair( char *tempString, int arrayInts[2] ){
   
   //printf( "Rx String: %s\n", tempString );
   // Parse tempString into tokens
   char * pchA;
   char * pchM;
   
   //printf ("Splitting string \"%s\" into tokens:\n",tempString);
   pchA = strtok (tempString,"AM");
   pchM = strtok (NULL,"AM");
   
   //printf ("%s-----",pchA);
   //printf ("%s\n",pchM);
   
   arrayInts[0] = atoi(pchA);
   arrayInts[1] = atoi(pchM);
   return 0;
}

int openDataSocket(){
   
   // Open socket
   socket_fd = socket(AF_INET, SOCK_STREAM, 0);
   if (socket_fd < 0){
      error("ERROR opening socket");
   }
   bzero((char *) &serv_addr, sizeof(serv_addr));
   // Port number to be used for 1st Port, Maybe another for a emergency heartbeat
   portno = 8081;
   
   serv_addr.sin_family = AF_INET; 
   serv_addr.sin_addr.s_addr = INADDR_ANY;
   serv_addr.sin_port = htons(portno);
   if ( bind(socket_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0 ){
      error("ERROR on binding");
   }
   
   // Listen for connection from client socket and accept
   listen(socket_fd,5);
   clilen = sizeof(cli_addr);
   newsockfd = accept(socket_fd, (struct sockaddr *) &cli_addr, &clilen);
   if (newsockfd < 0){
      error("ERROR on accept");
   }
   DEBUG_PRINT("Connected to ClientSocket. %d\n", newsockfd);
   
   return newsockfd;   
}// End OpenDataSocket()