#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <memory.h>
#include "fifo.h"

fifo* fifo_create(void);
void fifo_destroy(fifo* fifo_ptr);
void fifo_in(fifo* fifo_ptr, int data);
int fifo_out(fifo* fifo_ptr);

fifo* fifo_create(void){
	fifo *fifo_ptr = malloc(sizeof(fifo));
	memset(fifo_ptr, 0, sizeof(fifo));
	fifo_ptr->write_ptr = 0;
	fifo_ptr->read_ptr = 0;
	fifo_ptr->flag = EMPTY;
	return fifo_ptr;
}

void fifo_destroy(fifo* fifo_ptr){
	free(fifo_ptr);
	printf("destroy fifo \n");
}

void fifo_in(fifo* fifo_ptr, int data){
	if(fifo_ptr->flag != FULL ){
		fifo_ptr->item[fifo_ptr->write_ptr] = data;
		fifo_ptr->write_ptr ++;
		fifo_ptr->write_ptr %= FIFO_LENGTH;
		if((fifo_ptr->write_ptr - fifo_ptr->read_ptr) == -1){
			fifo_ptr->flag = FULL;
		}else{
			fifo_ptr->flag = NORMAL;
		}
		printf("							write_ptr = %d \n", fifo_ptr->write_ptr);
	}else{
		printf("fifo is full, write invalide\n");
	}
}

int fifo_out(fifo* fifo_ptr){
	int data = 0;

	if(fifo_ptr->flag != EMPTY){
		data = fifo_ptr->item[fifo_ptr->read_ptr];
		fifo_ptr->read_ptr ++;
		fifo_ptr->read_ptr %= FIFO_LENGTH;
		if((fifo_ptr->write_ptr - fifo_ptr->read_ptr) == 0){
			fifo_ptr->flag = EMPTY;	
		}
				printf("							read_ptr = %d \n", fifo_ptr->read_ptr);
		return data;
	}else{
		printf("fifo is empty, read invalide\n");
		return -1;
	}

	return -1;
}

