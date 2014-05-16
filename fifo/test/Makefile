
CC = gcc
CFLAGS := -g -Wall 
LIB := 
OBJ = ../fifo.o ./test.o
all: demo

demo:test fifo
	$(CC) $(CFLAGS) -o ./demo $(OBJ) 

test:
	$(CC) $(CFLAGS) $(LIB) -o ./test.o -c ./test.c
fifo:
	$(CC) $(CFLAGS) -o ../fifo.o -c ../fifo.c

clean:
	rm ./test.o ./demo ../fifo.o


.PHONY: $(PHONY) clean
