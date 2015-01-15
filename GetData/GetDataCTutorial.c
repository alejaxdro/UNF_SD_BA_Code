#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int file;
char *filename = "/dev/i2c-1";
void dumpMsg( unsigned char[], int ); 
unsigned char res;

void main(){

	if ((file = open(filename, O_RDWR)) < 0) {
		/* ERROR HANDLING: you can check errno to see what went wrong */
		perror("Failed to open the i2c bus");
		exit(1);
		} //end if

int addr = 0x6b;     // The I2C address of the device
	if (ioctl(file, I2C_SLAVE, addr) < 0) {
		printf("Failed to acquire bus access and/or talk to slave.\n");
		/* ERROR HANDLING; you can check errno to see what went wrong */
		exit(1);
	}//end if

unsigned char buf[45] = {0};
int i = 0;

	// res = i2c_smbus_read_byte_data(file, reg);
	  // if (res < 0) {
		// /* ERROR HANDLING: i2c transaction failed */
	  // } else {
	  // printf("%2x",res);
		// /* res contains the read word */
	  // }

  for (i = 0; i<1; i++){
		// Using I2C Read
		if (read(file,buf,45) != 45) {
			/* ERROR HANDLING: i2c transaction failed */
			printf("Failed to read from the i2c bus: %s.\n", strerror(errno));
				printf("\n\n");
		} else {
			dumpMsg(buf,sizeof(buf));
			// printf( "buf is:  %2x %2x %2x \n",buf[0],buf[1],buf[2]);
		}//end else
	}//end for
	
	/******* try writing back the same thing thats read remember
	with i2c write a byte read a byte *****/
}//end main

void dumpMsg( unsigned char msg[], int size ) 
{
   int count;
  
   printf( " \nMsg:");
   for( count = 0; count < size; count++ )
   {
      printf( "%2x", msg[count] );
   }
   printf( ":\n");
}//end dumpMsg