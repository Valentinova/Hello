#ifndef __IMAGE_H__
#define __IMAGE_H__

#include <jpeglib.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h> 
#include <unistd.h>
#include "VG/openvg.h"
#include "VG/vgu.h"
#include "EGL/egl.h"
#include "GLES/gl.h"
#include "fontinfo.h"
#include "shapes.h"

#define BIAS_X 430
#define BIAS_Y 270
#define SCALE 10.0f
#define BIAS  1.0f
#define XGA_W 1024
#define XGA_H 768
#define IMAGE_WORK_DISPLAY	0x1 
#define CURETIME	2000
#define BLACKTIME	200

void image_init();
void image_end();
void image_fresh();

extern xmlNodePtr image_checkvalid(char *file_path, xmlNodePtr root);
extern void draw_layers(xmlNode * node,int index);

void imagetest(int w, int h);
void Draw_raster(int defi_x, int defi_y, int bold); //for printer debug
void image_fresh_color(unsigned int R, unsigned int G, unsigned int B);
void Image_fresh_layer(int layer);

#endif
