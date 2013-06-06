all: Mountains

Mountains: Mountains.c
	gcc Mountains.c -o Mountains -laquaterm -lobjc

clean:
	rm -f Mountains *~
