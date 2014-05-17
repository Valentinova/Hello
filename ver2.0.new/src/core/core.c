#include<stdio.h>
#include "core.h"

int init_sys();

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

int  init_sys()
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
	image_init();
	image_fresh();	
	motor_init();
	oled_init();
	printf("init ready\n");
	return 1;
}
