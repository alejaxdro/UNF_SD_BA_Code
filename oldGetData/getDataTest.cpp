#include <iostream>
#include <string>
#include <unistd.h>
#include "SimpleGPIO.h"
#include "L3GD20Hgyroscope.h"
#include "EasyDriver.h"
using namespace std;

// ************************************* This program is useless!!! **********************************************


int main(int argc, char *argv[]){

   int I2CBus = 1;
   int I2CAddress = 0x6b;
   
   cout << "Starting L3GD20H I2C sensor state read" << endl;
   char namebuf[MAX_BUS];
  	snprintf(namebuf, sizeof(namebuf), "/dev/i2c-%d", I2CBus);
   int file;
   if((file = open(namebuf, O_RDWR)) < 0)
   {
      cout << "Failed to open L3GD20H Sensor on " << namebuf << " 
      I2C Bus" << endl;
      return(1);
   }
   if(ioctl(file, I2C_SLAVE, I2CAddress) < 0)
   {
      cout << "I2C_SLAVE address " << I2CAddress << " failed..." << endl;
      return(2);
   }
   int numberBytes = L3GD20H_I2C_BUFFER;
   int bytesRead = read(file, this->dataBuffer, numberBytes);
   if(bytesRead == -1)
   {
    	cout << "Failure to read Byte Stream in readFullSensorState()" << endl;
   }
   close(file);
   if (this->dataBuffer[0]!=0x03)
   {
    	cout << "MAJOR FAILURE: DATA WITH L3GD20H HAS LOST SYNC!" << endl;
   }
   cout << "Number of bytes read was " << bytesRead << endl;
   for (int i=0; i<8; i++)
   {
      printf("Byte %02d is 0x%02x\n", i, dataBuffer[i]);
   }
   cout << "Closing L3GD20H I2C sensor state read" << endl;

}// End of main()
