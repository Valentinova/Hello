#include "../global.h"
#include "gatekeeper.h"

int hand_client(int sockfd);

void * gatekeeper_thread()
{
	int sockfd,client_fd_temp; /*sock_fd：linsten socket；client_fd_temp：data transport socket */
	struct sockaddr_in my_addr;
	struct sockaddr_in remote_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
	{
		perror( "gatekeeper:socket created error\n");
	   	exit(1);
	}
	my_addr.sin_family=AF_INET;
	my_addr.sin_port=htons(SERVPORT);
	my_addr.sin_addr.s_addr = INADDR_ANY;
	bzero( &(my_addr.sin_zero),8);
	if (bind(sockfd, (struct sockaddr *) &my_addr, sizeof(struct sockaddr)) == -1)
	{
		perror( "gatekeeper:bind error\n");
		exit(1);
	}
	if (listen(sockfd, BACKLOG) == -1)
	{
		perror( "gatekeeper:listen error\n");
		exit(1);
	}

	while(status_backend != INIT)
	{
		socklen_t sin_size = sizeof(struct sockaddr_in);
		client_fd_temp = accept(sockfd, (struct sockaddr *) &remote_addr,&sin_size); 
		if (client_fd_temp == -1) {
			perror( "gatekeeper:accept error\n");
			continue;
		}
		printf( "gatekeeper:received a connection from %s\n",(char*)inet_ntoa(remote_addr.sin_addr));
		if(!hand_client(client_fd_temp))
		{
			close(client_fd_temp);
		}
	}

	return ((void *)0);
}

int  hand_client(int sockfd)
{
 	int  ret = 0;
	cmd_recv(&command,sockfd);
	if(HANDSHAKE == command.cmd)
	{
		if(status_frontend == IDLE)
		{
			present_client_fd = sockfd;

			pthread_mutex_lock(&lock_gatekeeper_require);
			require_gatekeeper = NEW_CLIENT;
			pthread_mutex_unlock(&lock_gatekeeper_require);

			fback.status = OFFLINE;
			ret = 1;
		}
		else
		{
			fback.status = ONLINE;
		}
	}
	else
	{
		fback.status = ERROR;
	}
	fback.buf_len = 0;
	status_send(&fback,sockfd);

	return ret;
}
