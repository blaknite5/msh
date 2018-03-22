CC=gcc
CFLAGS=-I.
DEPS= getinput.h
OBJ= main.o getinput.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
msh: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)

clean: 
	rm $(OBJ) msh
