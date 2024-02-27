CC = gcc
OBJ = main.o src/init.o src/player.o
INCL = -I include -L /usr/local/lib -l SDL2 

src/snake: $(OBJ)
	$(CC) -o bin/snake $(OBJ) $(INCL)
main.o: main.c
	$(CC) -c main.c
init.o: init.c
	$(CC) -c src/init.c
player.o: player.c
	$(CC) -c src/player.c
clean:
	rm $(OBJ) bin/snake
