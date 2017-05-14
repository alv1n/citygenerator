#ifndef definitions
#define definitions

#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "definitions.h"

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
#define COL_TEXT       al_map_rgb(0x1d, 0x1a, 0x12)

#define OPTION_N (sizeof(enum SETTINGS))

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
    int offset; /*Number to increase or decrease val by */
}MenuOption;

enum SETTINGS
{
    WALLS,
    RINGS,
    SIZE,
    EXITS,
};

void generate(void);

/* New generation */
void generate2(MenuOption *options);
void generateWalls(point center, MenuOption *options, point *walls, mainstreet *streets);

/* Draw stuff */
void drawWalls(point *walls, MenuOption *options);
void drawGUI(MenuOption *options, int selected);
void drawLargeStreets(point *largeStreets, MenuOption *options);

#endif
