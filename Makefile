all: Mountains

Mountains: Mountains.c clouds.c
	gcc Mountains.c clouds.c -o Mountains -laquaterm -lobjc

clean:
	rm -f Mountains *~
