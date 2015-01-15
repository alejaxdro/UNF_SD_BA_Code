// manualGetData.c
//
// This program is going to get data from sensors through C.  11/17/2014
// This method seems to read the i2c buffer inconsistently... 11/18/2014
// Elizabeth and Alejandro are moving to manualGetDataR1.c    11/19/2014
// Elizabeth and Alejandro are using i2c-dev.h functions to   11/30/2014
//    to access sensors in a more consistent matter.


#include <stdlib.h>
#include <fcntl.h>
#include <fcntl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <termios.h>
#include <math.h>
#include <errno.h>
#include <time.h>


#define I2C1 open("/dev/i2c-1", O_RDWR)

void printMsg( char[], int );

main(){
   
   FILE *data;
	unsigned int size_buffer = 128;
	unsigned int i, dataHandle, bytesSent, bytesRead, sampleNum = 0;
	char buf[size_buffer];   
   
   // I2C bus on pins 19(SCL), 20(SDA)
   int I2CBus = 1;
   
   // L3GD20H Gyroscope I2C Address
   unsigned char I2CAddress = 0x6b;
   
   // Gyroscope Enable Register
   unsigned char enableGyroReg = 0x20;
   unsigned char gyroData      = 0x26;
   
   printf("\n*********** Starting L3GD20H I2C Gyroscope Data Collect ************ \nUsing I2c address: 0x%2x :",I2CAddress);
   
   // Open ports; defined in Sensor_r?.h
   int port_9Ax = I2C1;
   printf(" port_9Ax handle: %d:\n", port_9Ax);
   
   // Open file to collect data from Gyro Sensor
   data = fopen("/root/GetData/data.txt","a");
   
   printf("Gyroscope Data below will be placed in data file handle:%d \n\n", data);   

   //for(sampleNum = 1; sampleNum >= 1; sampleNum++)
   //{
      if(port_9Ax < 0){
      // Error checking for port open
         printf("Failed to open L3GD20H Sensor on /dev/i2c-1 I2C Bus \n\n");
      }else{
         //printf("Open port_9Ax with file handle: %d\n",port_9Ax);
      }
      if(ioctl(port_9Ax, I2C_SLAVE, I2CAddress) < 0)
      {
         printf("I2C_SLAVE address 0x%2x failed...\n\n",I2CAddress);
      }
      
      // Turn on Device **********************************************************
      // Turning on Gyroscope at Address 0x6b by setting Register 0x20 from 0x07 to value 0x0f

      // Create enable Gyro message
      buf[0] = enableGyroReg;
      buf[1] = 0x07;
      
      // *****!!! Make sure to specify number of bytes to prevent overwriting important registers on devices !!!*****
      bytesSent = write(port_9Ax, buf, 2); 
      if (bytesSent < 0) {
         printf("Error writing file: %s\n", strerror(errno));
      }else{
         printMsg( buf, 2 );
      }

      // Create Read Gyro message
      buf[0] = gyroData;

      
      /* Using I2C Read, equivalent of i2c_smbus_read_byte(file) */
      bytesRead = read(port_9Ax, buf, 1); 
      if (bytesRead < 0) {
         printf("Error reading from file: %s\n", strerror(errno));
      }else{
		 write(open("/tmp/a", O_WRONLY | O_APPEND), buf, 10);
	     printMsg( buf, 10);
      }      
		exit(0);

/*      
      bytesRead = read(port_9Ax, dataBuffer, size_buffer);
      if (bytesRead == -1)
      {
         printf("Failure to read Byte Stream in readFullSensorState()\n\n");
      }
      printf("Number of bytes read:%d \n\n",bytesRead);
      printf(":");
      
      // Write sample number and gyro data to data.txt
      fprintf(data,"Sample,%d,",sampleNum);
      for (i=0; i<=128; i++)
      {
         printf("%02x,", dataBuffer[i]);
         bytesWritten = fprintf(data,"%x,",dataBuffer[i]);
         if(i%16==0)printf("\n");
      }
      printf("\n\nNumber of bytes written to data.txt: %d: Errno is %d\n",bytesWritten, errno);
      fprintf(data,"\n");
      printf("\n");
 //  }// End of for(sampleNum)

 */         
   close(port_9Ax);
   close(dataHandle);
   printf("\n************** Closing L3GD20H I2C sensor state read ***************\n");
   
}// End of main()   


// ******************* User Defined Functions ***********************

// printMsg( msg[], size of message ) prints strings/buffers larger than one byte
void printMsg( char msg[], int size ) 
{
   int count;
   // Display Sending 3 Messages
   printf( ": \nMsg:");
   for( count = 0; count < size; count++ )
   {
      printf( "%x", msg[count] );
   }
   printf( ":\n");
}
