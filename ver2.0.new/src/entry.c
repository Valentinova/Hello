#include "global.h"
#include "gatekeeper/gatekeeper.h"
#include "frontend/frontend.h"
#include "backend/backend.h"

int init_sys();
int status_send(feedback_t * fback ,int sockfd);
int cmd_recv( cmd_t * cmd,int sockfd);
void keyboard_exit(int signo);

info_t sys_info;
para_t parameter;
cmd_t command;
feedback_t fback;
int layers_status;
int present_client_fd; 
int new_file_flag; 
fifo* myfifo;
MyTimer* heartbeat;

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
	pthread_mutex_init(&lock_gatekeeper_require,NULL);
	pthread_mutex_init(&lock_frontend_require,NULL);
	status_frontend = IDLE;
	status_backend = INIT;
	require_gatekeeper = NONE;
	require_frontend = NONE;
	myfifo = fifo_create();
	heartbeat = InitHeartBeat(4000/*ms*/);
    StopHeartBeat(heartbeat);

	signal(SIGPIPE,SIG_IGN);//ignore the SIGPIPE signal
	signal(SIGINT, keyboard_exit);

	sys_info.version = 11;
	sys_info.max_high = 22;
	layers_status = -1;
	present_client_fd = 0;

	return 1;
}

int status_send(feedback_t * fback ,int sockfd)
{
	if((send(sockfd,fback,sizeof( feedback_t),0))==-1)
	{
		perror("send error");
		return 0;
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

void keyboard_exit(int signo){
	printf("exit by keyboard \n");
	fifo_destroy(myfifo);
	DestroyHeartBeat(heartbeat);
	_exit(0);
}
