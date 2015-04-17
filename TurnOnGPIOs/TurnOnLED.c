/**********************************************
*
* Author: Elizabeth Voelkel 3/19/2015 -- included iolib.h into /usr/lib 
*                                        included libiofunc.a into /usr/include
*Library source: http://www.element14.com/community/community/designcenter/single-board-computers/next-gen_beaglebone/blog/2013/10/10/bbb--beaglebone-black-io-library-for-c                       
*
* This turns on a LED from a gpio using the iolib blinks slow or fast based on switch input
*
*************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <iolib.h>


main(void){
int del;
iolib_init(); //initialize the iolib
iolib_setdir(8,11, DIR_IN); //sets P8 pin 11 direction
iolib_setdir(8,12, DIR_OUT); //sets P8 pin 12 direction

 while(1){  
        if (is_high(8,11)){  
            del=100; // fast speed  
        }  
        if (is_low(8,11)){  
            del=500; // slow speed  
        }  
  
        pin_high(8,12);  
        iolib_delay_ms(del);  
        pin_low(8,12);  
        iolib_delay_ms(del);  
  
    }  
  
    iolib_free();  //returns 
  
    return(0);  
}  