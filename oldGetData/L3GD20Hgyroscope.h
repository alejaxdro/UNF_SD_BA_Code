/*
 * L3GD20Hgyroscope.h
 * Definition of a class to interface with the STMicroelectronics L3GD20H 3 Axis Gyroscope
 * over the I2C bus
 *
 * Copyright Derek Molloy, School of Electronic Engineering, Dublin City University
 * www.eeng.dcu.ie/~molloyd/
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED ''AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL I
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef L3GD20HGYROSCOPE_H_
#define L3GD20HGYROSCOPE_H_
#define L3GD20H_I2C_BUFFER 0x80

enum L3GD20H_DATARATE {
	OUTPUTDR_100HZ 		= 0,
	OUTPUTDR_200HZ 		= 1,
	OUTPUTDR_400HZ 		= 2,
	OUTPUTDR_800HZ 		= 3,
};

class L3GD20Hgyroscope {

private:
	int I2CBus, I2CAddress;
	char dataBuffer[L3GD20H_I2C_BUFFER];

	int angularVelocityX;
	int angularVelocityY;
	int angularVelocityZ;

	double pitch;  //in degrees
	double roll;   //in degrees

	float temperature; //accurate to 0.5C
	L3GD20H_DATARATE datarate;

	int  convertAngularVelocity(int msb_addr, int lsb_addr);
	int  writeI2CDeviceByte(char address, char value);
	//char readI2CDeviceByte(char address);
	void calculatePitchAndRoll();

public:
	L3GD20Hgyroscope(int bus, int address);
	void displayMode(int iterations);

	int  readFullSensorState();
	// The following do physical reads and writes of the sensors
	int setDatarate(L3GD20H_DATARATE range);
	L3GD20H_DATARATE getDatarate();
	float getTemperature();
   
	int getAngularVelocityX() { return angularVelocityX; }
	int getAngularVelocityY() { return angularVelocityY; }
	int getAngularVelocityZ() { return angularVelocityZ; }

	float getPitch() { return pitch; }  // in degrees
	float getRoll() { return roll; }  // in degrees

	virtual ~L3GD20Hgyroscope();
};

#endif /* L3GD20HACCELEROMETER_H_ */
