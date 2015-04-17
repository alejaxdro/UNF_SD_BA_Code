//
// Author:  Alejandro     03/27/2015   
//
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <errno.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <math.h>

// User Defined Libraries
#include "roboClawLib.c"
#include "getdata_r0.c"
#include "termlib.c"


int main (void)
{
   // Local Declarations
   int i,j=10, northpx = 100; 
   short x, y, z;
   double dot,lenSq1,lenSq2,angle;
   
   printf("Testing Angles from 2 axis\n\n");
   
   for(i=-10; i<=10; i++){
      x = i;
      if(x < 0){
         y = j+i;
      }else{
         y = j-i;
      }
      
      dot = x*northpx + y*0;// + z*0;
      lenSq1 = x*x + y*y;// + z*z;
      lenSq2 = 0*0 + northpx*northpx + 0*0;
      angle = acos( dot / sqrt(lenSq1 * lenSq2) )*180/3.14159265359;
      
      printf("X: %d, Y: %d, N_X: %d, N_Y: 0", x, y, northpx, 0);
      printf("  Degree off North: %f\n", angle);
   }
   printf("*********************************************\n");
   for(i=10; i>=-10; i--){
      x = i;
      if(x < 0){
         y = j+i;
      }else{
         y = j-i;
      }
      y = y*-1;
      dot = x*northpx + y*0;// + z*0;
      lenSq1 = x*x + y*y;// + z*z;
      lenSq2 = 0*0 + northpx*northpx + 0*0;
      angle = acos( dot / sqrt(lenSq1 * lenSq2) )*180/3.14159265359;
      
      printf("X: %d, Y: %d, N_X: %d, N_Y: 0", x, y, northpx, 0);
      printf("  Degree off North: %f\n", angle);
   }

   
   if( y < 0){
      angle = -1*angle;
   }
   
	return 0;
}// end main