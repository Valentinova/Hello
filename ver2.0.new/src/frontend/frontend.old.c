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
#include "frontend.h"
#include "../backend/hardware/hardware.h"

pthread_mutex_t lock_fd;
pthread_mutex_t lock_job;
pthread_mutex_t lock_hung;
pthread_cond_t  cond_fd;
pthread_cond_t  cond_job;
pthread_cond_t  cond_hung;
int status_client_fd;
int status_print_job;
int status_print_hung;

int status;
int cmd_parameters[3];
int layers_status;
int client_fd;

int motor_move_direction;
int motor_move_len;

int image_initstate = 0; 
int dlp_state = 0;   //flag:  0 is off, 1 is on;  used in projector debug
int serial_dlp = 0;  //inited in backend thread; used in projector debug
int file_check = 0;  //flag:  xml file check. 0 is not checked, 1 is checked.
int beat_timeout = 0;//flag:  1 means heartbeat is timeout. 
int firstbeat = 0;   //flag:  1 means first beat from client has done.

cmd_t command;
feedback_t fback;
info_t sys_info;
para_t parameter;

int  send_info(int cmd,int sockfd);
int  get_para(int cmd,int sockfd);
int  get_file(int cmd,int sockfd, int name);
int  prepare_print(int cmd,int sockfd);
int  send_layers_status(int cmd,int sockfd);
int  hung_action(int cmd,int sockfd);
int  recover_action(int cmd,int sockfd);
int  motor_move(int cmd,int sockfd);
int  close_client(int cmd,int sockfd);
int  projector_control(int cmd,int sockfd);
void heartbeat_timer(int cmd,int sockfd);

