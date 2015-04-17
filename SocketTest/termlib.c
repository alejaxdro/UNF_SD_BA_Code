// Function Library for Putty Terminal for Boat House UI

#include <stdio.h>

#include "termlib.h"

unsigned int graphics[8];

struct boxpalette
{
	unsigned char topleft[3];
	unsigned char topright[3];
	unsigned char bottomleft[3];
	unsigned char bottomright[3];
	unsigned char horiz[3];
	unsigned char vert[3];
};

struct boxpalette palette1;


// Positions cursor at x column and y line
// 1 Based
void cursorposition( int x, int y )
{
   printf( "\x1B[%d;%dH", y, x );

}// end printposition( x, y )


void setcolor( int fore, int back )
{
	// foreground set from 30d to 37d and background is set from 40d to 47d
	printf( "\x1B[;%d;%dm", fore, back );
}// end setcolor()


void clearscreen()
{
	// Clears Display
	printf( "\x1B[2J" );
}// end clearline()


void clear2eol()
{
	// Clears all characters from cursor position to the end of line
	printf( "\x1B[K" );
}



void setboxpalette()
{
	
	palette1.topleft[0] = 0xE2;
	palette1.topleft[1] = 0x94;
	palette1.topleft[2] = 0x8C;
	palette1.topright[0] = 0xE2;
	palette1.topright[1] = 0x94;
	palette1.topright[2] = 0x90;
	palette1.bottomleft[0] = 0xE2;
	palette1.bottomleft[1] = 0x94;
	palette1.bottomleft[2] = 0x94;
	palette1.bottomright[0] = 0xE2;
	palette1.bottomright[1] = 0x94;
	palette1.bottomright[2] = 0x98;
	palette1.horiz[0] = 0xE2;
	palette1.horiz[1] = 0x94;
	palette1.horiz[2] = 0x80;
	palette1.vert[0] = 0xE2;
	palette1.vert[1] = 0x94;
	palette1.vert[2] = 0x82;
	
//	graphics[0] = 0x00E2948C;
//	txfr.L = graphics[0];
//	putchar( txfr.B[1] );
//	putchar( txfr.B[2] );
//	putchar( txfr.B[3] );

}// end setboxpalette()


void drawbox( int posx, int posy, int length, int height )
{
	//example styles: '╓╖╙╜─║║─' or '┌┐└┘─││─' or '╔╗╚╝═║║═'
	height--;
	int count;
	//putstring( palette1.topleft );
	// top of box
	cursorposition( posx, posy );
	putchar( palette1.topleft[0] );
	putchar( palette1.topleft[1] );
	putchar( palette1.topleft[2] );

	for( count = 0; count < length-2; count++ )
	{		
		putchar( palette1.horiz[0] );
		putchar( palette1.horiz[1] );
		putchar( palette1.horiz[2] );
	}// end for
	putchar( palette1.topright[0] );
	putchar( palette1.topright[1] );
	putchar( palette1.topright[2] );

	// right side of box
	for( count = 0; count < height-1; count++ )
	{
		printf( "\x1B[1B\x1B[1D" );
		putchar( palette1.vert[0] );
		putchar( palette1.vert[1] );
		putchar( palette1.vert[2] );
	}//end for
	printf( "\x1B[1B\x1B[1D" );
	putchar( palette1.bottomright[0] );
	putchar( palette1.bottomright[1] );
	putchar( palette1.bottomright[2] );

	// left side of box
	cursorposition( (posx)+1, (posy) );
	for( count = 0; count < height-1; count++ )
	{
		printf( "\x1B[1B\x1B[1D" );
		putchar( palette1.vert[0] );
		putchar( palette1.vert[1] );
		putchar( palette1.vert[2] );
	}// end for
	printf( "\x1B[1B\x1B[1D" );
	putchar( palette1.bottomleft[0] );
	putchar( palette1.bottomleft[1] );
	putchar( palette1.bottomleft[2] );

	// bottom of box
	for( count = 0; count < length-2; count++ )
	{
		putchar( palette1.horiz[0] );
		putchar( palette1.horiz[1] );
		putchar( palette1.horiz[2] );
	}// end for

}// end drawbox()