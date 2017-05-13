#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#define PI 3.14159265 
#define SCREEN_W 640
#define SCREEN_H 480
const float FPS  = 60;

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

void generate(void);

/*Assumes polygon only has concave corners */
void draw_polygon(point *list, int corners);

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

    al_init();
    al_init_primitives_addon();
    al_install_keyboard();
    
    //Anti-aliasing
    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);

    ALLEGRO_DISPLAY *display = al_create_display(SCREEN_W, SCREEN_H);
    
    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
    ALLEGRO_EVENT event;

    al_register_event_source( queue, al_get_display_event_source(display));
    al_register_event_source( queue, al_get_keyboard_event_source());

    generate();
    while(1)
    {
        al_flip_display();
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
            if(event.keyboard.keycode == ALLEGRO_KEY_E)
            {
                goto exit;
            }
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
    printf("%d\n", shape);
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

    /*Colour in city  */
    for(int i = 0; i < shape; i++)
    {
        al_draw_filled_triangle(center.x, center.y, wall[i].x, wall[i].y, wall[(i+1)%shape].x, wall[(i+1)%shape].y, COL_CITY); 
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
            
            al_draw_circle(center.x + outer_blocks[outer_count].length * cos(mainstreets[i].slope), center.y + outer_blocks[outer_count].length * sin(mainstreets[i].slope), 3, COL_BUILDING, 1 ); //Draw a circle to denote place TODO: remove 
            outer_count++;

            /* Smaller chance to have a segment divider in the lower 1/3 */
        }
        if(rand() % (shape / 3) == 0)
        {
            inner_blocks[inner_count].length = mainstreets[i].length * (rand() % 20 + 10) / 100;
            inner_blocks[inner_count].slope = mainstreets[i].slope;
            al_draw_circle(center.x + inner_blocks[inner_count].length * cos(mainstreets[i].slope), center.y + inner_blocks[inner_count].length * sin(mainstreets[i].slope), 3, COL_BUILDING, 1 );
            inner_count++;
        }
    }
}
