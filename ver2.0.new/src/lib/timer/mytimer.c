/*
 *
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include "mytimer.h"

int internal_beat_timeout; 

void setTimer(timer_t *timerId, int timeMSec);  
void CreateHeartBeat(MyTimer* timer_ptr);

void InitHeartBeat(MyTimer* timer_ptr, int interval);
void DestroyHeartBeat(MyTimer* timer_ptr);
void ResetHeartBeat(MyTimer* timer_ptr);
void StopHeartBeat(MyTimer* timer_ptr);
int  IsTimeout(MyTimer* timer_ptr);

void CreateHeartBeat(MyTimer* timer_ptr){
	struct sigevent sev;  

	sev.sigev_notify = SIGEV_SIGNAL;  
	sev.sigev_signo = SIGUSR1;  
	sev.sigev_value.sival_ptr = NULL;  
	//sev.sigev_notify_function = func;  
	//sev.sigev_notify_attributes = NULL;  
	if (timer_create (CLOCK_REALTIME, &sev, &(timer_ptr->mytimer)) == -1)  
	{  
	    perror("timer_create, error");  
	    return;  
	}  
	if ((int)(timer_ptr->mytimer) == -1)  
	    perror("timer_create error, id is -1");  
	return;  
}  

void setTimer(timer_t *timerId, int timeMSec)  
{  
	struct itimerspec its;  

	its.it_value.tv_sec = timeMSec / 1000;  
	its.it_value.tv_nsec = (timeMSec % 1000) * 1000000;  
	its.it_interval.tv_sec = timeMSec / 1000;;  
	its.it_interval.tv_nsec =  (timeMSec % 1000) * 1000000;  
	if (timer_settime (*timerId, 0, &its, NULL) == -1)  
	{  
	    perror("timer_settime error"); 
	}  
}  

void BeatTimeout(union sigval sig)  
{  
	printf("Beat Timeout....\n");
	internal_beat_timeout = 1; //indicate timeout of beat
}

void StopHeartBeat(MyTimer* timer_ptr){
	printf("Stop the timer.\n");
	setTimer(&(timer_ptr->mytimer),0/*ms*/);
}

void ResetHeartBeat(MyTimer* timer_ptr){
	timer_ptr->timeout = 0;
	internal_beat_timeout = 0;
	printf("Reset the timer. Beat %d times.\n", timer_ptr->resetcnt++);
	setTimer(&(timer_ptr->mytimer),timer_ptr->interval/*ms*/);
}

void InitHeartBeat(MyTimer* timer_ptr, int interval){
	timer_ptr->interval = interval;
	CreateHeartBeat(timer_ptr);
	signal(SIGUSR1,(__sighandler_t)BeatTimeout);
	setTimer(&(timer_ptr->mytimer),timer_ptr->interval/*ms*/);
}

void DestroyHeartBeat(MyTimer* timer_ptr){
    if(-1 == timer_delete(timer_ptr->mytimer)){
         printf("Delete timer error\n");
	} 
	printf("Destory mytimer...\n");
	free(timer_ptr);
}

int IsTimeout(MyTimer* timer_ptr){
	if(internal_beat_timeout == 1){
		return 1;
	}else{
		return 0;
	}
	return 0;
}
