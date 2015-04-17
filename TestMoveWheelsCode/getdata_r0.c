/*get9axisData_v3r0.c 
  
This code has the basic i2c operating functions that opens the bus, reads, 
and writes from specified adresses on each device. 

2/7/15     Elizabeth Voelkel       v2r0  Sending and Receiving data using registers and buffers
2/12/15    Alejandro Lepervanche   v2r1  Added structs/unions and conversions
2/13/15    "        "                    Finished adding conversions to all 3 sensors
2/15/15    Elizabeth Voelkel       v3r0  Got Magnetometer working; HP Filter to Normal Mode
           Alejandro Lepervanche         Adding timing/clock time for data collection
2/15/15    "         " 	           v4r0  Writing Data to a file for matlab

	// accel - addr = 0x1d 0x6b gyro is the same 28-2d
	// 0x28-0x2d which are as follows:
	// 28 - XL	// 29 - XH
	// 2a - YL	// 2b - YH
	// 2c - ZL	// 2d - ZH
	
	// mag - addr = 0x1d
	// 0x05-0x0d which are as follows:
	//	05 - TempL	// 06 - TempH
	// 07 - Status_M
	// 08 - XL	// 09 - XH
	// 0a - YL	// 0b - YH
	// 0c - ZL	// 0d - ZH

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

#define DEBUG 0

#if defined(DEBUG) && DEBUG > 0
 #define DEBUG_PRINT(fmt, args...) fprintf(stderr, fmt, ##args)
 //fprintf(stderr, "DEBUG: %s:%d:%s(): " fmt, \
    __FILE__, __LINE__, __func__, ##args)
#else
 #define DEBUG_PRINT(fmt, args...) // Don't do anything in release builds
#endif

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

SENSOR_BUF accel;
SENSOR_BUF magn;
SENSOR_BUF gyro;

// Declare User Functions
void read_sensor( unsigned char data[], unsigned char addr, unsigned char reg);
void getData( void );
void displayprompt( void );
double calcNorthOffset( int north, short x, short y, short z );


void init9axis(){

   // Initialize 9-axis Sensors
	system( "i2cset -y 1 0x1d 0x20 0x57" ); // ctrl1	
   system( "i2cset -y 1 0x1d 0x21 0x20" ); // ctrl2
	system( "i2cset -y 1 0x1d 0x24 0xF0" ); // ctrl5 Enables Temp and Sets magn data rate to 50Hz
	system( "i2cset -y 1 0x1d 0x25 0x20" ); // ctrl6
	system( "i2cset -y 1 0x1d 0x26 0x80" ); // ctrl7 Normal Mode
	DEBUG_PRINT( "accel_mag on\n" );
	system( "i2cset -y 1 0x6b 0x20 0x0f" ); // ctrl1
	DEBUG_PRINT( "gyro on\n" );
     
}// end init9axis

double calcNorthOffset( int northAx, short x, short y, short z ){
   // Local Declarations
   double dot,lenSq1,lenSq2,angle;
   
   DEBUG_PRINT("North is out of Range: Magnitude of X: %d\n\n", x);
   dot = x*northAx + y*0;// + z*0;
   lenSq1 = x*x + y*y;// + z*z;
   lenSq2 = 0*0 + northAx*northAx + 0*0;
   angle = acos( dot / sqrt(lenSq1 * lenSq2) )*180/3.14159265359;
   DEBUG_PRINT("Degree off North: %f\n\n", angle);
   
   if( y < 0){
      angle = -1*angle;
   }
   
   return angle;
}

int calculateTurnRate( int maxNorth, int turn, int commandAngle ) {
   // Calculate Angle offset from North
   double angle = calcNorthOffset( maxNorth, magn.data.x.val, magn.data.y.val, magn.data.z.val );   
   angle += commandAngle;
   DEBUG_PRINT("Angle off North: %f\n", angle);
   // Find Angle from north to set turn
   int dspeed = 2;
   // Decide which way to turn
   if( angle > 20 ) {
      // Increase turn
      DEBUG_PRINT( "\nIncrease Turn: Right?\n\n");
      return dspeed;
   }else if( angle < -20 ){
      // Decrease turn
      DEBUG_PRINT( "\nDecrease Turn: Right?\n\n");
      return -dspeed;
   }else{
      return -turn;
   }
}

void getData(){
   // Local Declarations
   int i, p;
   unsigned char addr, reg;
   unsigned char data[9];// Read 9 bytes of data
   
   // Collect Data p times
	for ( p = 0; p < 1; p++ ){
      DEBUG_PRINT( "\n*********************** LOOP **********************\n" );

      // Get Acceleration XYZ Data from I2C port
      DEBUG_PRINT( "\n************** ACCELEROMETER DATA ************\n" );
		addr = 0x1d;// accel_mag
		reg = 0x28; // Device register to access accel data
		read_sensor( data , addr, reg ); //send device address and register to start read from (data= 8 bytes read)
		for (i = 0; i < 6; i++ ){
         // Copy xyz data from buffer to struct
         accel.byt[i] = data[i];
			//DEBUG_PRINT(":%X", accel.byt[i]);		
      }
      //DEBUG_PRINT(":\n");
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
         //DEBUG_PRINT(":%X", magn.byt[i]);	
		}
      //DEBUG_PRINT(":\n");
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
			//DEBUG_PRINT(":%X", gyro.byt[i]);		
      }      
      //DEBUG_PRINT(":\n");
      // convert from two's complement
      gyro.data.x.val = ~gyro.data.x.val + 1;
      gyro.data.y.val = ~gyro.data.y.val + 1;
      gyro.data.z.val = ~gyro.data.z.val + 1;
      // Display Data
      DEBUG_PRINT(" X val = %d\n Y val = %d\n Z val = %d \n",gyro.data.x.val,gyro.data.y.val, gyro.data.z.val); 
      
      
	}// End for loop
      
}// End getData()

int SpinRobotAndFindMaxNorth ( int Serial ){
   
	unsigned char data[FOUR];
	int i, turn, speed, stop = 64;
   int maxMagnNorth = 0;
   
   // Initialize 9 axis Sensor
//   DEBUG_PRINT( "\nInitialize Sensor...\n");
//   init9axis();
   
   // Moving 2 wheels from one UART  
   //usleep(2000000);  // Delay (in micro seconds) for user to walk to watch the motor moving
   
   // Set speed = 0 to 63
   turn = 15;
   
   // Accelerate Turn to turn
   for(i = 0; i <= ( turn ); i++){
      // Turns 1/2 turn
      forwardBackwardM1( stop + i, Serial );
      forwardBackwardM2( stop - i, Serial );      
      // Acceleration delay
      usleep( 40000 );
      // Get North Measurement and compare and save
      DEBUG_PRINT( "Get Magnetometer Data... Sample %d\n", i);
      getData();
      if( magn.data.x.val > maxMagnNorth){
         maxMagnNorth = magn.data.x.val;
         DEBUG_PRINT( "Max Magnitude is %d\n", maxMagnNorth);
      }
   }// end for loop
   
   // Collect Magnetic measurements while turning in place

   for(i = 0; i <= 100; i++){   
      // Acceleration delay
      usleep( 10000 );
      // Get North Measurement and compare and save
      DEBUG_PRINT( "Get Magnetometer Data... Sample %d\n", i);
      getData();
      if( magn.data.x.val > maxMagnNorth){
         maxMagnNorth = magn.data.x.val;
         DEBUG_PRINT( "Max Magnitude is %d\n", maxMagnNorth);
      }
   }// end for loop
   
   // Deccelerate
   for(i = turn; i >= 0; i--){
      // Turns
      forwardBackwardM1( stop + i, Serial );
      forwardBackwardM2( stop - i, Serial );
      // Acceleration delay
      usleep( 20000 );   
   }// end for loop
	
   // Stop Motors
	forwardBackwardM1( stop, Serial );
	forwardBackwardM2( stop, Serial );
   
	return maxMagnNorth;
}// end 

void read_sensor( unsigned char data[], unsigned char addr, unsigned char reg) {
	/* opens the i2c bus 1 */
	int file;
	char filename[40];
	char buf[256] = {0};
	int i = 0;
	
	sprintf(filename,"/dev/i2c-1");
	if ((file = open(filename,O_RDWR)) < 0) {
		printf("Failed to open the bus.");
		/* ERROR HANDLING; you can check errno to see what went wrong */
		exit(1);
	}
	if (ioctl(file,I2C_SLAVE,addr) < 0) {
		printf("Failed to acquire bus access and/or talk to slave.\n");
		/* ERROR HANDLING; you can check errno to see what went wrong */
		exit(1);
	}
   
	/*writes to i2c device*/
	for (i; i<9; i++){
		buf[0] = reg;
		reg++;
		if (write(file,buf,1) != 1) {
			/* ERROR HANDLING: i2c transaction failed */
			printf("Failed to write to the i2c bus. Errno = %f\n", errno);
			printf("\n\n");
      }
      if (read(file,buf,1) != 1) {
         /* ERROR HANDLING: i2c transaction failed */
         printf("Failed to read from the i2c bus. Errno = %f\n", errno);         
         printf("\n\n");
      } else {
         data[i]=buf[0];
         //DEBUG_PRINT("buf %d: %x\n", i, data[i]);
      }
   }
   close(file);
}

void displayprompt(){
      printf("********************* Control Blue Armadillo Prototype *********************\n");
      printf("**                                                                        **\n");
      printf("**     Select one of the following:                                       **\n");
      printf("**             1. Type w and hit enter to add speed                       **\n");
      printf("**             2. Type s and hit enter to subtract speed                  **\n");
      printf("**             3. Type a and hit enter to add to turn left                **\n");
      printf("**             4. Type d and hit enter to add to turn right               **\n");
      printf("**                                                                        **\n");
      printf("**             5. Type r and hit enter to reset turn coefficient          **\n");
      printf("**             6. Type q and hit enter to stop motors                     **\n");
      printf("**                                                                        **\n");
      printf("**             7. Type z and hit enter to end program                     **\n");
      printf("**                                                                        **\n");
      printf("****************************************************************************\n");
      printf("-->");

}// end displayprompt()

