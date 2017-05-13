CFLAGS = -Wall -Wextra

ALFLAGS = `pkg-config --libs allegro-5 allegro_main-5 allegro_image-5 allegro_primitives-5`

all:
	gcc $(CFLAGS) -o main main.c $(ALFLAGS) -lm

clean:
	rm *.o
	rm main
