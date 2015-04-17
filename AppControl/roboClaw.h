/*
 * RoboClaw.h
 *
 *  Created on: Feb 8, 2014
 *      Author: kerry
  *  Created on: 2014_11_08
 *      Author: Big Catz
 */

#ifndef ROBOCLAW_H_
#define ROBOCLAW_H_
#include <stdbool.h>
/**********************Constants*************************************/
#define NELEMS(x)  (sizeof(x) / sizeof(x[0]))  // length of array
#define ADDRESS 128
#define	MASK 0X7F
#define FOUR 4   //  size of array
#define FIVE 5 // size of array
#define TRUE 1
#define FALSE 0
#define CRC 0
#define SHUTDOWNCHANNELS 0
#define CLOSED -1 // check if port is closed
#define UART1 open("/dev/ttyO1", O_RDWR|O_NOCTTY|O_NDELAY|O_NONBLOCK)
#define UART2 open("/dev/ttyO2", O_RDWR|O_NOCTTY|O_NDELAY|O_NONBLOCK)
#define UART4 open("/dev/ttyO4", O_RDWR|O_NOCTTY|O_NDELAY|O_NONBLOCK)
#define	M1FORWARD 0
#define	M1BACKWARD 1
#define	SETMINMB 2
#define	SETMAXMB 3
#define	M2FORWARD 4
#define	M2BACKWARD 5
#define	M17BIT 6
#define	M27BIT 7
#define	MIXEDFORWARD 8
#define	MIXEDBACKWARD 9
#define	MIXEDRIGHT 10
#define	MIXEDLEFT 11
#define	MIXEDFB 12
#define	MIXEDLR 13
#define	GETM1ENC 16
#define	GETM2ENC 17
#define	GETM1SPEED 18
#define	GETM2SPEED 19
#define	RESETENC 20
#define	GETVERSION 21
#define	GETMBATT 24
#define	GETLBATT 25
#define	SETMINLB 26
#define	SETMAXLB 27
#define	SETM1PID 28
#define	SETM2PID 29
#define	GETM1ISPEED 30
#define	GETM2ISPEED 31
#define	M1DUTY 32
#define	M2DUTY 33
#define	MIXEDDUTY 34
#define	M1SPEED 35
#define	M2SPEED 36
#define	MIXEDSPEED 37
#define	M1SPEEDACCEL 38
#define	M2SPEEDACCEL 39
#define	MIXEDSPEEDACCEL 40
#define	M1SPEEDDIST 41
#define	M2SPEEDDIST 42
#define	MIXEDSPEEDDIST 43
#define	M1SPEEDACCELDIST 44
#define	M2SPEEDACCELDIST 45
#define	MIXEDSPEEDACCELDIST 46
#define	GETBUFFERS 47
#define	GETCURRENTS 49
#define	MIXEDSPEED2ACCEL 50
#define	MIXEDSPEED2ACCELDIST 51
#define	M1DUTYACCEL 52
#define	M2DUTYACCEL 53
#define	MIXEDDUTYACCEL 54
#define	GETERROR 90
#define	WRITENVM 94

/***********************************************************************/
/**********************Function_Prototypes*****************************/
/*********************************************************************/

/* The write_n function writes data to the serial port
 * input: array [e.g ADDRESS, driveMotor, speed, 0] and serial port
 * 		  Note: checksum is calculated in the write_n, so array needs
 * 		  to account for this.
 * output: return none
 * 		   writes to serial
 */
void write_n(unsigned char data[], int serialPort);

/* The read_n functions reads data from the serial port
 * input: address, command, length of array, data array and serial port.
 * 		  Note: data array size may not be declared prior to entry
 * 		  		into the read_n function.
 * output: returns a boolean, [e.g true or false]
 * 		   Note: return type is an int but the value is a boolean
 */
bool read_n(unsigned char address,unsigned char cmd, int length, unsigned char dataArray[], int serialPort);

/* The isSerialOpen function checks if serial port is open
 * input: serial port
 * output: returns a boolean, [e.g true or false] true if port is open
 * 		   and false if it is closed.
 * 		   Note: return type is an int but the value is a boolean
 */
bool isSerialOpen(int serial);

/* The closeSerial function closes the specified serial port
 * input: serial port
 * output: none
 */
void closeSerial(int serial);

/* The function forwordM1 moves motor 1 forward
 * input: speed and serial
 * output: none
 */
void forwardM1(unsigned char speed, int serial);

/* The function backwordM1 moves motor 1 forward
 * input: speed and serial
 * output: none
 */
void backwardM1(unsigned char speed, int serial);

/* The function forwordM2 moves motor 1 forward
 * input: speed and serial
 * output: none
 */
void forwardM2(unsigned char speed, int serial);

/* The function backwordM2 moves motor 2 forward
 * input: speed and serial
 * output: none
 */
void backwardM2(unsigned char speed, int serial);

/* The function forwordBackwardM1 moves motor 1 forward and backward
 * input: speed and serial
 * output: none
 */
void forwardBackwardM1(unsigned char speed, int serial);

/* The function forwordBackward2 moves motor 2 forward
 * input: speed and serial
 * output: none
 */
void forwardBackwardM2(unsigned char speed, int serial);

/* The function forwordMixed
 * input: speed and serial
 * output: none
 */
void forwardMixed(unsigned char speed, int serial);

/* The function backwardMixed
 * input: speed and serial
 * output: none
 */
void backwardMixed(unsigned char speed, int serial);

/* The function turnRightMixed
 * input: speed and serial
 * output: none
 */
void turnRightMixed(unsigned char speed, int serial);

/* The function turnLeftMixed
 * input: speed and serial
 * output: none
 */
void turnLeftMixed(unsigned char speed, int serial);

/* The function forwordBackwardMixed
 * input: speed and serial
 * output: none
 */
void forwardBackwardMixed(unsigned char speed, int serial);

/* The function leftRightMixed
 * input: speed and serial
 * output: none
 */
void leftRightMixed(unsigned char speed, int serial);

/* The getM1Encoder functions reads data from encoder
 * input: encoder, status, serial port
 * output: returns a boolean, [e.g true or false]
 * 		   Note: return type is an int but the value is a boolean
 */
bool getM1Encoder(unsigned char* encoder, unsigned char* status, int serial,
	unsigned char arr[]);

/* The getM2Encoder functions reads data from encoder
 * input: encoder, status, serial port
 * output: returns a boolean, [e.g true or false]
 * 		   Note: return type is an int but the value is a boolean
 */
bool getM2Encoder(unsigned long* enc, unsigned char* status, int serial, unsigned char arr[]);

/* The getM1Speed functions gets speed value
 * input: speed, status, serial port
 * output: returns a boolean, [e.g true or false]
 * 		   Note: return type is an int but the value is a boolean
 */
bool getM1Speed(unsigned long* speed, unsigned char* status, int serial, unsigned char arr[]);

/* The getM2Speed functions gets speed value
 * input: speed, status, serial port
 * output: returns a boolean, [e.g true or false]
 * 		   Note: return type is an int but the value is a boolean
 */
bool getM2Speed(unsigned long* speed, unsigned char* status, int serial, unsigned char arr[]);

/* The resetEncoders function resets all encoders
 * input: serial port
 * output: none
 */
void resetEncoders(int serial);



#endif /* ROBOCLAW_H_ */
