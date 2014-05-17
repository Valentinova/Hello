/*This is mytimer for heartbeat;
 *
 */
#ifndef __MYTIMER_H__
#define __MYTIMER_H__

typedef struct{
	timer_t mytimer;
	int interval;//ms
	int timeout; //flag: 1 is timeout ture
    unsigned int resetcnt;
	}MyTimer;

extern void InitHeartBeat(MyTimer* timer_ptr, int interval);
extern void ResetHeartBeat(MyTimer* timer_ptr);
extern void StopHeartBeat(MyTimer* timer_ptr);
extern int IsTimeout(MyTimer* timer_ptr);

#endif
