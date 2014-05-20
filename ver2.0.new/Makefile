##
## This file is part of the qingxiao project.
##
## Copyright (C) 2013 3Green Tech Inc.
##

CC = gcc
CFLAGS := -g -Wall -L/opt/vc/lib -lGLESv2 -ljpeg 
LIBVG = src/lib/libshapes.o src/lib/oglinit.o
XMLFLAGS = -I/usr/local/include/libxml2
XMLLIB = -lxml2
BCMLIB = -l bcm2835
HARDWAREDIR = src/backend/hardware/
OPENVGLIB = -I/opt/vc/include -I/opt/vc/include/interface/vcos/pthreads
OBJ = build/fifo.o build/mytimer.o build/crc32.o build/uart.o build/oled.o build/image.o build/motor.o build/entry.o build/gatekeeper.o build/frontend.o build/backend.o
## -L/opt/vc/lib -L/usr/local/lib
all: demo

demo:fifo mytimer crc32 image uart oled motor entry gatekeeper frontend backend
	$(CC) $(CFLAGS) $(XMLLIB) -o build/demo $(OBJ) $(LIBVG) $(BCMLIB)

entry:
	$(CC) $(CFLAGS) $(OPENVGLIB) $(XMLLIB) $(XMLFLAGS) -o build/entry.o -c src/entry.c

mytimer:
	$(CC) $(CFLAGS) -o build/mytimer.o -Wall -c src/lib/timer/mytimer.c

fifo:
	$(CC) $(CFLAGS) -o build/fifo.o -Wall -c src/lib/fifo/fifo.c

crc32:
	$(CC) $(CFLAGS) -o build/crc32.o -c src/lib/crc32/crc32.c

image:
	$(CC) $(CFLAGS) $(OPENVGLIB) $(XMLLIB) $(XMLFLAGS) -o build/image.o -c $(HARDWAREDIR)/image/image.c

uart:
	$(CC) -o build/uart.o -Wall -c $(HARDWAREDIR)/uart/uart.c

motor:
	$(CC) $(CFLAGS) -o build/motor.o  -c $(HARDWAREDIR)/motor/motor.c -L/usr/local/lib -lbcm2835
		
oled:
	$(CC) $(CFLAGS) -o build/oled.o -c $(HARDWAREDIR)/oled/oled.c

gatekeeper:
	$(CC) $(CFLAGS) $(OPENVGLIB) $(XMLLIB) $(XMLFLAGS) -o build/gatekeeper.o -c src/gatekeeper/gatekeeper.c
	
frontend:
	$(CC) $(CFLAGS) $(OPENVGLIB) $(XMLLIB) $(XMLFLAGS) -o build/frontend.o -c src/frontend/frontend.c

backend:
	$(CC) $(CFLAGS) $(OPENVGLIB) $(XMLLIB) $(XMLFLAGS) -o build/backend.o -c src/backend/backend.c

clean:
	rm build/*

.PHONY: $(PHONY) clean
