#include "definitions.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>


extern void generate2(MenuOption *options)
{

    al_clear_to_color(COL_BACKGROUND);
    /* Midpoint of city */
    point center;
    center.x = SCREEN_W / 2;
    center.y = SCREEN_H / 2;
    
    /* Allocate for walls */
    //point *walls = malloc(options[SIZE].val * sizeof(point) );
    //mainstreet *center_streets = malloc(options[SIZE].val * sizeof(mainstreet) );
    int size = options[SIZE].val;
    point walls[size];
    mainstreet center_streets[size];
    
    generateWalls(center, options, walls, center_streets);

    /* Draw Walls */
    drawWalls(walls, options);
    
}

extern void generateWalls(point center, MenuOption *options, point *walls, mainstreet *streets)
{
    int n = options[WALLS].val;
    int length = options[SIZE].val;
    for(int i = 0; i < n; i++)
    {
        /* Randomize length */ 
        streets[i].length = length + (rand() % (length / 2));

        /*Get slope */
        streets[i].slope = i * 2.0 * PI / (float) n ;

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
