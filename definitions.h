#ifndef definitions
#define definitions

#define min(a,b) (a<b?a:b)
#define max(a,b) (a>b?a:b)

#define PI 3.14159265 
#define SCREEN_W 640
#define SCREEN_H 480

#define COL_BACKGROUND al_map_rgb(0xE2, 0xD2, 0xB8)
#define COL_CITY       al_map_rgb(0xB8, 0xA9, 0x95)
#define COL_WALL       al_map_rgb(0x5E, 0x56, 0x44)
#define COL_BUILDLINE  al_map_rgb(0x2E, 0x2E, 0x2E)
#define COL_BUILDING   al_map_rgb(0x70, 0x6D, 0x6D)

typedef struct
{
    float x;
    float y;
}point;

typedef struct
{
    int length;
    float slope;
}mainstreet;

typedef struct
{
    float x1;
    float y1;
    float x2;
    float y2;
    float x3;
    float y3;
    ALLEGRO_COLOR col;
    float thickness; /* If zero, assume filled triangle */
}ShapeTriangle;

typedef struct
{
    int val;
    const char *desc;
    int offset;
}MenuOption;

enum SETTINGS
{
    BORDERS,
    RINGS,
    SIZE,
    EXITS,
};

void generate(void);

/* New generation */


#endif
