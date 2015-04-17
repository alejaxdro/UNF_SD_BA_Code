// socket.c 
// 
// Alejandro Lepervanche 
//
// Date: 3/09/2015         This code creates and provides functions for socket ip communication
//       3/12/2015         Current Message from App: "X:Y:Angle:Distance" This is converted to data!!
//       3/17/2015         Removing String to Number conversion and trying to add check for socket connected/disconnected
//                         Checks for clean break, but cannot handle softbreaks where no one is notified.
//       3/20/2015         Added parsing of message to convert to numbers
//       3/24/2015         Removed parsing. CHANGE to UDP instead of stream to prevent blocking and looking for /n's.
//

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <time.h>

// User-Defined Libraries
#include "getdata_r0.c"

// User-Defined Constants
#define BUFSIZE 1024
#define LOCAL_PORT 8081
#define REMOTE_PORT 8081

// Global Variables
int rc;
unsigned int rcv_cnt;
clock_t timestamp;

// Declare User-Defined Functions
void error( const char* );



int main(int argc, char **argv)
{
   // Local Variables  
   int n, i = 0, exit = 0, retval, ret, errorn;
   int msg_complete = 1, sectornum = 0;

   char *x_str, *y_str, *ang_str, *dist_str, tempstr[64];
   signed int app_x, app_y, app_ang, app_dist;   
   
   // Socket Variables
   struct sockaddr_in myaddr;           // my BBB's address 
   struct sockaddr_in remaddr;          // Remote Android's address
   socklen_t addrlen = sizeof(remaddr); // length of addresses
   int recvlen;                         // # bytes received
   int sockfd;                          // our socket
   unsigned char buf[BUFSIZE];          // receive buffer
   
   
   // ***************  Open UDP Socket  *************** //
      sockfd = socket(AF_INET, SOCK_DGRAM, 0);//  | SOCK_NONBLOCK <-- another way to set non-blocking socket but did not work..
      if (sockfd < 0) error("cannot create socket\n"); 

      // Set socket to non-blocking
      fcntl(sockfd, F_SETFL, O_NONBLOCK);
      
      // bind the socket to any valid IP address and a specific port
      memset((char *)&myaddr, 0, sizeof(myaddr)); 
      myaddr.sin_family = AF_INET; 
      myaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
      myaddr.sin_port = htons(LOCAL_PORT); 
      rc = bind(sockfd, (struct sockaddr *)&myaddr, sizeof(myaddr));
      if ( rc < 0 ) error("bind failed"); 
      
      // Setting Option to Allow Sending to IPV4 Broadcast Address
      int broadcast = 1;
      rc = setsockopt(sockfd,SOL_SOCKET,SO_BROADCAST,&broadcast,sizeof(broadcast)); 
      if (rc == -1) 
      {
         fprintf(stderr, "setsockopt() failed\n");
         return 0;
      }
      
      // Remote Address
      char *server = "192.168.1.200";	// Broadcast?? 255.255.255.255
      memset((char *) &remaddr, 0, sizeof(remaddr));
      remaddr.sin_family = AF_INET;
      remaddr.sin_port = htons(REMOTE_PORT);
      rc = inet_aton(server, &remaddr.sin_addr);
      if ( rc == 0 ) 
      {
         fprintf(stderr, "inet_aton() failed\n");
         return 0;
      }
   // ********** End of code that starts socket ********************** //

   
   // ************* Main loop *********************** //
   while( 1 ){//exit < 100 ){
      
      // Check for blocking call
      printf("Waiting for message...\n");
      
      // Socket is UP
      //DEBUG_PRINT("Read from Socket...     Loop #%d\n",exit);
      // This reads in socket input
      recvlen = recv(sockfd, buf, BUFSIZE, 0); 
      if (recvlen > 0) 
      { 
         rcv_cnt++;
         buf[recvlen] = 0; 
         
         CtrlPktParse( &buf, recvlen );
         
         
      } 
      // Send message
      buf[0] = 'H';
      buf[1] = 0;
      
      rc = sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr *)&remaddr, sizeof(remaddr));
      if ( rc < 0 ) error("sendto failed"); 
      
      // Delay (in micro seconds)
      usleep( 10000 ); 
      
      //exit++;
   }// end while
   
   // Close socket
   close(sockfd);
   return 0; 
}


// **************************** User-Defined Functions **************************** //

void error( const char *msg )
{
   perror( msg );
   exit( 1 );
}


CtrlPktParse( char* Pkt, int len )
{
   switch( Pkt[0] )
   {
      case 'H':
      // Hello Request
         pktdsp_H( Pkt, len );
         break;
      case 'h':
      // Hello Response
         pktdsp_h( Pkt, len );
         break;
      case 'I':
      // Input Change
         pktdsp_I( Pkt, len );
         break;
      case 'O':
      // Output Control
         pktdsp_O( Pkt, len );
         break;
      case 'R':
      // Reset Controller
         break;
      case 'r':
      // Reset Controller Response
         break;
      case 'S':
      // Store Controller Parameter Message
         break;
      case 'G':
      // Get Data
         break;
      case 'g':
      // Get Data Response
         break;
      case 'Z':
      // Special Test
         break;
      case 'F':
      // Get Memory Block
         break;
      case 'f':
      // Get Memory Block Response
         break;
      case 'U':
      // UI Write
         break;
      case 'u':
      // UI Input
         break;
      default:
      // Unknown Command
         pktdsp_unknown( Pkt, len );
         break;
      
   }
}


