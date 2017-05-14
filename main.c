#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "definitions.h"

/*Assumes polygon only has concave corners */
/*void draw_polygon(point *list, int corners);*/

int main(int argc, char *argv[])
{
    if(argc > 1)
    {
        srand(atoi(argv[1]));
    }
    else
    {
        srand(time(NULL));
    }

    /*Initialise allegro*/
    al_init();
    al_init_primitives_addon();
    al_install_keyboard();
    al_init_font_addon();
    al_init_ttf_addon();
    
    /*Anti-aliasing */
    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);

    /*Display to draw to*/
    ALLEGRO_DISPLAY *display = al_create_display(SCREEN_W, SCREEN_H);
    
    /*Creating queue for events, to handle input and closing window*/
    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
    ALLEGRO_EVENT event;

    /* Create a timer to refresh display */
    ALLEGRO_TIMER *timer = al_create_timer(1.0 / 60.0);

    al_register_event_source( queue, al_get_display_event_source(display));
    al_register_event_source( queue, al_get_keyboard_event_source());
    al_register_event_source( queue, al_get_timer_event_source(timer));

    /* Initalize options */
    unsigned int selected = 0;
    MenuOption *options = calloc(OPTION_N, sizeof(MenuOption) );
   
    options[WALLS].val = 10;
    options[WALLS].desc = "No. of walls";
    options[WALLS].offset = 1;
   
    options[RINGS].val = 2;
    options[RINGS].desc = "No of blocks";
    options[RINGS].offset = 1;

    options[SIZE].val = 140;
    options[SIZE].desc = "Size of city";
    options[SIZE].offset = 10;

    options[EXITS].val = 1;
    options[EXITS].desc = "Amount of exits";
    options[EXITS].offset = 1;


    /* TODO Add other options */

    al_start_timer(timer);
    generate2(options);
    while(1)
    {
        al_wait_for_event(queue, &event);
        if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            goto exit; 
        }
        

        if(event.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            if(event.keyboard.keycode == ALLEGRO_KEY_R)
            {
                generate();
            }
            if(event.keyboard.keycode == ALLEGRO_KEY_T)
            {
                generate2(options);
            }

            if(event.keyboard.keycode == ALLEGRO_KEY_E)
            {
                goto exit;
            }
            /*Controls for settings */
            if(event.keyboard.keycode == ALLEGRO_KEY_UP)
            {
            //    selected = selected > 0 ? selected-- : selected;
                selected--;
            }
            if(event.keyboard.keycode == ALLEGRO_KEY_DOWN)
            {
                //selected = selected < OPTION_N ? selected++ : selected; 
                selected++;
            }
            if(event.keyboard.keycode == ALLEGRO_KEY_RIGHT)
            {
                options[selected].val += options[selected].offset;
            }
            if(event.keyboard.keycode == ALLEGRO_KEY_LEFT)
            {
                options[selected].val -= options[selected].offset;
            }
            

            drawGUI(options, selected);
        }
        if(event.type == ALLEGRO_EVENT_TIMER)
        {
            al_flip_display();
            al_start_timer(timer);
        }
    }
exit:
    al_destroy_event_queue(queue);
    al_destroy_display(display);
    return 0;
}

