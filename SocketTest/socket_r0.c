// socket.c 
// 
// Alejandro Lepervanche 
//
// Date: 3/09/2015         This code creates and provides functions for socket ip communication
//       3/12/2015         Current Message from App: "X:Y:Angle:Distance" This is converted to data!!
//
// 

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>

// Global Variables
int sockfd, newsockfd;

// Declare User-Defined Functions
int openDataSocket( void );

void error(const char *msg)
{
    perror(msg);
    exit(1);
}


int main()
{
   int newsockfd;
   char *x_str, *y_str, *ang_str, *dist_str,buffer[256];
   int app_x, app_y, app_ang, app_dist;
   int n, i = 0, exit = 0;

   // Open Socket as Server, Listen for Client, and Accept and Bind for streamed communication
   newsockfd = openDataSocket();
   
   bzero(buffer,256);
   while(1){
      // This reads in socket input
      n = read( newsockfd, buffer, 255 );
      if (n < 0) error("ERROR reading from socket");
      
      //printf("Here is the message: %s\n",buffer);
      // Decide what to do based on message
      
      // Token used as delimiter for incoming messages
      char *token = strtok(buffer, ":");
      i = 0;
      // Separate Data from String
      while(token) {
         switch(i){
            case 0 : x_str    = token;
            case 1 : y_str    = token;
            case 2 : ang_str  = token;
            case 3 : dist_str = token;
         }
         i++;
         //puts(token);
         token = strtok(NULL, ":");
      }
      printf("Contents of the input stringlets:\nX     :%s\nY     :%s\nAngle :%s\nDist  :%s\n",x_str,y_str,ang_str,dist_str);
      
      // make app.x etc into a struct
      app_x    = atoi( x_str );
      app_y    = atoi( y_str );
      app_ang  = atoi( ang_str );
      app_dist = atoi( dist_str );
      
      printf("Contents message in integer form:\nX     :%d\nY     :%d\nAngle :%d\nDist  :%d\n\n",app_x,app_y,app_ang,app_dist);
      
      usleep( 500000 );
      
      //exit++;
   }// end while
   
   // This writes to socket output
   n = write(newsockfd,"I got your message",18);
   if (n < 0) error("ERROR writing to socket");
   
   // Close new socket and socket
   close(newsockfd);
   close(sockfd);
   return 0; 
}

// **************************** User-Defined Functions ****************************

int openDataSocket(){
   int portno;//, sockfd, newsockfd;
   socklen_t clilen;
   struct sockaddr_in serv_addr, cli_addr;
   
   // Open socket
   sockfd = socket(AF_INET, SOCK_STREAM, 0);
   if (sockfd < 0){
      error("ERROR opening socket");
   }
   bzero((char *) &serv_addr, sizeof(serv_addr));
   // Port number to be used for 1st Port, Maybe another for a emergency heartbeat
   portno = 8081;
   
   serv_addr.sin_family = AF_INET; 
   serv_addr.sin_addr.s_addr = INADDR_ANY;
   serv_addr.sin_port = htons(portno);
   if ( bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0 ){
      error("ERROR on binding");
   }
   
   // Listen for connection from client socket and accept
   listen(sockfd,5);
   clilen = sizeof(cli_addr);
   newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
   if (newsockfd < 0){
      error("ERROR on accept");
   }
   
   return newsockfd;   
}// End OpenDataSocket()