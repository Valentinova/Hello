/*
 *	Global variables and defines shared by all threads.
 * */
#ifndef __GLOBAL_H__
#define __GLOBAL_H__

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
#include <arpa/inet.h>
#include "lib/crc32/crc32.h"
#include "lib/fifo/fifo.h"
#include "lib/timer/mytimer.h"

//network
#define ONLINE		1
#define OFFLINE		0
#define ERROR		11
#define MAXDATASIZE	1024
#define SERVPORT 3333
#define NEW_CLIENT 1000
#define BACKLOG 1
//command
#define HANDSHAKE	1
#define SEND_INFO	2
#define GET_PARA	3
#define GET_FILE	4
#define START_PRINT	5
#define LAYERS_STATUS   6
#define HUNG_CMD	7
#define RECOVER		8
#define MOTOR_MOVE	10
#define DISCONNECT 11
#define PROJ_CMD    12
#define GET_FILE_JPEG 13
#define HEARTBEAT   14
#define RESET_PRINTER 15
//project debug
#define PROJ_ON     100
#define PROJ_OFF    101
#define PROJ_DEBUG  102
#define PROJ_LAYER  103
//feedback
#define WORK_NORMAL	17
#define WORK_ABNORMAL	18
#define COMMAND_INVALID 16
#define FINISH_MARK	2013
//file name on server
#define FILENAME	"test.xml"
#define FILENAME2 "jpegtest.jpg"
//status
#define IDLE 0
#define BUSY 1
#define NONE -1
#define PRINTING 2
#define HUNG 3
#define DEBUG 4
#define INIT 5

typedef struct _connect_t
{
	int status;
	int magic;
} connect_t;

typedef struct _info_t
{
	int version;
	int max_high;
	int sockfd;
} info_t;

typedef struct _parameters_t
{
	int cure_time;
	int freze_time;
    int layers;
	int thickness;
} para_t;

typedef struct _cmd_t
{
	int cmd;
	int buf_len;
	char buf[MAXDATASIZE];
}cmd_t;

typedef struct _feedback_t
{
	int status;
	int buf_len;
	char buf[MAXDATASIZE];
}feedback_t;

extern pthread_mutex_t lock_gatekeeper_require;
extern pthread_mutex_t lock_frontend_require;
extern int require_gatekeeper;
extern int require_frontend;
extern int status_frontend;
extern int status_backend;

extern int status_send(feedback_t * fback,int sockfd);
extern int cmd_recv(cmd_t *cmd,int sockfd); 
extern cmd_t command;
extern feedback_t fback;
extern int present_client_fd; 
extern int layers_status;
extern int new_file_flag; 
extern info_t sys_info;
extern para_t parameter;
extern fifo* myfifo;
extern MyTimer* heartbeat;

#endif
