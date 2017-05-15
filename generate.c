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

    printf("%d\n", walls->x);
    generateCityBlocks(center, center_streets, walls, options, blocks);

    al_clear_to_color(COL_BACKGROUND);

    /* Colour in city */
    drawCityFilling(center, walls, options);

    drawCityBlocks(blocks, rings*walls_n);

    /* Draw Walls */
    drawWalls(walls, options);
    
}

extern void drawCityBlocks(CityBlock *blocks, int n)
{
    for(int i = 0; i < n; i++)
    {
        al_draw_line(blocks[i].a.x, blocks[i].a.y, blocks[i].b.x, blocks[i].b.y, COL_CITY, 6);
        al_draw_line(blocks[i].c.x, blocks[i].c.y, blocks[i].d.x, blocks[i].d.y, COL_CITY, 6);
        al_draw_line(blocks[i].a.x, blocks[i].a.y, blocks[i].c.x, blocks[i].c.y, COL_CITY, 6);
        al_draw_line(blocks[i].b.x, blocks[i].b.y, blocks[i].d.x, blocks[i].d.y, COL_CITY, 6);
    }
}

extern void generateCityBlocks(point center, mainstreet *streets, point *walls, MenuOption *options, CityBlock *blocks)
{
    int rings = options[RINGS].val;
    int size  = options[SIZE].val;
    int walls_n = options[WALLS].val;

    /*Generate correct amount of walls */
    point list[rings][walls_n];
    for(int i = 0; i < rings; i++)
    {
        for(int j = 0; j < walls_n; j++)
        {
            int len =  j *  streets[j].length / (walls_n+1); + (rand() % ( (int) ((float) len * 2.0 / 3.0)) + (len * 1 / 3));
            list[i][j].x = center.x + len * cos(streets[j].slope );
            list[i][j].y = center.y + len * sin(streets[j].slope );
        }
    }

    
    for(int i = 0; i < rings; i++)
    {
        if(i == rings - 1)
        {
            for(int j = 0; j < walls_n; j++)
            {
                blocks[i*walls_n+j].a.x = list[i][j].x;
                blocks[i*walls_n+j].a.y = list[i][j].y;

                blocks[i*walls_n+j].b.x = list[i][(j+1)%walls_n].x;
                blocks[i*walls_n+j].b.y = list[i][(j+1)%walls_n].y;

                blocks[i*walls_n+j].b.x = walls[j].x; 
                blocks[i*walls_n+j].b.y = walls[j].y;

                blocks[i*walls_n+j].b.x = walls[(j+1)%walls_n].x;
                blocks[i*walls_n+j].b.y = walls[(j+1)%walls_n].y;
            }
        }
        else
        {
            for(int j = 0; j < walls_n; j++)
            {
                blocks[i*walls_n+j].a.x = list[i][j].x;
                blocks[i*walls_n+j].a.y = list[i][j].y;

                blocks[i*walls_n+j].b.x = list[i][(j+1)%walls_n].x;
                blocks[i*walls_n+j].b.y = list[i][(j+1)%walls_n].y;

                blocks[i*walls_n+j].b.x = list[i+1][j].x;
                blocks[i*walls_n+j].b.y = list[i+1][j].y;

                blocks[i*walls_n+j].b.x = list[i+1][(j+1)%walls_n].x;
                blocks[i*walls_n+j].b.y = list[i+1][(j+1)%walls_n].y;
            }
        }
    }
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
