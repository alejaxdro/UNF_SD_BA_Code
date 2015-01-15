
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
	system ("echo `date +'%s'` - `i2cdump -y 1 0x6b | head -n 4 | tail -n 1 | cut -d ' ' -f 8,9,10,11,12,13,14,15` >> gyro");
	//system ("echo `date +'%s'` - `i2cdump -y 1 0x1d | head -n 4 | tail -n 1 | cut -d ' ' -f 10,11,12,13,14,15` >> gyro");
}
