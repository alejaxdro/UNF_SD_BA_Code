/*get9axisData_v3r0.c 
  
This code is a main that uses getdata_r0.c library 

2/17/15     Alejandro Lepervanche   v0  Developed getdata_r#.c library for main.
2/18/15     Alejandro Lepervanche   v0  Added North finding, with some error/interference.

*/


#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <stdio.h>
//#include <termios.h>
#include <math.h>
#include <errno.h>
#include <time.h>
//#include <string.h>
//#include <unistd.h>
//#include <sys/types.h>
//#include <sys/stat.h>
#include <fcntl.h>


#define DEBUG 0 // Set 0 to turn off debug prints and Set 1 to turn on

#if defined(DEBUG) && DEBUG > 0
 #define DEBUG_PRINT(fmt, args...) fprintf(stderr, fmt, ##args)
 //fprintf(stderr, "DEBUG: %s:%d:%s(): " fmt, \
    __FILE__, __LINE__, __func__, ##args)
#else
 #define DEBUG_PRINT(fmt, args...) // Don't do anything in release builds
#endif

// User Defined Libraries
#include "getdata_r0.c"
#include "termlib.h"
#include "termlib.c"

// // Global Declarations
   // typedef union{
         // short val;
         // unsigned char byte[2];
      // } SH_CH2;
   // typedef struct{
         // SH_CH2 x;
         // SH_CH2 y;
         // SH_CH2 z;
      // } SENSOR_XYZ; 
   // typedef union{
         // SENSOR_XYZ data;
         // unsigned char byt[6];
      // } SENSOR_BUF;

SENSOR_BUF accel;
SENSOR_BUF magn;
SENSOR_BUF gyro;


int main() {
   
   // Local Declarations
   struct timeval tvstart, tvstop; 
   time_t current_time;
   int i, north = 3100;
   double dot,lenSq1,lenSq2,angle;
   
   // Initialize 9 axis Sensor
   DEBUG_PRINT( "\nInitialize Sensor...\n\n");
   init9axis();
   
   // Start clock to check cpu_time used
   gettimeofday(&tvstart, NULL);
   
   for( i = 0; i < 1000; i++){
      
      // Update Data using getData
      DEBUG_PRINT( "Get Data...\n\n");
      getData();
      
      usleep( 100000 );
      
      clearscreen();
      cursorposition(1,1);
      
      if( magn.data.y.val > north ){
         printf("North is in Range Magnitude of Y: %d\n\n", magn.data.y.val);
      }else{
         printf("North is out of Range: Magnitude of Y: %d\n\n", magn.data.y.val);
         dot = magn.data.x.val*0 + magn.data.y.val*north + magn.data.z.val*0;
         lenSq1 = magn.data.x.val*magn.data.x.val + 
                  magn.data.y.val*magn.data.y.val + 
                  magn.data.z.val*magn.data.z.val;
         lenSq2 = 0*0 + north*north + 0*0;
         angle = acos( dot / sqrt(lenSq1 * lenSq2) )*180/3.14159265359;
         printf("Degree off North: %f\n\n", angle);
      }
      
   }// end loop
   

   // Stop Clock time and save to time spent variable
   gettimeofday(&tvstop, NULL);
   // Display time taken
   printf("Total time = %f seconds\n\n",
         (double) (tvstop.tv_usec - tvstart.tv_usec) / 1000000 +
         (double) (tvstop.tv_sec - tvstart.tv_sec));
   
}// End main()

