#include "color.h"
#include "myLib.h"
typedef unsigned short u16;

unsigned short* videoBuffer = (u16 *) 0x6000000;

extern player pl;

void setPixel(int r, int c, u16 color)
{
	*(videoBuffer + OFFSET(r, c, 240)) = color; 
}

void drawRect(int r, int c, int width, int height, u16 color)
{
	int endW = c + width;
	int endH = r + height;
	int begW = c;
	int begH = r;
	if (endW > 240) endW = 240;
	if (endH > 150) endH = 150;
	if (begW < 0) begW = 0;
	if (begH < 0) begH = 0;
	for(int i = begW; i < endW; i++)
	{
		for(int j = begH; j < endH; j++)
		{
			setPixel(j, i, color);
		}
	}
}

void drawRectUnbounded(int r, int c, int width, int height, u16 color)
{
	for(int i = c; i < c + width; i++)
	{
		for(int j = r; j < r + height; j++)
		{
			setPixel(j, i, color);
		}
	}
}

void drawHollowRect(int r, int c, int width, int height, u16 color)
{
	int endW = c + width;
	int endH = r + height;
	if (endW > 240) endW = 240;
	if (endH > 150) endH = 150;

	for(int i = c; i < endW; i++)
	{
		setPixel(r, i, color);
	}
	if (r + height < 150)
	{
		for(int i = c; i < endW; i++)
		{
			setPixel(endH - 1, i, color);
		}
	}
	for(int i = r; i < endH; i++)
	{
		setPixel(i, c, color);
	}
	if (c + width < 240)
	{
		for(int i = r; i < endH; i++)
		{
			setPixel(i, endW - 1, color);
		}
	}
	
}

void drawPlayer(int r, int c)
{
	plotLine(c - 1, r - 2, c + 1, r - 2, WHITE);
	plotLine(c - 1, r + 2, c + 1, r + 2, WHITE);
	plotLine(c - 2, r - 1, c - 2, r + 1, WHITE);
	plotLine(c + 2, r - 1, c + 2, r + 1, WHITE);
	if (pl.isInvincible) drawRect(r - 1, c - 1, 3, 3, GREEN);
	else drawRect(r - 1, c - 1, 3, 3, MAGENTA);
	
}

void drawEnemy(int r, int c, int type)
{
	if (type == 1)
	{
		plotLine(c - 1, r - 2, c + 1, r - 2, ORANGE);
		plotLine(c - 1, r + 2, c + 1, r + 2, ORANGE);
		plotLine(c - 2, r - 1, c - 2, r + 1, ORANGE);
		plotLine(c + 2, r - 1, c + 2, r + 1, ORANGE);
		setPixel(r, c, RED);
	} 
	else if (type == 2)
	{
		setPixel(r - 3, c - 3, YELLOW);
		setPixel(r - 3, c - 2, YELLOW);
		setPixel(r - 2, c - 3, YELLOW);
		setPixel(r + 3, c - 3, YELLOW);
		setPixel(r + 3, c - 2, YELLOW);
		setPixel(r + 2, c - 3, YELLOW);
		setPixel(r - 3, c + 3, YELLOW);
		setPixel(r - 3, c + 2, YELLOW);
		setPixel(r - 2, c + 3, YELLOW);
		setPixel(r + 3, c + 3, YELLOW);
		setPixel(r + 3, c + 2, YELLOW);
		setPixel(r + 2, c + 3, YELLOW);
		plotLine(c - 1, r - 2, c + 1, r - 2, ORANGE);
		plotLine(c - 1, r + 2, c + 1, r + 2, ORANGE);
		plotLine(c - 2, r - 1, c - 2, r + 1, ORANGE);
		plotLine(c + 2, r - 1, c + 2, r + 1, ORANGE);
		setPixel(r, c, RED);
	}
	else if (type == 3)
	{
		plotLine(c - 1, r - 2, c + 1, r - 2, ORANGE);
		plotLine(c - 1, r + 2, c + 1, r + 2, ORANGE);
		plotLine(c - 2, r - 1, c - 2, r + 1, ORANGE);
		plotLine(c + 2, r - 1, c + 2, r + 1, ORANGE);
		setPixel(r - 1, c - 1, WHITE);
		setPixel(r + 1, c - 1, WHITE);
		setPixel(r - 1, c + 1, WHITE);
		setPixel(r + 1, c + 1, WHITE);
		setPixel(r, c, RED);
	}
	else if (type == 4)
	{
		setPixel(r, c, GREEN);
		setPixel(r - 1, c, GREEN);
		setPixel(r + 1, c, GREEN);
		setPixel(r, c - 1, GREEN);
		setPixel(r, c + 1, GREEN);
		plotLine(c - 1, r - 3, c + 1, r - 3, BLUE);
		plotLine(c - 1, r + 3, c + 1, r + 3, BLUE);
		plotLine(c - 3, r - 1, c - 3, r + 1, BLUE);
		plotLine(c + 3, r - 1, c + 3, r + 1, BLUE);
		setPixel(r - 2, c - 2, BLUE);
		setPixel(r - 2, c + 2, BLUE);
		setPixel(r + 2, c - 2, BLUE);
		setPixel(r + 2, c + 2, BLUE);
	}
	else if (type == 5)
	{
		setPixel(r, c, WHITE);
		setPixel(r - 1, c - 1, RED);
		setPixel(r - 1, c + 1, RED);
		setPixel(r + 1, c - 1, RED);
		setPixel(r + 1, c + 1, RED);
		setPixel(r - 2, c, RED);
		setPixel(r - 3, c, RED);
		setPixel(r + 2, c, RED);
		setPixel(r + 3, c, RED);
		setPixel(r, c - 2, RED);
		setPixel(r, c - 3, RED);
		setPixel(r, c + 2, RED);
		setPixel(r, c + 3, RED);
	}
	else
	{
		plotLine(c - 2, r - 4, c + 2, r - 4, PURPLE);
		plotLine(c - 2, r + 4, c + 2, r + 4, PURPLE);
		plotLine(c - 4, r - 2, c - 4, r + 2, PURPLE);
		plotLine(c + 4, r - 2, c + 4, r + 2, PURPLE);
		setPixel(r - 3, c - 3, PURPLE);
		setPixel(r - 3, c + 3, PURPLE);
		setPixel(r + 3, c - 3, PURPLE);
		setPixel(r + 3, c + 3, PURPLE);
		plotLine(c - 1, r - 2, c + 1, r - 2, BLUE);
		plotLine(c - 1, r + 2, c + 1, r + 2, BLUE);
		plotLine(c - 2, r - 1, c - 2, r + 1, BLUE);
		plotLine(c + 2, r - 1, c + 2, r + 1, BLUE);
		setPixel(r, c, WHITE);
	}
}

