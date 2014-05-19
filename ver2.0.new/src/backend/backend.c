#include "global.h"
#include "backend.h"

/***********************************************/
int image_initstate; 
int dlp_state;
int serial_dlp;
int file_check;

int motor_move(int cmd,int sockfd);
int projector_control(int cmd,int sockfd);
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

/***********************************************/
int  motor_move(int cmd,int sockfd)
{
	int direction ,len;
	direction = *(int *)command.buf;
	len	= *(int *)(command.buf+sizeof(int));
	printf("%d,%d  \n",direction,len);
	motor_move_dis(direction,len);
	fback.status = WORK_NORMAL;
	fback.buf_len = 0;
	status_send(&fback,sockfd);
	return 1;
}

int projector_control(int cmd,int sockfd)
{
	int op, clr,layer,defi_x,defi_y,bold;
	
	printf(" In projector_control!\n");
	
	op = *(int *)command.buf;
	switch(op)
	{
		case PROJ_ON: 
			if(0 == dlp_state)  {uart_dlppoweron(serial_dlp); dlp_state=1;}else{ printf("Projector is already on\n");}
			break;
		case PROJ_OFF:
			if(1 == dlp_state)  {uart_dlppoweroff(serial_dlp); dlp_state=0;}else{ printf("Projector is already off\n");}
			break;
		case PROJ_DEBUG:
			clr = *((int *)command.buf+1);
			//image_init();
			if(0 == image_initstate){image_initstate = 1;image_init();}else{printf("image inited already\n");}	
			switch(clr)
				{
				case 0: image_fresh_color(0, 0, 0);break;
				case 1: image_fresh_color(255, 255, 255);break;
				case 2: image_fresh_color(0, 0, 255);break;
				case 3: image_fresh_color(0, 255, 0);break;
				case 4: image_fresh_color(255, 0, 0);break;
				case 5:
					     defi_x = *((int *)command.buf+2);
					     defi_y = *((int *)command.buf+3);
					     bold= *((int *)command.buf+4);
					     Draw_raster(defi_x, defi_y, bold); break;	//Raster  :int defi_x, int defi_y, int bold
				case 6: imagetest(XGA_W, XGA_H); break; //JEPG Test	
				}
			break;
		case PROJ_LAYER:
			layer = *((int *)command.buf+1);
			Image_fresh_layer(layer);
			break;
		default:
			break;
	}
	fback.status = WORK_NORMAL;
	fback.buf_len = 0;
	status_send(&fback,sockfd);
	return 1;
}

