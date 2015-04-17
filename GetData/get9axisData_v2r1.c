/*get9axisData_v2r1.c 

This code has the basic i2c operating functions that opens the bus, reads, 
and writes from specified adresses on each device. 

2/7/15     Elizabeth Voelkel       v2r0  Sending and Receiving data using registers and buffers
2/12/15    Alejandro Lepervanche   v2r1  Added structs/unions and conversions
2/13/15    "        "                     Finished adding conversions to all 3 sensors

	// accel - addr = 0x1d 0x6b gyro is the same 28-2d
	// 0x28-0x2d which are as follows:
	// 28 - XL
	// 29 - XH
	// 2a - YL
	// 2b - YH
	// 2c - ZL
	// 2d - ZH
	
	// mag - addr = 0x1d
	// 0x05-0x0d which are as follows:
	//	05 - TempL
	// 06 - TempH
	// 07 - Status_M
	// 08 - XL
	// 09 - XH
	// 0a - YL
	// 0b - YH
	// 0c - ZL
	// 0d - ZH

*/

#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <stdio.h>
//#include <termios.h>
//#include <math.h>
#include <errno.h>
//#include <time.h>
//#include <string.h>
//#include <unistd.h>
//#include <sys/types.h>
//#include <sys/stat.h>
#include <fcntl.h>

#define DEBUG 1

#if defined(DEBUG) && DEBUG > 0
 #define DEBUG_PRINT(fmt, args...) fprintf(stderr, fmt, ##args)
 //fprintf(stderr, "DEBUG: %s:%d:%s(): " fmt, \
    __FILE__, __LINE__, __func__, ##args)
#else
 #define DEBUG_PRINT(fmt, args...) // Don't do anything in release builds
#endif

// Declare User Functions
void read_sensor( unsigned char data[], unsigned char addr, unsigned char reg);

// Global Declarations
   typedef union{
         short val;
         unsigned char byte[2];
      } SH_CH2;
   typedef struct{
         SH_CH2 x;
         SH_CH2 y;
         SH_CH2 z;
      } SENSOR_XYZ; 
   typedef union{
         SENSOR_XYZ data;
         unsigned char byt[6];
      } SENSOR_BUF;


int main() {
   // Local Declarations
   int i, p;
   unsigned char addr, reg;
   unsigned char data[9];// Read 9 bytes of data
   SENSOR_BUF accel;
   SENSOR_BUF magn;
   SENSOR_BUF gyro;
   
   // Initialize 9-axis Sensors
	DEBUG_PRINT( "running...\n" );
	system( "i2cset -y 1 0x1d 0x20 0xA7" );
	DEBUG_PRINT( "accel_mag on\n" );
	system( "i2cset -y 1 0x6b 0x20 0x0f" ); 
	DEBUG_PRINT( "gyro on\n" );
   
   // Collect Data p times
	for ( p = 0; p < 100; p++ ){
      DEBUG_PRINT( "\n*********************** LOOP **********************\n" );
      
		// Get Acceleration XYZ Data from I2C port
      DEBUG_PRINT( "\n************** ACCELEROMETER DATA ************\n" );
		addr = 0x1d;// accel_mag
		reg = 0x28; // Device register to access accel data
		read_sensor( data , addr, reg ); //send device address and register to start read from (data= 8 bytes read)
		for (i = 0; i < 6; i++ ){
         // Copy xyz data from buffer to struct
         accel.byt[i] = data[i];
			DEBUG_PRINT(":%X", data[i]);		
      }      
      DEBUG_PRINT(":\n");
      // convert from two's complement
      accel.data.x.val = ~accel.data.x.val + 1;
      accel.data.y.val = ~accel.data.y.val + 1;
      accel.data.z.val = ~accel.data.z.val + 1;
      // Display Data
      DEBUG_PRINT(" X val = %d\n Y val = %d\n Z val = %d \n",accel.data.x.val,accel.data.y.val, accel.data.z.val);
      
      
		// Get Magnetometer XYZ Data from I2C port
      DEBUG_PRINT("\n************** MAGNETOMETER DATA *************\n");
		addr = 0x1d;// accel_mag
		reg = 0x05; // Device register to access mag data
		read_sensor( data , addr, reg ); //send device address and register to start read from (data= 8 bytes read)	
		for ( i = 0; i < 6; i++ ){
         // Copy xyz data from buffer to struct
         magn.byt[i] = data[i+3];
         DEBUG_PRINT(":%X", magn.byt[i]);	
		}
      DEBUG_PRINT(":\n");
      // convert from two's complement
      magn.data.x.val = ~magn.data.x.val + 1;
      magn.data.y.val = ~magn.data.y.val + 1;
      magn.data.z.val = ~magn.data.z.val + 1;
      // Display Data
      DEBUG_PRINT(" X val = %d\n Y val = %d\n Z val = %d \n",magn.data.x.val,magn.data.y.val, magn.data.z.val);      
		
      
		// Get Gyroscope XYZ Data from I2C port
      DEBUG_PRINT("\n************** GYROSCOPE DATA *************\n");
		addr = 0x6b;// gyro
		reg = 0x28; // Device register to access gyro data
		read_sensor( data , addr, reg ); //send device address and register to start read from (data= 8 bytes read)	
		for (i = 0; i < 6; i++ ){
         // Copy xyz data from buffer to struct
         gyro.byt[i] = data[i];
			DEBUG_PRINT(":%X", gyro.byt[i]);		
      }      
      DEBUG_PRINT(":\n");
      // convert from two's complement
      gyro.data.x.val = ~gyro.data.x.val + 1;
      gyro.data.y.val = ~gyro.data.y.val + 1;
      gyro.data.z.val = ~gyro.data.z.val + 1;
      // Display Data
      DEBUG_PRINT(" X val = %d\n Y val = %d\n Z val = %d \n",gyro.data.x.val,gyro.data.y.val, gyro.data.z.val); 
      
	}// End for loop
   
}// End main()


void read_sensor( unsigned char data[], unsigned char addr, unsigned char reg) {

	// Opens the i2c bus 1
	int file;
	char filename[40];
	char buf[256] = {0};
	int i = 0;
	
	sprintf(filename,"/dev/i2c-1");
	if ((file = open(filename,O_RDWR)) < 0) {
		printf("Failed to open the bus.");
		// ERROR HANDLING; you can check errno to see what went wrong
		exit(1);
	}
	if (ioctl(file,I2C_SLAVE,addr) < 0) {
		printf("Failed to acquire bus access and/or talk to slave.\n");
		// ERROR HANDLING; you can check errno to see what went wrong 
		exit(1);
	}

	/*writes to i2c device*/
	for (i; i<9; i++){
		buf[0] = reg;
		reg++;
		if (write(file,buf,1) != 1) {
			// ERROR HANDLING: i2c transaction failed
			printf("Failed to write to the i2c bus. Errno = %f\n", errno);
			printf("\n\n");
		}
		if (read(file,buf,1) != 1) {
			// ERROR HANDLING: i2c transaction failed
			printf("Failed to read from the i2c bus. Errno = %f\n", errno);
			printf("\n\n");
		} else {
			data[i]=buf[0];
			//printf("buf %d: %x\n", i, data[i]);
		}
	}
   close(file);
}