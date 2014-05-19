#ifndef __FRONTEDN_H__
#define __FRONTEND_H__
#include "../inc/crc32.h"


#define ONLINE		1
#define OFFLINE		0
#define ERROR		11
#define MAXDATASIZE	1024
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
#define WORK_OVER	11
#define PROJ_CMD    12
#define GET_FILE_JPEG 13
#define HEARTBEAT   14



//projector
#define PROJ_ON     100
#define PROJ_OFF    101
#define PROJ_DEBUG  102
#define PROJ_LAYER  103



//feedback
#define WORK_NORMAL	3
#define WORK_ABNORMAL	4

#define FINISH_MARK	2013

//Network
#define SERVPORT 3333
#define BACKLOG 1
#define IDLE  -1
#define FILENAME	"test.xml"
#define FILENAME2 "jpegtest.jpg"



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
	int buf_len;	//the length of struct
	unsigned char buf[MAXDATASIZE];
}cmd_t;

typedef struct _feedback_t
{
	int status;
	int buf_len;//the length of struct
	char  buf[MAXDATASIZE];
}feedback_t;





extern int status_send(feedback_t * fback,int sockfd);
extern int cmd_recv(cmd_t *cmd,int sockfd); 

extern pthread_mutex_t lock_fd;
extern pthread_mutex_t lock_job;
extern pthread_mutex_t lock_hung;
extern pthread_cond_t  cond_fd;
extern pthread_cond_t  cond_job;
extern pthread_cond_t  cond_hung;
extern int status_client_fd;
extern int status_print_job;
extern int status_print_hung;

extern int status;
extern int cmd_parameters[3];
extern int layers_status;
extern cmd_t command;

extern int motor_move_direction;
extern int motor_move_len;
extern feedback_t fback;

extern int dlp_state;  
extern int serial_dlp;
extern int file_check;
extern int firstbeat;
extern int beat_timeout;

extern info_t sys_info;
extern para_t parameter;

extern void * frontend_thread();
#endif
