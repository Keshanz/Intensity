#include "color.h"
#include "myLib.h"



extern enemy enemies[10];
extern int enemiesSize;
extern int trueEnemiesSize;
extern int enemiesLastRemoved[10];
extern int enemiesLastRemovedSize;
extern bullet bullets[100];
extern int bulletsSize;
extern int bulletsLastRemoved[100];
extern int bulletsLastRemovedSize;
extern player pl;
extern int score;



void updateEnemies();
void updateBullets();
void removeBullet();
int checkCollision();

//update all enemy movements and check for collision
void updateEnemies()
{
	for (int i = 0; i < enemiesSize; i++)
	{
		//only do things if the enemy is active
		if (enemies[i].isAlive == 1)
		{
			//set enemy to a pointer for ease of access
			enemy * en = enemies + i;
			//set oldrow and oldCol
			en->oldRow = en->row;
			en->oldCol = en->col;
			//check for collision with players
			if (checkCollision(en->row, en->col, en->hitbox))
			{
				//enemy dies if collision is true
				en->isAlive = 0;
				score++;
				trueEnemiesSize--;
				//decrease until next valid enemy if removing last one in array
				if (enemiesSize - 1 == i) 
				{
					int j = i;
					while (enemies[j].isAlive <= 0 && j >= 0)
					{
						enemiesSize--;
						j--;
					} 
				}
				else
				{
					enemiesLastRemoved[enemiesLastRemovedSize] = i;
					enemiesLastRemovedSize++;
				}
				//draw a rectangle to erase the enemy
				drawRect(en->row - 4, en->col - 4, 9, 9, BLACK);
			}
			else
			{
				//if the cooldown for the bullets is 0, create a bullet
				if (en->curCooldown == 0)
				{
					en->curCooldown = en->cooldown;
					//variables for enemies that shoot multiple bullets
					int totalBullets = 1;
					int j = 0;
					while (j < totalBullets)
					{
						//similar to the toPlace in main for enemies
						int toPlace = bulletsSize;
						
						while (bulletsLastRemoved[bulletsLastRemovedSize - 1] >= bulletsSize && bulletsLastRemovedSize > 0)
						{
							bulletsLastRemovedSize--;
						}
						if (bulletsLastRemovedSize != 0)
						{
							toPlace = bulletsLastRemoved[bulletsLastRemovedSize - 1];
							bulletsLastRemovedSize--;
						}
						else
						{
							bulletsSize++;
						}
						bullet b;
						//use the distance between player and enemy for calculations
						int dy = pl.row - en->row;
						int dx = pl.col - en->col;
						//to avoid floats, multiple by 100 each
						dy *= 100;
						dx *= 100;
						
						//give different properties based on the bulletType for each enemy
						if (en->bulletType == 1)
						{	
							//way of avoiding square roots
							while (dx * dx + dy * dy > 90000)
							{
								dx = (dx * 4) / 5;
								dy = (dy * 4) / 5;
							}
							//return to normal value
							dx /= 100;
							dy /= 100;
							bullet temp = {1, en->row, en->col, en->row, en->col, 1000, 1, 0, dy, dx, 0, 0, 0};
							b = temp;
						}
						else if (en->bulletType == 2)
						{
							totalBullets = 5;
							while (dx * dx + dy * dy > 40000)
							{
								dx = (dx * 4) / 5;
								dy = (dy * 4) / 5;
							}
							dx /= 100;
							dy /= 100;
							bullet temp = {1,  en->row, en->col, en->row, en->col, 1000, 0, 0, dy, dx, 0, 0, 0};
							b = temp;
							//code for the multishot bullets
							if (j == 0) b.dy = b.dy - 1;
							else if (j == 1) b.dy = b.dy + 1;
							else if (j == 2) b.dx = b.dx - 1;
							else if (j == 3) b.dx = b.dx + 1;
							if (b.dy == 0 && b.dx == 0) 
							{
								j++;
								continue;
							}
						}
						else if (en->bulletType == 3)
						{
							while (dx * dx + dy * dy > 160000)
							{
								dx = (dx * 4) / 5;
								dy = (dy * 4) / 5;
							}
							dx /= 100;
							dy /= 100;
							bullet temp = {2,  en->row, en->col, en->row, en->col, 2000, 1, 0, dy, dx, 1, 2, 0};
							if (en->iterations >= 2) temp.bounces += (en->iterations - 1);
							b = temp;
						}
						else if (en->bulletType == 4)
						{
							while (dx * dx + dy * dy > 250000)
							{
								dx = (dx * 4) / 5;
								dy = (dy * 4) / 5;
							}
							dx /= 100;
							dy /= 100;
							bullet temp = {3,  en->row, en->col, en->row, en->col, 1000, 0, 0, dy, dx, 1, 0, 0};
							b = temp;
						}
						else if (en->bulletType == 5)
						{
							while (dx * dx + dy * dy > 160000)
							{
								dx = (dx * 4) / 5;
								dy = (dy * 4) / 5;
							}
							dx /= 100;
							dy /= 100;
							bullet temp = {4,  en->row, en->col, en->row, en->col, 1000, 1, 0, dy, dx, 3, 0, 1};
							b = temp;
						}
						else
						{
							bullet temp = {1,  en->row, en->col, en->row, en->col, 1000, 1, 0, dy, dx, 0, 0, 0};
							b = temp;
						}
						bullets[toPlace] = b;
						
						j++;
					}
				}
				en->curCooldown--;
				//if enemy can move, move
				if (en->curMoveCooldown == 0)
				{
					en->curMoveCooldown = en->moveCooldown;
					int dy = pl.row - en->row;
					int dx = pl.col - en->col;
					dy *= 100;
					dx *= 100;
					while (dx * dx + dy * dy > 40000)
					{
						dx = (dx * 4) / 5;
						dy = (dy * 4) / 5;
					}
					dx /= 100;
					dy /= 100;
					en->row += dy;
					en->col += dx;
				}
				en->curMoveCooldown--;
			}
			//if the curTimer is 0, increase the difficulty of the enemy
			if (en->curTimer == 0)
			{
				en->iterations++;
				en->curTimer = en->timer;
				en->cooldown = en->cooldown * 8 / 10;
				en->moveCooldown = en->moveCooldown * 8 / 10;
				switch (en->type)
				{
					case 1:
						if (en->iterations >= 3) en->bulletType = 2;
						else if (en->iterations >= 5) en->bulletType = 3;
						break;
					case 3:
						if (en->iterations >= 3) en->bulletType = 2;
						break;
					case 5:
						en->cooldown = en->cooldown * 8 / 10;
						break;
					default:
						break;
				}
			}
			en->curTimer--;;
		}
	}
}

