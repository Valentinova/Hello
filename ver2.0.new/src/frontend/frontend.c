#include "../global.h"
#include "frontend.h"

int client_fd;

void cmd_valid(void);
void write_fifo(void);
int  send_info(int cmd,int sockfd);
int  get_para(int cmd,int sockfd);
int  get_file(int cmd,int sockfd, int name);
int  send_layers_status(int cmd,int sockfd);
void heartbeat_timer(int cmd,int sockfd);

void * frontend_thread()
{
	int ret;
	while(1)
	{
		while(status_frontend == IDLE)
		{
			if(require_gatekeeper == NEW_CLIENT){
				status_frontend = BUSY;
				
				pthread_mutex_lock(&lock_gatekeeper_require);
				require_gatekeeper = NONE; 
				pthread_mutex_unlock(&lock_gatekeeper_require);

				client_fd = present_client_fd;
				break;
			}
		}

		while(status_frontend == BUSY)
		{
			ret = cmd_recv(&command,client_fd);
			if(ret == 1){
				printf("frontend: get command %d to excute \n", command.cmd);
			}else if(ret == -1 || ret == 0){
				printf("frontend: read socket error \n");
				if(IsTimeout(heartbeat) == 1){
					printf("frontend: Heartbeat Timeout \n");
					command.cmd = DISCONNECT; 
				}
			}

			switch(command.cmd)
			{
				case SEND_INFO: send_info(command.cmd,client_fd);break;
				case GET_PARA: get_para(command.cmd,client_fd);break;
				case GET_FILE: get_file(command.cmd,client_fd, 0);break;
				case GET_FILE_JPEG: get_file(command.cmd,client_fd, 1);break;
				case LAYERS_STATUS:send_layers_status(command.cmd,client_fd);break;
				case HEARTBEAT:heartbeat_timer(command.cmd,client_fd);break;

				case MOTOR_MOVE: if(status_backend == IDLE || status_backend == DEBUG){write_fifo();break;}else{cmd_valid();break;}
				case PROJ_CMD: if(status_backend == IDLE || status_backend == DEBUG){write_fifo();break;}else{cmd_valid();break;}
				case START_PRINT: if(status_backend == IDLE){write_fifo();break;}else{cmd_valid();break;}
				case HUNG_CMD: if(status_backend == PRINTING){write_fifo();break;}else{cmd_valid();break;}
				case RECOVER: if(status_backend == HUNG){write_fifo();break;}else{cmd_valid();break;}
				case RESET_PRINTER: if(status_backend == IDLE || status_backend == DEBUG || status_backend == HUNG){write_fifo();break;}else{cmd_valid();break;}

				case DISCONNECT: status_frontend = IDLE; break;
				default:break;
			}
		}
	}
		return((void*)0);
}

void write_fifo(void)
{
	pthread_mutex_lock(&lock_frontend_require);
	if(fifo_in(myfifo, &command, sizeof(cmd_t))){
		printf("frontend: push a command into fifo \n");
	}else{
		printf("frontend: fifo is full\n");
	}
	pthread_mutex_unlock(&lock_frontend_require);
}

void cmd_valid(void)
{
	printf("frontend: command invalid \n");
	fback.status = COMMAND_INVALID;
	fback.buf_len = 0;
	status_send(&fback,client_fd);
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
    StopHeartBeat(heartbeat);
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
	ResetHeartBeat(heartbeat);
	printf("the crc32 is %d and the recive is %d\n",crc32,*(int*)(command.buf+sizeof(int)));
	if(crc32 == *(int*)(command.buf+sizeof(int)))
		printf("the crc32 is right\n");
	fclose(fd);
	fback.status = WORK_NORMAL;
	fback.buf_len = 0;
	status_send(&fback,sockfd);

	new_file_flag = 1;
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

void heartbeat_timer(int cmd,int sockfd)
{
	ResetHeartBeat(heartbeat);
	
	fback.status = WORK_NORMAL;
	fback.buf_len = 0;
	status_send(&fback,sockfd);
}

