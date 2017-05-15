#include "definitions.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>


extern void generate2(MenuOption *options)
{
    int rings = options[RINGS].val;
    int size  = options[SIZE].val;
    int walls_n = options[WALLS].val;

    /* Midpoint of city */
    point center;
    center.x = SCREEN_W / 2;
    center.y = SCREEN_H / 2;
    
    /* Allocate for walls */
    point walls[size];
    mainstreet center_streets[size];
    
    generateWalls(center, options, walls, center_streets);
    
    /* Generate CityBlocks */
    CityBlock blocks[rings * walls_n];

    generateCityBlocks(center, center_streets, options, blocks);

    al_clear_to_color(COL_BACKGROUND);

    /* Colour in city */
    drawCityFilling(center, walls, options);

    /* Draw Walls */
    drawWalls(walls, options);
    
}

extern void generateCityBlocks(point center, mainstreet *streets, MenuOption *options, CityBlock *blocks)
{
    int rings = options[RINGS].val;
    int size  = options[SIZE].val;
    int walls_n = options[WALLS].val;

    point list[rings][walls_n];
    for(int i = 0; i < rings; i++)
    {
        for(int j = 0; j < walls_n; j++)
        {
            float length = streets[j].length / (float) (walls_n+1) * j;
            length += rand() % (int) (length * 2 / 3) + (length * 1 / 3);
            list[i][j].x = length * cos(streets[j].slope + 0.2 * j);
            list[i][j].y = length * sin(streets[j].slope + 0.2 * j);
        }
    }
    /*
    for(int i = 0; i < rings; i++)
    {
        for(int j = 0; j < walls_n; j++)
        {
        }
    }
    */
}


extern void generateWalls(point center, MenuOption *options, point *walls, mainstreet *streets)
{
    int n = options[WALLS].val;
    int length = options[SIZE].val;
    for(int i = 0; i < n; i++)
    {
        /* Randomize length */ 
        streets[i].length = length + (rand() % (length / 3 * 2 ));

        /*Get slope */
        streets[i].slope = i * 2.0 * PI / (float) n  + (rand() % 20 / 20);

        /*Store as coordinates */
        walls[i].x = center.x + streets[i].length * cos(streets[i].slope);
        walls[i].y = center.y + streets[i].length * sin(streets[i].slope);
    }
}

extern void drawCityFilling(point center, point *walls, MenuOption *options)
{
    int n = options[WALLS].val; 
    for(int i = 0; i < n; i++)
    {
        al_draw_filled_triangle(center.x, center.y, walls[i].x, walls[i].y, walls[(i+1)%n].x, walls[(i+1)%n].y, COL_BUILDING);
    }
}

extern void drawWalls(point *walls, MenuOption *options)
{
    int n = options[WALLS].val;
    for(int i = 0; i < n; i++)
    {
        al_draw_line(walls[i].x, walls[i].y, walls[(i+1)%n].x, walls[(i+1)%n].y, COL_WALL, 10);
        
        /* Modifier for size */
        int s = (i % (n / 4) ) == 0 ? 0 : 4;
        al_draw_filled_circle(walls[i].x, walls[i].y, 12 + s, COL_WALL);
    }
}


extern void drawLargeStreets(point *largeStreets, MenuOption *options)
{
    int n = options[WALLS].val;
    for(int i = 0; i < n; i++)
    {
        al_draw_line(largeStreets[i].x, largeStreets[i].y, largeStreets[(i+1)%n].x, largeStreets[(i+1)%n].y, COL_CITY, 6);
        al_draw_filled_circle(largeStreets[i].x, largeStreets[i].y, 3,COL_CITY);
    }
}
