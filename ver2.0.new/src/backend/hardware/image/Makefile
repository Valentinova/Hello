#all: shapedemo hellovg mouse-hellovg
OBJECTS = image.o

image.o: image.c libshapes.o oglinit.o
	gcc   -Wall -I/opt/vc/include -I/opt/vc/include/interface/vcos/pthreads -I.. -I../ -I../../inc/ -c image.c libshapes.o oglinit.o -L/opt/vc/lib -lGLESv2 -ljpeg `xml2-config --cflags`  `xml2-config --libs`

testimage: test_image.c image.o libshapes.o oglinit.o
	gcc   -Wall -I/opt/vc/include -I/opt/vc/include/interface/vcos/pthreads -I.. -I../ -I../../inc/ -o testimage test_image.c image.o libshapes.o oglinit.o -L/opt/vc/lib -lGLESv2 -ljpeg `xml2-config --cflags`  `xml2-config --libs`

clean:
	rm testimage $(OBJECTS)
