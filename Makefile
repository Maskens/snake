CC = gcc
OBJ = main.o src/init.o

snake: $(OBJ)
	$(CC) -o snake $(OBJ) -I include -L /usr/local/lib -l SDL2 -l SDL2_image

main.o: main.c
	$(CC) -c main.c
init.o: init.c
	$(CC) -c src/init.c
clean:
	rm $(OBJ)
