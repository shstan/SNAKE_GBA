#include "myLib.h"
#include "text.h"
#include <stdio.h>
// the string drawing library

/** drawChar
 * draw a char
 * @param row row to draw the char
 * @param col column to draw a char
 * @param ch char the char to draw
 * @param color color of the char
 */
void drawChar(int row, int col, char ch, u16 color)
{
	int r,c;
	for(r=0; r<8; r++)
	{
		for(c=0; c<6; c++)
		{
			if(fontdata_6x8[OFFSET(r, c, 6) + ch*48])
			{
				setPixel(r+row, c+col, color);
			}
		}
	}
}



/**  drawString
 * draw a string
 * @param row row to draw the string
 * @param col column to draw the string
 * @param s the char array or starting pointer of the string
 * @param color color to draw the string
 */
void drawString(int row, int col, char *s, u16 color)
{
	while(*s)
	{
		drawChar(row, col, *s++, color);
		col += 6;

	}
}


