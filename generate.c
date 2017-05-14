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

    al_clear_to_color(COL_BACKGROUND);
    /* Midpoint of city */
    point center;
    center.x = SCREEN_W / 2;
    center.y = SCREEN_H / 2;
    
    /* Allocate for walls */
    point walls[size];
    mainstreet center_streets[size];
    
    generateWalls(center, options, walls, center_streets);

    /* Draw Walls */
    drawWalls(walls, options);
    
    /* Generate CityBlocks */
    CityBlock blocks[2 * walls_n];

    generateCityBlocks(center, center_streets, options, blocks);

    
}

void generateCityBlocks(point center, mainstreet *streets, MenuOption *options, CityBlock *blocks)
{
    int rings = options[RINGS].val;
    int size  = options[SIZE].val;
    int walls_n = options[WALLS].val;
    
    point inner_ring[walls_n];
    int inner_count = 0;
    point outer_ring[walls_n];
    for(int i = 0; i < walls_n; i++)
    {
        if(rand() % 3 != 0)
        {
            inner_ring[inner_count].x = center.x + streets[i].length * cos(streets[i].slope) / 4;
            inner_ring[inner_count].y = center.y + streets[i].length * sin(streets[i].slope) / 4;

            outer_ring[inner_count].x =center.x + variation + streets[i].length * cos(streets[i].slope) / 4 * 3;
            outer_ring[inner_count].y =center.y + variation + streets[i].length * cos(streets[i].slope) / 4 * 3;
            inner_count++;
        }
    }
    printf("%d\n", inner_count);
    for(int i = 0; i < inner_count; i++)
    {
        blocks[i].a.x = inner_ring[i].x;
        blocks[i].a.y = inner_ring[i].y;

        blocks[i].b.x = inner_ring[(i+1)%inner_count].x;
        blocks[i].b.y = inner_ring[(i+1)%inner_count].y;

        blocks[i].c.x = outer_ring[i].x;
        blocks[i].c.y = outer_ring[i].y;

        blocks[i].d.x = outer_ring[(i+1)%inner_count].x;
        blocks[i].d.y = outer_ring[(i+1)%inner_count].y;

        al_draw_line(blocks[i].a.x, blocks[i].a.y, blocks[i].c.x, blocks[i].c.y, COL_CITY, 5);
        al_draw_line(blocks[i].b.x, blocks[i].b.y, blocks[i].d.x, blocks[i].d.y, COL_CITY, 5);
        al_draw_line(blocks[i].a.x, blocks[i].a.y, blocks[i].b.x, blocks[i].b.y, COL_CITY, 5);
        al_draw_line(blocks[i].c.x, blocks[i].c.y, blocks[i].d.x, blocks[i].d.y, COL_CITY, 5);
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
