#include "global.h"
#include "backend.h"

/***********************************************/
int image_initstate; 
int dlp_state;
int file_check;
int serial;
cmd_t back_command;

int motor_move(void);
int projector_control(void);
int printing_job(void);
int start_print(void);
void backend_init(void);
int read_fifo(void);

void * backend_thread()
{
	backend_init(void);

	while(status_backend != INIT)
	{
		require_frontend = read_fifo();
		if(require_frontend == NONE){
			continue;
		}

		switch(require_frontend)
		{
			case RESET_PRINTER: status_backend = INIT;require_frontend = NONE;break; 
			case HUNG_CMD: require_frontend = NONE;break; 					  
			case RECOVER: require_frontend = NONE;break; 					  
								
			case START_PRINT: status_backend = PRINTING;
						      require_frontend = NONE;
							  if(printing_job()){
								  status_backend = IDEL;
							  }else{
								  printf("backend: printing job error \n");
							  }break;
			case MOTOR_MOVE: status_backend = DEBUG; 
						     require_frontend = NONE;
							 if(motor_move()){
								 status_backend = IDEL;
							 }else{
								 printf("backend: MOTOR_MOVE error \n");
							 }break;
			case PROJ_CMD:   status_backend = DEBUG; 
						     require_frontend = NONE;
							 if(projector_control()){
								 status_backend = IDEL;
							 }else{
								 printf("backend: PROJ_CMD error \n");
							 }break;
			default:break;
		}
	}
	return((void *)0);
}

int read_fifo(void){
	pthread_mutex_lock(&lock_frontend_require);
	if(fifo_out(myfifo, &back_command, sizeof(cmd_t))){
		printf("backend: get a command from fifo \n");
	}else{
		printf("backend: fifo is empty\n");
		return NONE;
	}
	pthread_mutex_unlock(&lock_frontend_require);
	
	return back_command.cmd;
}

void backend_init(void)
{
	status_backend = INIT;
	motor_init();
	oled_init();

	serial = uart_init(); 
	image_init();
	image_fresh();

	status_backend = IDLE;
	printf("backend: printer hardware init ready\n");
}

int printing_job()
{
	printf("backend: start a standard printing job! \n");
	uart_dlppoweron(serial);
	sleep(3);
	if(start_print() == 0){
		return 0;
	}
	uart_dlppoweroff(serial);
	motor_finish()
	printf("backend: printing job finished! \n");
	return 1;
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
		printf("backend: file total layers is %ld \n",xmlChildElementCount(tempNode));
		for( i = 0 ;i < xmlChildElementCount(tempNode); i++)
		{
			require_frontend = read_fifo();
			
			if(require_frontend == HUNG_CMD){
				status_backend = HUNG;
				require_frontend = NONE;
		
				while(status_backend == HUNG)
				{
					require_frontend = read_fifo();
					if(require_frontend == RECOVER){
						status_backend = PRINTING;
						require_frontend = NONE;
					}
					if(require_frontend == RESET_PRINTER){
						return 0;
					}	
				}
			}else{
				printf("backend: command valid in printing process...\n");
			}

			layers_status = i;
			display_int(i);
			draw_layers(tempNode,i);
			printf("backend: print %dth layer\n",i);
			sleep(parameter.cure_time);
			image_fresh();
			sleep(parameter.freze_time);
			if(!motor_layer(parameter.thickness))
				return 0;
		}
	}
	else
	{
		printf("backend: the file is wrong\n");
	}
	image_fresh();
	image_end();
	return 1;
}

/***********************************************/
int  motor_move(void)
{
	int direction ,len;
	direction = *(int *)back_command.buf;
	len	= *(int *)(back_command.buf+sizeof(int));
	printf("backend: direction = %d, len = %d  \n",direction,len);
	motor_move_dis(direction,len);
	return 1;
}

int projector_control(void)
{
	int op, clr,layer,defi_x,defi_y,bold;
	
	printf("backend: in projector_control!\n");
	
	op = *(int *)back_command.buf;
	switch(op)
	{
		case PROJ_ON: 
			if(0 == dlp_state)  {uart_dlppoweron(serial); dlp_state=1;}else{ printf("backend: Projector is already on\n");}
			break;
		case PROJ_OFF:
			if(1 == dlp_state)  {uart_dlppoweroff(serial); dlp_state=0;}else{ printf("backend: Projector is already off\n");}
			break;
		case PROJ_DEBUG:
			clr = *((int *)back_command.buf+1);
			//image_init();
			if(0 == image_initstate){image_initstate = 1;image_init();}else{printf("backend: image inited already\n");}	
			switch(clr)
				{
				case 0: image_fresh_color(0, 0, 0);break;
				case 1: image_fresh_color(255, 255, 255);break;
				case 2: image_fresh_color(0, 0, 255);break;
				case 3: image_fresh_color(0, 255, 0);break;
				case 4: image_fresh_color(255, 0, 0);break;
				case 5:
					     defi_x = *((int *)back_command.buf+2);
					     defi_y = *((int *)back_command.buf+3);
					     bold= *((int *)back_command.buf+4);
					     Draw_raster(defi_x, defi_y, bold); break;	//Raster  :int defi_x, int defi_y, int bold
				case 6: imagetest(XGA_W, XGA_H); break; //JEPG Test	
				}
			break;
		case PROJ_LAYER:
			layer = *((int *)back_command.buf+1);
			Image_fresh_layer(layer);
			break;
		default:
			break;
	}
	return 1;
}

