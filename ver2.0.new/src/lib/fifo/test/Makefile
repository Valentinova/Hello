
CC = gcc
CFLAGS := -g -Wall 
LIB := -lpthread 
OBJ = ../fifo.o ./test.o
all: demo

demo:test fifo
	$(CC) $(CFLAGS) -o ./demo $(OBJ) $(LIB)

test:
	$(CC) $(CFLAGS) -o ./test.o -c ./test.c
fifo:
	$(CC) $(CFLAGS) -o ../fifo.o -c ../fifo.c

clean:
	rm ./test.o ./demo ../fifo.o


.PHONY: $(PHONY) clean
