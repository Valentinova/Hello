#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h> 
#include <unistd.h>
#include "VG/openvg.h"
#include "VG/vgu.h"
#include "EGL/egl.h"
#include "GLES/gl.h"
#include "fontinfo.h"
#include "shapes.h"
#include "image.h"

int main(int argc, char **argv)
{
	image_mgr_t image_mgr;
	dev_mgr_t *image_dev;
	dev_req_t req;

	int ret = 0;

	if (argv[1] == NULL) {
		fprintf(stderr, "pls enter the file input!\n");
		return  -1;
	}

	image_dev = image_mgr_init(&image_mgr);

	do {
	
		req.type = IMAGE_WORK_DISPLAY;
		req.buf = (void *)argv[1];
		req.len = 0;

		ret = image_dev->work(image_dev, &req);
		
	} while (0);
	

	return ret;
}
