#define COLOR(r, g, b)  ((r) | (g)<<5 | (b)<<10)
#define RED COLOR(31,0,0)
#define GREEN COLOR(0, 31,0)
#define BLUE COLOR(0, 0, 31)
#define CYAN COLOR(0, 31, 31)
#define MAGENTA COLOR(31, 0, 31)
#define YELLOW COLOR(31, 31, 0)
#define WHITE COLOR(31, 31, 31)
#define ORANGE COLOR(31, 16, 0)
#define LIGHT_BLUE COLOR(20, 25, 27)
#define PURPLE COLOR(17, 6, 29)
#define BLACK 0

#define OFFSET(r, c, rowlen)  ((r)*(rowlen)+(c))