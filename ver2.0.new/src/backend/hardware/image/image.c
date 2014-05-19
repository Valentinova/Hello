#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "image.h"

int width = XGA_W;
int height = XGA_H;

xmlNodePtr image_checkvalid(char *file_path, xmlNodePtr root);
void draw_layers(xmlNode * node,int index);
void Rect_fill(VGfloat x, VGfloat y, VGfloat w, VGfloat h); 

static void draw_polygon(char *buf)
{
	char px[10], py[10];
	VGint i = 0;
	char *s;

	VGubyte *cmd = malloc(10000 * sizeof(VGubyte));
	VGfloat *points = malloc(20000 * sizeof(VGfloat));

	VGPath path = vgCreatePath(VG_PATH_FORMAT_STANDARD, 
				VG_PATH_DATATYPE_F, 
				SCALE, 
				BIAS, 
				200, 
				200, 
				VG_PATH_CAPABILITY_ALL);
	cmd[0] = VG_MOVE_TO_ABS;
	s = strtok(buf," ");
	while(s != NULL) {
		sscanf(s,"%[^','],%s",px, py);
		cmd[i+1] = VG_LINE_TO_ABS;
		points[2*i] = atof(px) + 20;
		points[2*i+1] = atof(py) + 20;
		s = strtok(NULL," ");
		i++;
	}

	vgAppendPathData(path, i, cmd,points);

	Fill(255,255,255,1);

	vgDrawPath(path,VG_FILL_PATH);

	vgDestroyPath(path);

	free(cmd);
	free(points);
}

void image_end()
{
	restoreterm();
	finish();
}

void image_init()
{

	saveterm();
	init(&width, &height);                  // Graphics initialization
//	rawterm();
    Start(width, height);                   // Start the picture
    Background(0, 0, 0);                    // Black background
//    Fill(44, 77, 232, 1);                   // Big blue marble
    End();                                  // End the picture
//      finish();                               // Graphics cleanup
}

void image_fresh()
{
    Start(width, height);                   // Start the picture
    Background(0, 0, 0);                    // Black background
//    Fill(44, 77, 232, 1);                   // Big blue marble
    End();                                  // End the picture
	
//	rawterm();
}

void image_fresh_color(unsigned int R, unsigned int G, unsigned int B)
{
	Start(width, height);                   // Start the picture
    	Background(R, G, B);                    // Black background
	//    Fill(44, 77, 232, 1);                   // Big blue marble
   	 End();    
}

void Image_fresh_layer(int layer)
{
	static xmlNodePtr  tempNode1 = NULL;
	xmlNodePtr  rootNode1 = NULL;
	
	if(0 == file_check){tempNode1 = image_checkvalid("test.xml",rootNode1); file_check = 1;}else{printf("xml file checked already \n");}

	if(layer < xmlChildElementCount(tempNode1) && layer >= 0)
		{
		draw_layers(tempNode1, layer);
		}else{
		printf("err: exceed  layers range \n ");
		}
}

// Rect makes a rectangle at the specified location and dimensions
void Rect_fill(VGfloat x, VGfloat y, VGfloat w, VGfloat h) {
	VGPath path = newpath();
	vguRect(path, x, y, w, h);
	Fill(255,0,0,1);
	vgDrawPath(path, VG_FILL_PATH | VG_STROKE_PATH);
	vgDestroyPath(path);
}

void Draw_raster(int defi_x, int defi_y, int bold) //for printer debug
{
	int i;
	printf("Draw raster!\n");
	Start(width,height);
	Background(0, 0, 0);
	for( i=0; i<=defi_x;i++){
		Rect_fill((int)(i*width/defi_x - bold/2), 0, bold, height);
		}
	for(i=0; i<=defi_y;i++){
		Rect_fill(0,(int)(i*height/defi_y - bold/2), width, bold);
		}
	End();
}

void imagetest(int w, int h) 
{
	int imgw = w, imgh = h;
	VGfloat cx = 0, cy = 0;
	Start(w, h);
	Background(0, 0, 0);
	Image(cx, cy, imgw, imgh, "jpegtest.jpg");
	End();
}

/*
 * print_element_names:
 * @a_node: the initial xml node to consider.
 *
 * Prints the names of the all the xml elements
 * that are siblings or children of a given xml node.
 */
void
draw_layers(xmlNode * node,int index)
{
	xmlNode *cur_node = NULL,*polynode = NULL;
    xmlChar *id,*points;

    char * token;

    printf("index = %d",index);
    for (cur_node = node->children; cur_node; cur_node = cur_node->next) {

	    if (cur_node->type == XML_ELEMENT_NODE) {
			if ((!xmlStrcmp(cur_node->name, (const xmlChar *)"g"))) {
				id = xmlGetProp(cur_node, (const xmlChar *)"id");

				token=strtok((char*)id,"layer");

				if( index == atoi(token) ){
				printf("layer = %s\n",token);
					/*Graphics initialization*/
				/* init(&width, &height); */
				/* Start the image */
				Start(width, height);
				/* Black background */
				Background(0, 0, 0);
				for(polynode = cur_node->children; polynode; polynode = polynode->next)
				{
				if ((!xmlStrcmp(polynode->name, (const xmlChar *)"polygon"))) {
				points = xmlGetProp(polynode,(const xmlChar *)"points");
				draw_polygon((char*)points);

				/* Stop the drawing after the last polygon of one layer */

				}
				}

	      				End();
					usleep(CURETIME);
//					finish();


				break;					}
			}

	    }
//	usleep(BLACKTIME);
//        draw_layers(cur_node->children,index);
    }

}

xmlNodePtr image_checkvalid(char *file_path, xmlNodePtr root)
{
	xmlDocPtr doc = NULL;
	xmlNodePtr rootNode = NULL;
	int ret = 0;

	xmlKeepBlanksDefault(0);
	doc = xmlReadFile(file_path, "UTF-8", XML_PARSE_RECOVER); // open a xml doc.
	rootNode = xmlDocGetRootElement(doc); // get root element.

	if (rootNode == NULL) {
		fprintf(stderr, "open file failed.\n");
		ret = -1;
		goto exit;
	}

	/* if the same, xmlStrcmp return 0, else return 1 */
	if (xmlStrcmp(rootNode->name, (const xmlChar *)"svg")) {
		fprintf(stderr, "check rootElement failed.\n");
		ret = -1;
		goto exit;
	}
	root = rootNode;
	if( root == NULL)
		printf("error before\n");
	return rootNode;
exit:
	if (rootNode != NULL) {
		xmlFree(rootNode);
	}

	if (doc != NULL) {
		xmlFreeDoc(doc);
	}

	return (xmlNodePtr)ret;

}

