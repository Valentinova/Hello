
CC = gcc
CFLAGS := -g -Wall 
LIB := -lrt
OBJ = ../mytimer.o ./test.o
all: demo

demo:test mytimer 
	$(CC) $(CFLAGS) -o ./demo $(OBJ) $(LIB)

test:
	$(CC) $(CFLAGS) -o ./test.o -c ./test.c
mytimer:
	$(CC) $(CFLAGS) -o ../mytimer.o -c ../mytimer.c

clean:
	rm ./test.o ./demo ../mytimer.o


.PHONY: $(PHONY) clean
