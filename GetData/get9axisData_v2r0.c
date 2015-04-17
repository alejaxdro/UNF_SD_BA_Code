/*get9axisData_v2r0.c 

This code has the basic i2c operating functions that open the bus,
 read, and write from specified adresses on the devices. 
 
2/7/15   Elizabeth Voelkel    v2r0  This code is adapted from elinux.org/Interfacing_with_I2C_Devices and from v1r0

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
	// 0d - ZH*/

#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <termios.h>
#include <math.h>
#include <errno.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void read_sensor( unsigned char data[], unsigned char addr, unsigned char reg);

int main () {
	printf("running...\n");
	system ("i2cset -y 1 0x1d 0x20 0xA7");
	printf("accel_mag on\n");
	system ("i2cset -y 1 0x6b 0x20 0x0f"); 
	printf("gyro on\n");
	int p=0;
	for (p;p<100;p++){
		printf("\n\naccel data\n");
		int i = 0;	
		
		unsigned char addr = 0x1d;// accel_mag
		unsigned char reg = 0x28; // Device register to access accel data
		unsigned char data[9]; 	  // Read 9 bytes of data at a time auto incriment from reg
		unsigned char accel_x;
		read_sensor( data , addr, reg ); //send device address and register to start read from (data= 8 bytes read)
		for (i;i<9;i++){
			printf("Data from addr: %x reg: %x is : %x\n",addr, reg+i, data[i]);
			//accel_x = 25 << & 26;
			//printf("accel_x : %x",accel_x);
			}
			
		addr = 0x1d;// accel_mag
		reg = 0x05; // Device register to access mag data
		i=0;	
		printf("\n\nmag data\n");
		read_sensor( data , addr, reg ); //send device address and register to start read from (data= 8 bytes read)	
		for (i;i<9;i++){
			printf("Data from addr: %x reg: %x is : %x\n",addr, reg+i, data[i]);
			}
			
		addr = 0x6b;// gyro
		reg = 0x28; // Device register to access gyro data
		i=0;	
		printf("\n\ngyro data\n");
		read_sensor( data , addr, reg ); //send device address and register to start read from (data= 8 bytes read)	
		for (i;i<9;i++){
			printf("Data from addr: %x reg: %x is : %x\n",addr, reg+i, data[i]);
			}
		}
	}

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
}