//update all bullets and check for collisions
void updateBullets()
{
	for(int i = 0; i < bulletsSize; i++)
	{
		
		if (bullets[i].lifespan > 0 )
		{
			bullet * bull = bullets + i;
			bull->lifespan--;
			//check for collisions
			if (checkCollision(bull->row, bull->col, bull->hitbox))
			{
				//only reduce health if player isnt invincible, remove bullet either way
				if (!(pl.isInvincible)) pl.hp--;
				removeBullet(bullets, i);
			}
			//only move if the curdelay is 0
			else if (bull->curDel == 0)
			{
				bull->curDel = bull->delay;
				bull->oldRow = bull->row;
				bull->oldCol = bull->col;
				bull->row += bull->dy;
				bull->col += bull->dx;
				//rad is a way of finding out when to delete the bullet as it gets closer to the edges
				//it is a combination of the bullets size and its movement speed
				int rad = bull->hitbox;
				int x = bull->dx;
				if (x < 0) x = -x;
				int y = bull->dy;
				if (y < 0) y = -y;
				if (x > y) rad += x + 1;
				else rad += y + 1;
				//remove when too close to the edge
				if (bull->row > 149 - rad || bull->row < rad || bull->col > 239 - rad || bull->col < rad)
				{
					//do a final collision check 
					if (checkCollision(bull->row, bull->col, bull->hitbox))
					{
						if (!(pl.isInvincible)) pl.hp--;
					}
					//if the bullet explodes, do the explosion code
					if(bull->explodes)
					{
						//only one bullet type explodes so the code is the same
						//explodes into three bouncing bullets
						int j = 1;
						while (j > -2)
						{
							int toPlace = bulletsSize;
											
							while (bulletsLastRemoved[bulletsLastRemovedSize - 1] >= bulletsSize && bulletsLastRemovedSize > 0)
							{
								bulletsLastRemovedSize--;
							}
							if (bulletsLastRemovedSize != 0)
							{
								toPlace = bulletsLastRemoved[bulletsLastRemovedSize - 1];
								bulletsLastRemovedSize--;
							}
							else
							{
								bulletsSize++;
							}
							bullet b = {2, bull->row, bull->col, bull->row, bull->col, 2000, 1, 0, bull->dy, bull->dx, 1, 1, 0};
							if (bull->row < rad || bull->row > 149 - rad)
							{
								b.dy = -b.dy;
								b.dx += j;
							}
							else if (bull->col < rad || bull->col > 239 - rad) 
							{
								b.dx = -b.dx;
								b.dy += j;
							}
							bullets[toPlace] = b;
							j--;
						}
					}
					//bullet is not removed if it has bounces, instead reverse the direction it came from
					if (bull->bounces == 0)
					{
						removeBullet(bullets, i);
					}
					else
					{
						if (bull->row > 149 - rad || bull->row < rad)
						{
							bull->dy = -bull->dy;
						}
						if (bull->col > 239 - rad || bull->col < rad)
						{
							bull->dx = -bull->dx;
						}
						bull->bounces--;
					}
				}
			} 
			else bull->curDel--;
			
		}
	}
}

//check for collision between two rectangles
int checkCollision(int row, int col, int rad)
{
	return (pl.col - pl.hitbox <= col + rad && pl.col + pl.hitbox >= col - rad && pl.row - pl.hitbox <= row + rad && pl.row + pl.hitbox >= row - rad);
}

//removes a bullet from bullets and updates bulletsSize, bulletsLastRemoved, and bulletsLastRemovedSize
void removeBullet(bullet* bullets, int i)
{
	bullets[i].lifespan = 0; 
					
	if(bulletsSize - 1 == i) 
	{
		int j = i;
		while (bullets[j].lifespan <= 0 && j >= 0)
		{
			bulletsSize--;
			j--;
		} 
	}
	else
	{
		bulletsLastRemoved[bulletsLastRemovedSize] = i;
		bulletsLastRemovedSize++;
	}
	//draw rectangles to remove the bullet completely from the screen
	drawRect(bullets[i].row - 1 - bullets[i].hitbox, bullets[i].col - 1 - bullets[i].hitbox, 3 + 2 * bullets[i].hitbox, 3 + 2 * bullets[i].hitbox, BLACK);
	drawRect(bullets[i].oldRow - 1 - bullets[i].hitbox, bullets[i].oldCol - 1 - bullets[i].hitbox, 3 + 2 * bullets[i].hitbox, 3 + 2 * bullets[i].hitbox, BLACK);
	
}
