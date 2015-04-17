
#include <stdio.h>
#include <stdlib.h>
#include "iolib.h"
#include "iolib.c"


int main(void)
{
	int del;
	iolib_init();
	printf("iolib initialized");
   iolib_setdir(8,11, DIR_IN);
	iolib_setdir(8,12, DIR_OUT);
	
	while(1)
	{	
		pin_high(8,12);
      printf("on\n");
      usleep(1000000);
      pin_low(8,12);
      printf("off\n");
      usleep (1000000); 
	}
	
	iolib_free();
	
	return(0);
}

