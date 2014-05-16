#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>

#include "../fifo.h"

pthread_mutex_t lock_fifo;
fifo* myfifo;

void * producer_thread1(void *pin){
	pin = NULL;
	while(1){
		pthread_mutex_lock(&lock_fifo);
		fifo_in(myfifo, 1);
		pthread_mutex_unlock(&lock_fifo);
		printf("producer1 put 1 into myfifo\n");
		sleep(1);
	}
		return((void*)0);
}

void * producer_thread2(void *pin){
	pin = NULL;
	while(1){
		pthread_mutex_lock(&lock_fifo);
		fifo_in(myfifo, 2);
		pthread_mutex_unlock(&lock_fifo);
		printf("producer2 put 2 into myfifo\n");
		sleep(1);
	}
		return((void*)0);
}

void * consumer_thread1(void *pin){
	int require = 0;
	pin = NULL;
	
	while(1){
		pthread_mutex_lock(&lock_fifo);
		require = fifo_out(myfifo);
		pthread_mutex_unlock(&lock_fifo);
		printf("	consumer1 get %d form myfifo\n", require);
		sleep(1);
	}
		return((void*)0);
}

void * consumer_thread2(void *pin){
	int require = 0;
	pin = NULL;
	
	while(1){
		pthread_mutex_lock(&lock_fifo);
		require = fifo_out(myfifo);
		pthread_mutex_unlock(&lock_fifo);
		printf("	consumer2 get %d form myfifo\n", require);
		sleep(1);
	}
		return((void*)0);
}

void keyboard_exit(int signo){
	printf("exit by keyboard \n");
	fifo_destroy(myfifo);
	_exit(0);
}

int main(){
	pthread_t th_producer1, th_producer2, th_consumer1, th_consumer2;
	void * ret;
	
	pthread_mutex_init(&lock_fifo, NULL);
	myfifo = fifo_create();
	signal(SIGINT, keyboard_exit);
	pthread_create(&th_producer1, NULL, producer_thread1, 0);	
	pthread_create(&th_producer2, NULL, producer_thread2, 0);	
	pthread_create(&th_consumer1, NULL, consumer_thread1, 0);	
	pthread_create(&th_consumer2, NULL, consumer_thread2, 0);	
	
	pthread_join(th_producer1, &ret);
	pthread_join(th_producer2, &ret);
	pthread_join(th_consumer1, &ret);
	pthread_join(th_consumer2, &ret);

	while(1){
		printf("thread error\n");
	}
	return 1;
}

