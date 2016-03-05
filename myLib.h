
#define REG_DISPCTL *(unsigned short *)0x4000000
extern unsigned short *videoBuffer;

#define BG2_ENABLE (1<<10)
#define MODE3 3

#define BUTTONS *(volatile unsigned int *)0x4000130
#define BUTTON_A (1<<0)
#define BUTTON_B (1<<1)
#define BUTTON_SELECT (1<<2)
#define BUTTON_START (1<<3)
#define BUTTON_RIGHT (1<<4)
#define BUTTON_LEFT (1<<5)
#define BUTTON_UP (1<<6)
#define BUTTON_DOWN (1<<7)
#define BUTTON_R (1<<8)
#define BUTTON_L (1<<9)
#define KEY_DOWN_NOW(key)  (key & ~(BUTTONS))

#define SCANLINECOUNTER *(volatile unsigned short *)0x4000006

typedef struct {
	int row;
	int col;
	int hp;
	int charge;
	int isInvincible;
	int hitbox;
} player;

typedef struct {
	int type;
	int isAlive;
	int row;
	int col;
	int oldRow;
	int oldCol;
	int hitbox;
	int moveCooldown;
	int curMoveCooldown;
	int cooldown;
	int curCooldown;
	int bulletType;
	int timer;
	int curTimer;
	int iterations;
} enemy;

typedef struct {
	int type;
	int row;
	int col;
	int oldRow;
	int oldCol;
	int lifespan;
	int delay;
	int curDel;
	int dy;
	int dx;
	int hitbox;
	int bounces;
	int explodes;
} bullet;

enemy enemies[10];
int enemiesLastRemoved[10];
bullet bullets[100];
int bulletsLastRemoved[100];

void setPixel(int r, int c, unsigned short color);
void drawRect(int r, int c, int height, int width, unsigned short color);
void drawRectUnbounded(int r, int c, int height, int width, unsigned short color);
void drawHollowRect(int r, int c, int width, int height, unsigned short color);
void plotLine(int x0, int y0, int x1, int y1, unsigned short color);
void drawPlayer(int r, int c);
void drawEnemy(int r, int c, int type);
void drawBullet(int r, int c, int type);
void waitForVblank();