void drawBullet(int r, int c, int type)
{
	if (type == 1)
	{
		setPixel(r, c, WHITE);
	}
	else if (type == 2)
	{
		drawRect(r - 1, c - 1, 3, 3, WHITE);
		drawHollowRect(r - 2, c - 2, 5, 5, LIGHT_BLUE);
		setPixel(r - 2, c - 2, BLACK);
		setPixel(r - 2, c + 2, BLACK);
		setPixel(r + 2, c - 2, BLACK);
		setPixel(r + 2, c + 2, BLACK);
	}
	else if (type == 3)
	{
		setPixel(r, c, WHITE);
		setPixel(r - 1, c, RED);
		setPixel(r, c - 1, RED);
		setPixel(r + 1, c, RED);
		setPixel(r, c + 1, RED);
	}
	else
	{
		drawRect(r - 2, c - 2, 5, 5, WHITE);
		plotLine(c - 1, r - 3, c + 1, r - 3, PURPLE);
		plotLine(c - 1, r + 3, c + 1, r + 3, PURPLE);
		plotLine(c - 3, r - 1, c - 3, r + 1, PURPLE);
		plotLine(c + 3, r - 1, c + 3, r + 1, PURPLE);
		setPixel(r - 2, c - 2, PURPLE);
		setPixel(r - 2, c + 2, PURPLE);
		setPixel(r + 2, c - 2, PURPLE);
		setPixel(r + 2, c + 2, PURPLE);
	}
}

void plotLine(int x0, int y0, int x1, int y1, u16 color)
{
	if (x0 > x1) 
	{
		int temp = x0;
		x0 = x1;
		x1 = temp;
		temp = y0;
		y0 = y1;
		y1 = temp;
	}
	int c = 1;
	int dy = y1 - y0;
	if (y0 > y1) 
	{
		c = -1;
		dy = y0 - y1;
	}
	int dx = x1 - x0;
	
	if (dx > dy)
	{
		int D = (dy << 1) - dx;
		setPixel(y0, x0, color);
		int y = y0;

		for(int x = x0 + 1; x <= x1; x++)
		{
			if (D > 0)
			{
				y += c;
				setPixel(y, x, color);
				D += (dy << 1) - (dx << 1);
			}
			else
			{
				setPixel(y, x, color);
				D += (dy << 1);
			}
		}
	}
	else 
	{
		if (y0 > y1) 
		{
			int temp = x0;
			x0 = x1;
			x1 = temp;
			temp = y0;
			y0 = y1;
			y1 = temp;
		}
		c = 1;
		if (x0 > x1) c = -1;

		int dx = x1 - x0;
		int D = (dx << 1) - dy;
		setPixel(y0, x0, color);
		int x = x0;
		
		for(int y = y0 + 1; y <= y1; y++)
		{
			if (D > 0)
			{
				x += c;
				setPixel(y, x, color);
				D += (dx << 1) - (dy << 1);
			}
			else
			{
				setPixel(y, x, color);
				D += (dx << 1);
			}
		}
	}
}

void waitForVblank()
{
	while(SCANLINECOUNTER > 160);
	while(SCANLINECOUNTER < 160);
}