void * frontend_thread()
{
	int ret;
	while(1)
	{
		pthread_mutex_lock(&lock_fd);
		while(status_client_fd == IDLE)
		{
			pthread_cond_wait(&cond_fd,&lock_fd);
		}
		pthread_mutex_unlock(&lock_fd);
		client_fd = status_client_fd;
		while(1)
		{
			ret = cmd_recv(&command,client_fd);
			if(ret == 1){
				printf("get command %d to excute \n", command.cmd);
			}else if(ret == -1 || ret == 0){
				//printf("SOCKET_ERROR \n");
				if(beat_timeout == 1){
					printf("Heartbeat Timeout \n");
					command.cmd = WORK_OVER; 
				}else{
					continue;
				}
			}
			switch(command.cmd)
			{
				case SEND_INFO: send_info(command.cmd,client_fd);break;
				case GET_PARA: get_para(command.cmd,client_fd);break;
				case GET_FILE: get_file(command.cmd,client_fd, 0);break;
				case GET_FILE_JPEG: get_file(command.cmd,client_fd, 1);break;
				case START_PRINT:prepare_print(command.cmd,client_fd);break;
				case LAYERS_STATUS:send_layers_status(command.cmd,client_fd);break;
				case HUNG_CMD:hung_action(command.cmd,client_fd);break;
				case RECOVER:recover_action(command.cmd,client_fd);break;
				case MOTOR_MOVE:motor_move(command.cmd,client_fd);break;
				case WORK_OVER:close_client(command.cmd,client_fd);break;
				case PROJ_CMD:projector_control(command.cmd,client_fd);break;
				case HEARTBEAT:heartbeat_timer(command.cmd,client_fd);break;
				default:break;
			}

			if((IDLE == status_print_job)&&(START_PRINT == command.cmd))
			{   //start the print process
				pthread_mutex_lock(&lock_job);
				status_print_job = command.cmd;
				pthread_cond_signal(&cond_job);
				pthread_mutex_unlock(&lock_job);
			}
			if(WORK_OVER == command.cmd)
				break;
		}
		status_client_fd = IDLE;
	}
		return((void*)0);

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

void heartbeat_timer(int cmd,int sockfd)
{
	if(firstbeat == 0){ InitHeartBeat(); firstbeat = 1;} //first beat
	
	Reset_HeartBeat();
	beat_timeout = 0;
	if(client_fd != IDLE)
	{
	fback.status = WORK_NORMAL;
	fback.buf_len = 0;
	status_send(&fback,sockfd);
	}
}

int  status_send(feedback_t * fback ,int sockfd)
{
	if((send(sockfd,fback,sizeof( feedback_t),0))==-1)
	{
		perror("send error");
		exit(1);
	}
	return 1;
}

int  cmd_recv( cmd_t * cmd,int sockfd)
{
	int recvbytes;
	recvbytes = recv(sockfd,(void*)cmd,sizeof( cmd_t),0);
	if(recvbytes < 0)
	{
		return -1;  // SOCKET_ERROR
	}else if(recvbytes == 0){
		return 0;   // connect is break
	}
	return 1;       //reve correct
}

int  close_client(int cmd,int sockfd)
{
	if(START_PRINT == status_print_job)
	{
		pthread_mutex_lock(&lock_hung);
		if(status_print_hung == 1)
		{
			pthread_cond_signal(&cond_job);
		}
		//status_print_hung = 2;
		pthread_mutex_unlock(&lock_hung);
	}
	return 1;
}

int  send_info(int cmd,int sockfd)
{
	char * info;
	int i;
	sys_info.sockfd = sockfd;
	fback.status = WORK_NORMAL;
	fback.buf_len = sizeof(info_t);
	info = (char *)(&sys_info);
	for(i=0;i<fback.buf_len;i++)
	{
		fback.buf[i] = info[i];
	}
	status_send(&fback,sockfd);

	return 1;
}

int  get_para(int cmd,int sockfd)
{
	para_t * para;
	para = (para_t *)(command.buf);
	parameter.cure_time = para->cure_time;
	parameter.freze_time = para->freze_time;
	parameter.layers  = para->layers;
	parameter.thickness = para->thickness;

	fback.status = WORK_NORMAL;
	fback.buf_len = 0;

	status_send(&fback,sockfd);
	return 1;
}

int  get_file(int cmd,int sockfd, int name)
{
	FILE * fd;
	int * finish_mark ;
	unsigned int crc32 = 0;
	int i=0,ret=0;
	if(name == 0){fd = fopen(FILENAME,"w");}else if(name == 1){remove(FILENAME2); fd = fopen(FILENAME2,"w");}
	finish_mark =(int *) command.buf;
 	if(firstbeat == 1) stopTimer();
	while(*finish_mark != FINISH_MARK)
	{
		printf("%d receive %d bytes\n",i++,command.buf_len);

		ret = fwrite(command.buf,1,command.buf_len,fd);
		crc32 = ssh_crc32_s(command.buf,command.buf_len,crc32);
		//fwrite(command.buf,sizeof(char),command.buf_len,fd);
		printf("fwrite ret = %d\n ",ret);
		bzero(command.buf,command.buf_len);
		fback.status = WORK_NORMAL;
		fback.buf_len = 0;
		status_send(&fback, sockfd);
		cmd_recv(&command,sockfd);
		finish_mark = (int *)command.buf;
		printf("the finish_mark is %d\n",*finish_mark);
	}
	if(firstbeat == 1) Reset_HeartBeat();
	printf("the crc32 is %d and the recive is %d\n",crc32,*(int*)(command.buf+sizeof(int)));
	if(crc32 == *(int*)(command.buf+sizeof(int)))
		printf("the crc32 is right\n");
	fclose(fd);
	fback.status = WORK_NORMAL;
	fback.buf_len = 0;

	file_check = 0; //new xml file is not checked

	status_send(&fback,sockfd);
	return 1;
}

int  prepare_print(int cmd,int sockfd)
{
	fback.status = WORK_NORMAL;
	fback.buf_len = sizeof(int);
	status_send(&fback,sockfd);
	return 1;
}

int  motor_move(int cmd,int sockfd)
{
	int direction ,len;
	direction = *(int *)command.buf;
	len	= *(int *)(command.buf+sizeof(int));
	printf("%d,%d  \n",direction,len);
	motor_move_dis(direction,len);
	motor_move_direction = direction;
	motor_move_len = len;
	fback.status = WORK_NORMAL;
	fback.buf_len = 0;
	status_send(&fback,sockfd);
	return 1;
}

int send_layers_status(int cmd,int sockfd)
{
	fback.status = WORK_NORMAL;
	fback.buf_len = sizeof(int);
	memcpy(fback.buf,&layers_status,sizeof(int));
	status_send(&fback,sockfd);
	return 1;
}

int hung_action(int cmd,int sockfd)
{//hung the print process
	pthread_mutex_lock(&lock_hung);
	status_print_hung = 1 ;
	pthread_mutex_unlock(&lock_hung);
	fback.status = WORK_NORMAL;
	fback.buf_len = 0;
	status_send(&fback,sockfd);
	return 1;
}

int recover_action(int cmd,int sockfd)
{//recover the print process
	pthread_mutex_lock(&lock_hung);
	status_print_hung = 0 ;
	pthread_cond_signal(&cond_hung);
	pthread_mutex_unlock(&lock_hung);
	fback.status = WORK_NORMAL;
	fback.buf_len = 0;
	status_send(&fback,sockfd);
	return 1;
}

