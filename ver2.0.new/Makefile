##
## This file is part of the qingxiao project.
##
## Copyright (C) 2013 3Green Tech Inc.
##

CC = gcc
CXX = g++
CFLAGS := -g -Wall -L/opt/vc/lib -lGLESv2 -ljpeg -I./src/inc
CXXFLAGS := -g
LIBVG = src/lib/libshapes.o src/lib/oglinit.o
XMLFLAGS = -I/usr/local/include/libxml2
XMLLIB = -lxml2
BCMLIB = -l bcm2835
HARDWAREDIR = src/backend/hardware/
OPENVGLIB = -I/opt/vc/include -I/opt/vc/include/interface/vcos/pthreads
OBJ = build/time.o build/crc32.o build/uart.o build/image.o build/motor.o build/core.o build/oled.o build/gatekeeper.o build/frontend.o build/backend.o
all: demo

demo:time image uart motor crc32 core oled gatekeeper frontend backend
	$(CC) $(CFLAGS) $(XMLLIB) -o build/demo $(OBJ) $(LIBVG) $(BCMLIB)

core:
	$(CC) $(CFLAGS) $(XMLLIB) $(XMLFLAGS) -o build/core.o -c src/core/core.c

image: $(HARDWAREDIR)/image/image.c
	$(CC) $(CFLAGS) $(OPENVGLIB) $(XMLLIB) $(XMLFLAGS) -o build/image.o -c $(HARDWAREDIR)/image/image.c

uart:
	$(CC) -o build/uart.o -Wall -c $(HARDWAREDIR)/uart/uart.c

motor:
	$(CC) $(CFLAGS) -o build/motor.o  -c $(HARDWAREDIR)/motor/motor.c -L/usr/local/lib -lbcm2835
time:
	$(CC) -o build/time.o -Wall -c $(HARDWAREDIR)/time/time.c
crc32:
	$(CC) $(CFLAGS) -o build/crc32.o -c src/inc/crc32.c
# network:
# 	$(CC) $(CFLAGS) -o build/network.o -c src/platforms/network/client.c
oled:
	$(CC) $(CFLAGS) -o build/oled.o -c $(HARDWAREDIR)/oled/oled.c
gatekeeper:
	$(CC) $(CFLAGS) -o build/gatekeeper.o -c src/gatekeeper/gatekeeper.c
frontend:
	$(CC) $(CFLAGS) -o build/frontend.o -c src/frontend/frontend.c
backend:
	$(CC) $(CFLAGS) $(XMLFLAGS) -o build/backend.o -c src/backend/backend.c

clean:
	rm build/*


.PHONY: $(PHONY) clean