pktdsp_O( char* Pkt, int len )
{
// Output Control

   unsigned int Ndx;
   
   for(Ndx = 0; Ndx < len; Ndx++)
   {
      printf("%02X ", Pkt[Ndx]);
   }
   printf("%08X ", (unsigned long) clock());
   printf("Output Control");
   printf(", MAC-%02X:%02X:%02X:%02X:%02X:%02X"
          , Pkt[1], Pkt[2], Pkt[3], Pkt[4], Pkt[5], Pkt[6]);
   printf(", OutID-%d", (Pkt[8]*256)+Pkt[7]);
   printf(", Crnt-%d", (Pkt[10]*256)+Pkt[9]);
   printf(", Last-%d", (Pkt[12]*256)+Pkt[11]);
   printf(", Tgt-%d", (Pkt[14]*256)+Pkt[13]);
   printf(", Rate-%d", (Pkt[16]*256)+Pkt[15]);
   printf(", ChgTime-%d", (Pkt[20]<<24)+(Pkt[19]<<16)+(Pkt[18]<<8)+Pkt[17]);
   printf("\r\n");
}


pktdsp_H( char* Pkt, int len )
{
// Hello Request
   unsigned int Ndx;

   struct tm time;
   gettimeofday( &time, 0 );
   printf("%10d ", time.tm_sec);

   printf("Hello Request");
   if( len < 2 )
   {
      printf("\r\n");
      return;
   }
   printf(", MAC-%02X:%02X:%02X:%02X:%02X:%02X"
          , Pkt[1], Pkt[2], Pkt[3], Pkt[4], Pkt[5], Pkt[6]);
   printf(", v%d.%dr%d", (Pkt[7]*256)+Pkt[8], Pkt[9], Pkt[10]);
   printf(", IP-%02X:%02X:%02X:%02X"
          , Pkt[11], Pkt[12], Pkt[13], Pkt[14]);
   printf("\r\n");
   for(Ndx = 15; Ndx < len; Ndx++)
   {
      printf("%02X:", Pkt[Ndx]);
   }
   printf("\r\n");
}


pktdsp_h( char* Pkt, int len )
{
// Hello Response
   unsigned int Ndx;
   
   struct tm time;
   gettimeofday( &time, 0 );
   printf("%10d ", time.tm_sec);

   printf("Hello Response");
   printf("");
   printf(", MAC-%02X:%02X:%02X:%02X:%02X:%02X"
          , Pkt[1], Pkt[2], Pkt[3], Pkt[4], Pkt[5], Pkt[6]);
   printf(", v%d.%dr%d", (Pkt[7]*256)+Pkt[8], Pkt[9], Pkt[10]);
   printf(", IP-%02X:%02X:%02X:%02X"
          , Pkt[11], Pkt[12], Pkt[13], Pkt[14]);
   printf(", BrdID-%d", (Pkt[24]*256)+Pkt[23]);
   printf("\r\n");
   printf("  MaxIs-%d", (Pkt[16]*256)+Pkt[15]);
   printf(", MaxOs-%d", (Pkt[18]*256)+Pkt[17]);
   printf(", Binds-%d", (Pkt[20]*256)+Pkt[19]);
   printf(", MaxEs-%d", (Pkt[22]*256)+Pkt[21]);
   printf("\r\n");

}


pktdsp_I( char* Pkt, int len )
{
   unsigned int Ndx;
   
//   printf("\r\n");
   struct tm time;
   gettimeofday( &time, 0 );
   printf("%10d ", time.tm_sec);

   printf("Input Change");
   printf(", MAC-%02X:%02X:%02X:%02X:%02X:%02X"
          , Pkt[1], Pkt[2], Pkt[3], Pkt[4], Pkt[5], Pkt[6]);
   printf(", IP-%02X:%02X:%02X:%02X"
          , Pkt[11], Pkt[12], Pkt[13], Pkt[14]);
   printf(", ID-%d", (Pkt[12]*256)+Pkt[11]);
   printf(", Crnt-%d", (Pkt[14]*256)+Pkt[13]);
   printf(", Last-%d", (Pkt[16]*256)+Pkt[15]);
   printf("\r\n");
}


pktdsp_unknown( char* Pkt, int len )
{
   unsigned int Ndx;
   
   for(Ndx = 0; Ndx < len; Ndx++)
   {
      printf("%02X ", Pkt[Ndx]);
   }
   printf("\r\nUnknown Packet");
   printf("\r\n");
}