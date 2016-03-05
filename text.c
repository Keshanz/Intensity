#include "text.h"
#include "color.h"
#include "myLib.h"

void drawChar(int row, int col, char ch, unsigned short color)
{
	int r, c;
	for(r=0; r<8; r++)
	{
		for(c=0; c<6; c++)
		{
			if(fontdata_6x8[OFFSET(r, c, 6)+ch*48])
			{
				setPixel(row+r, col+c, color);
			}
		}
	}
}


void drawString(int row, int col, char *str, unsigned short color)
{
	while(*str)
	{
		drawChar(row, col, *str++, color);
		col +=6;

	}
}

//draws a number, couldve used a library but didnt know about it at the time of making
void drawNum(int row, int col, int num, unsigned short color)
{
	if (num == 0)
	{
		drawChar(row, col, (char) (48), color);
	}
	int length = 0;
	int temp = num;
	while(temp != 0)
	{
		length++;
		temp /= 10;
	}
	col = col + 6 * (length - 1);
	char ch;
	while(num != 0)
	{
		ch = (char)((num % 10) + 48);
		drawChar(row, col, ch, color);
		col -= 6;
		num /= 10;
	}
}