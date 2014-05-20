#include "global.h"
#include "../gatekeeper/gatekeeper.h"
#include "../frontend/frontend.h"
#include "../backend/backend.h"

int init_sys();
int status_send(feedback_t * fback ,int sockfd);
int cmd_recv( cmd_t * cmd,int sockfd);

info_t sys_info;
para_t parameter;
cmd_t command;
feedback_t fback;
int layers_status;
int present_client_fd; 

pthread_mutex_t lock_gatekeeper_require;
pthread_mutex_t lock_frontend_require;
int require_gatekeeper;
int require_frontend;
int status_frontend;
int status_backend;

int main()
{
	pthread_t th_gatekeeper,th_frontend,th_backend;
	void * ret;
	
	init_sys();
	pthread_create(&th_gatekeeper,NULL,gatekeeper_thread,0);
	pthread_create(&th_frontend,NULL,frontend_thread,0);
	pthread_create(&th_backend,NULL,backend_thread,0);
	pthread_join(th_gatekeeper,&ret);
	pthread_join(th_frontend,&ret);
	pthread_join(th_backend,&ret);
	while(1)
	{
		printf("thread error\n");
	}
	return 1;
}

int init_sys()
{
	pthread_mutex_init(&lock_fd,NULL);
	pthread_mutex_init(&lock_job,NULL);
	pthread_cond_init(&cond_fd,NULL);
	pthread_cond_init(&cond_job,NULL);
	status_client_fd = IDLE;
	status_print_job = IDLE;
	status_print_hung = 0;  //set to 1 when hung
	signal(SIGPIPE,SIG_IGN);//ignore the SIGPIPE signal

	sys_info.version = 11;
	sys_info.max_high = 22;
	layers_status = -1;
	return 1;
}

int status_send(feedback_t * fback ,int sockfd)
{
	if((send(sockfd,fback,sizeof( feedback_t),0))==-1)
	{
		perror("send error");
		exit(1);
	}
	return 1;
}

int cmd_recv( cmd_t * cmd,int sockfd)
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

