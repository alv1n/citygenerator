CFLAGS = -Wall -Wextra
CFILES = main.c generate.c
ALFLAGS = `pkg-config --libs allegro-5 allegro_main-5 allegro_image-5 allegro_primitives-5 allegro_font-5 allegro_ttf-5`

all:
	gcc $(CFLAGS) -o main $(CFILES) $(ALFLAGS) -lm 

clean:
	rm *.o
	rm main
