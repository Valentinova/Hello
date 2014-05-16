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
	return fifo_ptr;
}

void fifo_destroy(fifo* fifo_ptr){
	free(fifo_ptr);
	printf("destroy fifo \n");
}

void fifo_in(fifo* fifo_ptr, int data){
	if((fifo_ptr->write_ptr < 10) && (fifo_ptr->write_ptr >= 0)){
		fifo_ptr->item[fifo_ptr->write_ptr] = data;
		fifo_ptr->write_ptr ++;
		printf("							write_ptr = %d \n", fifo_ptr->write_ptr);
	}else{
		printf("error: fifo overflow \n");
	}
}

int fifo_out(fifo* fifo_ptr){
	int data = 0;

	if(fifo_ptr->write_ptr > 0){
	   	if(fifo_ptr->read_ptr < fifo_ptr->write_ptr){
			data = fifo_ptr->item[fifo_ptr->read_ptr];
			fifo_ptr->read_ptr ++;
			if(fifo_ptr->read_ptr == fifo_ptr->write_ptr){
				fifo_ptr->read_ptr = 0;
				fifo_ptr->write_ptr = 0;		
			}
			printf("							read_ptr = %d \n", fifo_ptr->read_ptr);
			return data;
		}else if(fifo_ptr->read_ptr == fifo_ptr->write_ptr){
			fifo_ptr->read_ptr = 0;
			fifo_ptr->write_ptr = 0;		
			return -1;
		}
	}
	return -1;
}

