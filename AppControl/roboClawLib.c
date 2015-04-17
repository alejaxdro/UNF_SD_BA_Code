/*
 * roboClawLib.c
 *
 *  Created on: Jan 18, 2014
 *      Author: kerry
 */
 
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "roboClaw.h"
#include <stdlib.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>

/**************************************************************************/
/**********************  Function_Definitions  ****************************/
/**************************************************************************/

/**********************  Function_write_n  *****************************/
void write_n(unsigned char data[], int serial){
	//Local Declarations
	int n = NELEMS(data);
	unsigned char checksum = 0;
	int i; // index

	//Statements
	for(i = 0; i < n; i++){
		checksum += data[i];
	}
	data[n-1] = checksum & MASK;
	write(serial, data, sizeof(data));
}

/**********************  Function_read_n  ****************************/
bool read_n(unsigned char address,unsigned char cmd, int length, unsigned char dataArray[],int serial){
	//Local Declarations
	dataArray[length];
	int valid = FALSE;
	int i; // index
	unsigned char checksum = address;
   unsigned char msg[length];
   msg[0] = address;
   msg[1] = cmd;
   msg[2] = (address + cmd) & MASK;
   
	//Statements
	checksum += cmd;
	//write(serial,msg,sizeof(msg));
	write_n(msg,serial);
	unsigned int ccrc = read(serial, msg, sizeof(msg));
    printf("received: %d\n", ccrc);
	for(i = 0; i < length; i++){
		checksum += msg[i];
	} // end for
	printf("checksum   >>> %d\n", checksum);
	if((checksum & MASK) == ccrc){
		valid = TRUE;
	}
    close(serial);
	return valid;
}

/**********************Function_isSerialOpen****************************/
bool isSerialOpen(int serial){
	//Local Declaration
	int valid = FALSE;

	//Statements
	if(serial != CLOSED)
		valid = TRUE;

	return valid;
}

/**********************Function_closeSerial****************************/
void closeSerial(int serial){
	//Statements
	if(isSerialOpen(serial) == TRUE)
		close(serial);
}

/**********************Function_forwardM1****************************/
void forwardM1(unsigned char speed, int serial){
	unsigned char a[FOUR] = {ADDRESS, M1FORWARD, speed,CRC};
	write_n(a, serial);
}

/**********************Function_forwardM2****************************/
void forwardM2(unsigned char speed, int serial){
	unsigned char a[FOUR] = {ADDRESS, M2FORWARD, speed,CRC};
	write_n(a, serial);
}

/**********************Function_backwardM1****************************/
void backwardM1(unsigned char speed, int serial){
	unsigned char a[FOUR] = {ADDRESS, M1BACKWARD, speed,CRC};
	write_n(a, serial);
}

/**********************Function_backwardM2****************************/
void backwardM2(unsigned char speed, int serial){
	unsigned char a[FOUR] = {ADDRESS, M2BACKWARD, speed,CRC};
	write_n(a, serial);
}

/**********************Function_forwardBackwardM1****************************/
void forwardBackwardM1(unsigned char speed, int serial){
	unsigned char a[FOUR] = {ADDRESS, M17BIT, speed,CRC};
	write_n(a, serial);
}

/**********************Function_forwardBackwardM2****************************/
void forwardBackwardM2(unsigned char speed, int serial){
	unsigned char a[FOUR] = {ADDRESS, M27BIT, speed,CRC};
	write_n(a, serial);
}

/**********************Function_forwardMixed****************************/
void forwardMixed(unsigned char speed, int serial){
	unsigned char a[FOUR] = {ADDRESS, MIXEDFORWARD, speed,CRC};
	write_n(a, serial);
}
/**********************Function_backwardMixed****************************/
void backwardMixed(unsigned char speed, int serial){
	unsigned char a[FOUR] = {ADDRESS, MIXEDBACKWARD, speed,CRC};
	write_n(a, serial);
}
/**********************Function_turnRightMixed****************************/
void turnRightMixed(unsigned char speed, int serial){
	unsigned char a[FOUR] = {ADDRESS, MIXEDRIGHT, speed,CRC};
	write_n(a, serial);
}

/**********************Function_turnLeftMixed****************************/
void turnLeftMixed(unsigned char speed, int serial){
	unsigned char a[FOUR] = {ADDRESS, MIXEDLEFT, speed,CRC};
	write_n(a, serial);
}
/**********************Function_forwardBackwardMixed****************************/
void forwardBackwardMixed(unsigned char speed, int serial){
	unsigned char a[FOUR] = {ADDRESS, MIXEDFB, speed,CRC};
	write_n(a, serial);
}
/**********************Function_leftRightMixed****************************/
void leftRightMixed(unsigned char speed, int serial){
	unsigned char a[FOUR] = {ADDRESS, MIXEDLR, speed,CRC};
	write_n(a, serial);
}

/**********************Function_getM1Encoder****************************/
bool getM1Encoder(unsigned char* enc, unsigned char* status, int serial, unsigned char arr[]){
	if(read_n(ADDRESS,GETM1ENC,FIVE,arr, serial)){
		*enc = (arr[0] << 24 | arr[1] << 16 | arr[2] << 8 | arr[3]);
		*status = arr[FOUR];
		return TRUE;
	} // end if
	*enc = 0;
	*status = 0;
	return FALSE;
}

/**********************Function_getM2Encoder****************************/
bool getM2Encoder(unsigned long* enc, unsigned char* status, int serial, unsigned char arr[]){
	if(read_n(ADDRESS,GETM2ENC,FIVE,arr, serial)){
		*enc = (arr[0] << 24 | arr[1] << 16 | arr[2] << 8 | arr[3]);
		*status = arr[FOUR];
		return TRUE;
	} // end if
	*enc = 0;
	*status = 0;
	return FALSE;
}

/**********************Function_getM1Speed****************************/
bool getM1Speed(unsigned long* speed, unsigned char* status, int serial, unsigned char arr[]){
	if(read_n(ADDRESS,GETM1SPEED,FIVE,arr, serial)){
		*speed = (arr[0] << 24 | arr[1] << 16 | arr[2] << 8 | arr[3]);
		*status = arr[FOUR];
		return TRUE;
	} // end if
	*speed = 0;
	*status = 0;
	return FALSE;
}

/**********************Function_getM2Speed****************************/
bool getM2Speed(unsigned long* speed, unsigned char* status, int serial, unsigned char arr[]){
	if(read_n(ADDRESS,GETM2SPEED,FIVE,arr, serial)){
		*speed = (arr[0] << 24 | arr[1] << 16 | arr[2] << 8 | arr[3]);
		*status = arr[FOUR];
		return TRUE;
	} // end if
	*speed = 0;
	*status = 0;
	return FALSE;
}

/**********************Function_resetEncoders****************************/
void resetEncoders(int serial){
	unsigned char reset[FOUR];
	reset[0] = ADDRESS;
	reset[1] = RESETENC;
	reset[2] = SHUTDOWNCHANNELS;
	write_n(reset, serial);
}
