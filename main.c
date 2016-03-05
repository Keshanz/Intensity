#include "myLib.h"
#include "color.h"
#include "text.h"
typedef unsigned short u16;

//array that holds the enemy units
extern enemy enemies[10];
//value of the last valid element in the array
int enemiesSize = 0;
//actual number of enemies in the field
int trueEnemiesSize = 0;
//array holding the indices of the enemies that were removed last
extern int enemiesLastRemoved[10];
int enemiesLastRemovedSize = 0;
//similar to the enemy array except for bullets
extern bullet bullets[100];
int bulletsSize = 0;
extern int bulletsLastRemoved[100];
int bulletsLastRemovedSize = 0;
int score = 0;

//the player.  It's a global variable because other files need access to its info
player pl = {80, 120, 15, 400, 0, 2};

void updateEnemies();
void updateBullets();

int main()
{
	REG_DISPCTL = MODE3 | BG2_ENABLE;
	while (1)
	{
		//draw the start screen
		drawRectUnbounded(0, 0, 240, 160, BLACK);
		drawString(70, 93, "INTENSITY", RED);
		drawString(80, 87, "PRESS START", YELLOW);
		//enter next screen when player hits start
		while(!KEY_DOWN_NOW(BUTTON_START));
		//set any and all things to their proper initial values
		score = 0;
		pl.row = 80;
		pl.col = 120;
		pl.hp = 15;
		pl.charge = 400;
		enemiesSize = 0;
		trueEnemiesSize = 0;
		enemiesLastRemovedSize = 0;
		bulletsSize = 0;
		bulletsLastRemovedSize = 0;
		drawRect(0, 0, 240, 160, BLACK);
		int plRad = 2;
		player oldpl = {80, 120, 10, 0, 0, 2};
		int spd = 1;
		int timer = 200;
		int timerNextValue = 400;
		int spawns = 0;
		//draw the general background once
		drawRectUnbounded(150, 0, 240, 20, BLUE);
		drawString(151, 180, "LIVES: ", YELLOW);
		drawString(151, 5, "SCORE: ", YELLOW);
		//the game loop, ends when the players hp hits 0
		while (pl.hp > 0)
		{
			
			if (KEY_DOWN_NOW(BUTTON_B))
			{
				//speed up to a rate of 2 pixels per frame
				spd = 2;
			} else {
				spd = 1;
			}
			if (KEY_DOWN_NOW(BUTTON_A) && pl.charge > 0)
			{
				//turn invincible, reduces the charge meter by a set amount
				pl.isInvincible = 1;
				pl.charge -= 7;
				if (pl.charge < 0) pl.charge = 0;
			}
			else
			{
				pl.isInvincible = 0;
				pl.charge ++;
				if (pl.charge > 400) pl.charge = 400; 
			}
			if (KEY_DOWN_NOW(BUTTON_UP))
			{
				pl.row -= spd;
				if (pl.row < plRad) pl.row = plRad;
			}
			if (KEY_DOWN_NOW(BUTTON_DOWN))
			{
				pl.row += spd;
				if (pl.row > 149 - plRad) pl.row = 149 - plRad;
			}
			if (KEY_DOWN_NOW(BUTTON_LEFT))
			{
				pl.col -= spd;
				if (pl.col < plRad) pl.col = plRad;
			}
			if (KEY_DOWN_NOW(BUTTON_RIGHT))
			{
				pl.col += spd;
				if (pl.col > 239 - plRad) pl.col = 239 - plRad;
			}
			//when timer hits 0, spawn a new enemy
			if (timer <= 0)
			{
				//make the next timer value smaller so enemies spawn progressively faster
				timerNextValue -= 8;
				//also make the timer dependent on the number of enemies on the field
				timer = timerNextValue - (350/(trueEnemiesSize + 1));
				//dont spawn more than 8 since graphics have trouble keeping up
				if (trueEnemiesSize < 8)
				{
					spawns++;
					
					//start by creating enemy location
					int enRow = 0;
					int enCol = 0;
					
					//unfairSpawn represents a bool for a spawn that is too close to the player
					int unfairSpawn = 1;
					while (unfairSpawn != 0)
					{
						//effectively random generation of a position using arbitrary variables
						enRow = (bulletsSize * pl.row + pl.col + spawns * trueEnemiesSize + unfairSpawn + pl.charge) % 130;
						enCol = (bulletsSize + pl.row * pl.col * spawns + trueEnemiesSize + unfairSpawn + pl.charge) % 220;
						enRow += 5;
						enCol += 5;
						int x = enRow - pl.row;
						int y = enCol - pl.col;
						//if spawn position is within 70 pixels of player, repeat the loop, changing the randomness slightly
						if (!(x * x + y * y <= 4900)) unfairSpawn = 0;
						else unfairSpawn += 41;
					}
					//variable to figure out where to place the new enemy
					int toPlace = enemiesSize;
					//first, there is a chance that the last removed entry is now above the last valid entry in the array
					//so lower it down until a valid entry is hit
					while (enemiesLastRemoved[enemiesLastRemovedSize - 1] >= enemiesSize && enemiesLastRemovedSize > 0)
					{
						enemiesLastRemovedSize--;
					}
					//then, if there is an index less than the size of the array that had an enemy removed, place the enemy there
					if (enemiesLastRemovedSize != 0) 
					{
						toPlace = enemiesLastRemoved[enemiesLastRemovedSize - 1];
						enemiesLastRemovedSize--;
					}
					//otherwise, place it at the end and increase enemiesSize
					else
					{
						enemiesSize++;
					}
					
					enemy newEn;
					//psuedoRandom generation of enemy type
					int rand = (pl.row + pl.col * spawns + enemiesSize + pl.charge) % 100;
					if (rand <= (10 + 100/(spawns + 1)))
					{
						enemy en = {1, 1, enRow, enCol, enRow, enCol, 2, 10, 5, 150, (pl.row + pl.col) % 150, 1, 1000, 1000, 0};
						newEn = en;
					} 
					else if (rand <= 30 + 150/(spawns + 1))
					{
						enemy en = {2, 1, enRow, enCol, enRow, enCol, 3, 20, 5, 400, (pl.row + pl.col) % 400, 2, 2000, 2000, 0};
						newEn = en;
					}
					else if (rand <= 50 + 200/(spawns + 1))
					{
						enemy en = {3, 1, enRow, enCol, enRow, enCol, 2, 2, 2, 70, rand % 70, 1, 2000, 2000, 0};
						newEn = en;
					}
					else if (rand <= 70 + 250/(spawns + 1))
					{
						enemy en = {4, 1, enRow, enCol, enRow, enCol, 3, 25, 10, 250, rand % 250, 3, 1000, 1000, 0};
						newEn = en;
					}
					else if (rand <= 90 + 300/(spawns + 1))
					{
						enemy en = {5, 1, enRow, enCol, enRow, enCol, 2, 80, 10, 500, rand % 500, 4, 1000, 1000, 0};
						newEn = en;
					}
					else
					{
						enemy en = {6, 1, enRow, enCol, enRow, enCol, 4, 100, 50, 600, rand % 600, 5, 2000, 2000, 0};
						newEn = en;
					}
					//finally, place enemy in array
					enemies[toPlace] = newEn;
					
					trueEnemiesSize++;
				}
			}
			//method that updates all enemy movement and checks for collisions
			updateEnemies();
			//method that updates all bullet movement and checks for collisions
			updateBullets();
			waitForVblank();
			//first, black out all the places where entities were
			//only care about enemies and bullets if they are active
			drawRect(oldpl.row - plRad, oldpl.col - plRad, 2 * plRad + 1, 2 * plRad + 1, BLACK);
			for(int i = 0; i < bulletsSize; i++)
			{
				if (bullets[i].lifespan > 0) drawRect(bullets[i].oldRow - bullets[i].hitbox - 1, bullets[i].oldCol - bullets[i].hitbox - 1, 3 + bullets[i].hitbox * 2, 3 + bullets[i].hitbox * 2, BLACK);
			}
			for(int i = 0; i < enemiesSize; i++)
			{
				if (enemies[i].isAlive == 1) drawRect(enemies[i].oldRow - enemies[i].hitbox - 1, enemies[i].oldCol - enemies[i].hitbox - 1, enemies[i].hitbox * 2 + 3, enemies[i].hitbox * 2 + 3, BLACK);
			}
			//then, draw their new locations
			drawPlayer(pl.row, pl.col);
			for(int i = 0; i < bulletsSize; i++)
			{
				if (bullets[i].lifespan > 0) drawBullet(bullets[i].row, bullets[i].col, bullets[i].type);
			}
			for(int i = 0; i < enemiesSize; i++)
			{
				if (enemies[i].isAlive == 1) drawEnemy(enemies[i].row, enemies[i].col, enemies[i].type);
			}

			//draw the charge bar at the bottom
			drawRectUnbounded(152, 95, pl.charge / 8, 7, GREEN);
			drawRectUnbounded(152, 95 + pl.charge / 8, 50 - pl.charge / 8, 7, BLUE);

			//draw hp and score
			drawRectUnbounded(151, 215, 20, 10, BLUE);
			drawNum(151, 215, pl.hp, YELLOW);
			drawRectUnbounded(151, 42, 20, 10, BLUE);
			drawNum(151, 42, score, YELLOW);

			
			oldpl = pl;
			timer--;
		}
		//game over screen
		drawRectUnbounded(0, 0, 240, 160, BLACK);
		drawString(70, 93, "GAME OVER", RED);
		drawString(80, 87, "PRESS START", YELLOW);
		while(!KEY_DOWN_NOW(BUTTON_START));
	}
	return 0;
}


