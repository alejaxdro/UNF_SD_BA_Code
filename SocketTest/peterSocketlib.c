// lr.c
// Lights Receive Test

#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define BUFSIZE 2048
#define LOCAL_PORT 8081
#define REMOTE_PORT 8081

int rc;
unsigned int rcv_cnt;

clock_t timestamp;
 
int main(int argc, char **argv) 
{
   struct sockaddr_in myaddr;  /* our address */ 
   struct sockaddr_in remaddr; /* remote address */ 
   socklen_t addrlen = sizeof(remaddr); /* length of addresses */ 
   int recvlen;             /* # bytes received */ 
   int fd;                 /* our socket */ 
   unsigned char buf[BUFSIZE]; /* receive buffer */ 
   
   // create UDP socket
   fd = socket(AF_INET, SOCK_DGRAM, 0);
   if (fd < 0) 
   { 
      perror("cannot create socket\n"); 
      return 0; 
   } 
   // bind the socket to any valid IP address and a specific port
   memset((char *)&myaddr, 0, sizeof(myaddr)); 
   myaddr.sin_family = AF_INET; 
   myaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
   myaddr.sin_port = htons(LOCAL_PORT); 
   rc = bind(fd, (struct sockaddr *)&myaddr, sizeof(myaddr));
   if ( rc < 0 ) 
   { 
      perror("bind failed"); 
      return 0; 
   } 
   
   // Setting Option to Allow Sending to IPV4 Broadcast Address
   int broadcast = 1;
   rc = setsockopt(fd,SOL_SOCKET,SO_BROADCAST,&broadcast,sizeof(broadcast)); 
   if (rc == -1) 
   {
		fprintf(stderr, "setsockopt() failed\n");
		return 0;
   }
   
   // Remote Address
	char *server = "255.255.255.255";	// Broadcast
   memset((char *) &remaddr, 0, sizeof(remaddr));
	remaddr.sin_family = AF_INET;
	remaddr.sin_port = htons(REMOTE_PORT);
   rc = inet_aton(server, &remaddr.sin_addr);
	if ( rc == 0 ) 
   {
		fprintf(stderr, "inet_aton() failed\n");
		return 0;
	}
   
   buf[0] = 'H';
   buf[1] = 0;
   
   rc = sendto(fd, buf, strlen(buf), 0, (struct sockaddr *)&remaddr, sizeof(remaddr));
   if ( rc < 0 ) 
   { 
      perror("sendto failed"); 
      return 0; 
   } 
/*
   buf[0] = 'G';
   buf[1] = 'b';
   buf[2] = 0x1;
   buf[3] = 0x0;
   buf[4] = 0;
   
   rc = sendto(fd, buf, 5, 0, (struct sockaddr *)&remaddr, sizeof(remaddr));
   if ( rc < 0 ) 
   { 
      perror("sendto failed"); 
      return 0; 
   } 
*/
   printf("\r\n");

   // Forever Loop
   for (;;) 
   { 
      recvlen = recvfrom(fd, buf, BUFSIZE, 0, (struct sockaddr *)&remaddr, &addrlen); 
      if (recvlen > 0) 
      { 
         rcv_cnt++;
         buf[recvlen] = 0; 
         
         CtrlPktParse( &buf, recvlen );
         
         
         
         
         
         
      } 
      
   }

//   close(fd);

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