void generate()
{
    al_clear_to_color(COL_BACKGROUND);

    /* City centerpoint */
    point center;
    center.x = SCREEN_W / 2;
    center.y = SCREEN_H / 2;

    /*Outline of city */
    int shape = rand() % 10 + 7;
   
    point wall[shape];
    mainstreet mainstreets[shape];
    /*Generate outline */
    for(int i = 0; i < shape; i++)
    {
        /*Take a random length */
        float length = rand() % 80 + 140;

        /*Divide into nice parts */
        float slope = i * 2.0 * PI / (float) shape + (rand() % 20 / 20); 
        wall[i].x = center.x + length * cos(slope);
        wall[i].y = center.y + length * sin(slope);

        mainstreets[i].length = length;
        mainstreets[i].slope = slope;

    }

    /*Colour in cityblocks */
    for(int i = 0; i < shape; i++)
    { al_draw_filled_triangle(center.x, center.y, wall[i].x, wall[i].y, wall[(i+1)%shape].x, wall[(i+1)%shape].y, COL_BUILDING);
    }


    mainstreet inner_blocks[shape];
    mainstreet outer_blocks[shape];
    int inner_count = 0;
    int outer_count = 0;
    /*Divide city into segments */

    for(int i = 0; i < shape; i++)
    {
        if(rand() % (shape / 6) == 0)
        {
            /*Get a random length in the upper 2/3 of citysegment */
            outer_blocks[outer_count].length = mainstreets[i].length * (rand() % 30 + 50) / 100;
            outer_blocks[outer_count].slope = mainstreets[i].slope;
            
            al_draw_circle(center.x + outer_blocks[outer_count].length * cos(mainstreets[i].slope), center.y + outer_blocks[outer_count].length * sin(mainstreets[i].slope), 3, COL_BUILDING, 1 ); /*Draw a circle to denote place TODO: remove */
            outer_count++;
        }

        /* Smaller chance to have a segment divider in the lower 1/3 */
        if(rand() % (shape / 5) == 0)
        {
            inner_blocks[inner_count].length = mainstreets[i].length * (rand() % 20 + 10) / 100;
            inner_blocks[inner_count].slope = mainstreets[i].slope;

            al_draw_circle(center.x + inner_blocks[inner_count].length * cos(mainstreets[i].slope), center.y + inner_blocks[inner_count].length * sin(mainstreets[i].slope), 3, COL_BUILDING, 1 );
            inner_count++; /* TODO: remove */
        }
    }
    /* Convert segment points into coordinates */
    point point_inner_block[inner_count];
    point point_outer_block[outer_count];
    for(int i = 0; i < inner_count; i++)
    {
        point_inner_block[i].x = center.x + inner_blocks[i].length * cos(inner_blocks[i].slope);
        point_inner_block[i].y = center.y + inner_blocks[i].length * sin(inner_blocks[i].slope);
    }
    for(int i = 0; i < outer_count; i++)
    {
        point_outer_block[i].x = center.x + outer_blocks[i].length * cos(outer_blocks[i].slope);
        point_outer_block[i].y = center.y + outer_blocks[i].length * sin(outer_blocks[i].slope);
    }

    /* Draw main streets */
    for(int i = 0; i < inner_count; i++)
    {
        al_draw_line(point_inner_block[i].x, point_inner_block[i].y, point_inner_block[(i+1)%inner_count].x, point_inner_block[(i+1)%inner_count].y, COL_CITY, 6);
        al_draw_filled_circle(point_inner_block[i].x, point_inner_block[i].y, 3, COL_CITY);
        if(rand() % 5 == 0)
        {
            al_draw_line(point_inner_block[i].x, point_inner_block[i].y, point_outer_block[i].x, point_outer_block[i].y, COL_CITY,6);
        }
    }
    for(int i = 0; i < outer_count; i++)
    {
        al_draw_line(point_outer_block[i].x, point_outer_block[i].y, point_outer_block[(i+1)%outer_count].x, point_outer_block[(i+1)%outer_count].y, COL_CITY, 6);
        al_draw_filled_circle(point_outer_block[i].x, point_outer_block[i].y, 3, COL_CITY);
        if(rand() % 4 == 0)
        {
            al_draw_line(point_outer_block[i].x, point_outer_block[i].y, wall[i].x, wall[i].y, COL_CITY,6);
        }
    }

    /*Draw citywall */
    for(int i = 0; i < shape; i++)
    {
        al_draw_line(wall[i].x, wall[i].y, wall[(i+1)%shape].x, wall[(i+1)%shape].y, COL_WALL, 10);
        if(i % ( shape / 4))
        {
            al_draw_filled_circle(wall[i].x, wall[i].y, 12, COL_WALL);
        }
        else
        {
            al_draw_filled_circle(wall[i].x, wall[i].y, 16, COL_WALL);
        }

    }
}

void drawGUI(MenuOption *options, int selected)
{
    int font_size = 24;
    al_draw_filled_rectangle(0,0,  100,OPTION_N*font_size, COL_BACKGROUND);
    ALLEGRO_FONT *font = al_load_font("BLKCHCRY.TTF", font_size, 0);
    for(unsigned int i = 0; i < OPTION_N; i++)
    {
        if(i == (unsigned int) selected)
        {
            al_draw_line(0, font_size * i, 0, font_size * (i+1), COL_TEXT, 10);
        }
        al_draw_textf( font, COL_TEXT, 11, font_size * i, 0, "%s: %d", options[i].desc, options[i].val);
    }

    al_destroy_font(font);
}
