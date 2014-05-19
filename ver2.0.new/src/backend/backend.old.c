#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>
#include <memory.h>
#include <fcntl.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include "backend.h"
#include "../frontend/frontend.h"

extern xmlNodePtr image_checkvalid(char *file_path, xmlNodePtr root);
extern void draw_layers(xmlNode * node,int index);

void Motor_move();
int start_print();

void * backend_thread()
{
	int serial;
	image_init();
	image_fresh();
	serial = uart_init(); serial_dlp = serial; //serial_dlp for debug
	
	image_fresh();
	while(1)
	{
		pthread_mutex_lock(&lock_job);
		while(status_print_job == IDLE)
		{
			pthread_cond_wait(&cond_job,&lock_job);
		}
		pthread_mutex_unlock(&lock_job);
		printf("Backend start the standard printing job! \n");
		switch(status_print_job)
		{
			case START_PRINT:uart_dlppoweron(serial);sleep(3);start_print();uart_dlppoweroff(serial);motor_finish();break;
			//case MOTOR_MOVE:Motor_move();break;
			default:break;
		}
		status_print_job = IDLE;
	}
	return((void *)0);
}

int  start_print()
{
	int i;
	xmlNodePtr  rootNode = NULL;
	xmlNodePtr  tempNode;
	tempNode = image_checkvalid((char *)"test.xml",(xmlNodePtr)rootNode);

	motor_to_low();
	image_init();
	image_fresh();
	if (tempNode)
	{
		printf("the total layers is %ld \n",xmlChildElementCount(tempNode));
		for( i = 0 ;i < xmlChildElementCount(tempNode); i++)
		{
			pthread_mutex_lock(&lock_hung);
			if(status_print_hung == 1)
				pthread_cond_wait(&cond_hung,&lock_hung);
			pthread_mutex_unlock(&lock_hung);
			//if(status_print_hung == 2 ) return 2;//client disconnected when in hung
			
			layers_status = i;
			display_int(i);
			draw_layers(tempNode,i);
			printf("print %d layer\n",i);
			sleep(parameter.cure_time);
			image_fresh();
			sleep(parameter.freze_time);
			if(!motor_layer(parameter.thickness))
				return 0;
		}
	}
	else
	{
		printf("the file is wrong\n");
	}
	image_fresh();

	image_end();
	return 1;
}

void Motor_move()
{
	motor_move_dis(motor_move_direction,motor_move_len);
}
