
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

int main() {
	//  Turn on sensors
	//system ("i2cset -y 1 0x6b 0x20 0x0f");	// Turn on gyro
	//system ("i2cset -y 1 0x1d 0x20 0xA7");	// Turn on accelerometer and set to 100Hz (table 36)
	//system ("i2cset 1 0x1d 0x");	// Turn on magnetometer 
	
		system ("echo sensor set");
	//  Configure sensors - this will be decided on later
	// 	things such as turning on the filters (decreasing sensitivity) 

	
	int t = 0;
	while(t <= 5000){
	system ("echo gyro `date` - `i2cdump -y 1 0x6b | head -n 4 | tail -n 1 | cut -d ' ' -f 8,9,10,11,12,13,14,15` >> gyroData.txt");
	//system ("echo acce `date` - `i2cdump -y 1 0x1d | head -n 4 | tail -n 1 | cut -d ' ' -f 10,11,12,13,14,15` >> accelData.txt");
	t++;
	}
}
//  12/1/14 Liz and Frias did all this crap.
//	need to find out what to write to set accelerometer and magnetometer as well 
//	as where cut and to read the bytes for both. 
// 	check and see if this is actually writing to a file.
