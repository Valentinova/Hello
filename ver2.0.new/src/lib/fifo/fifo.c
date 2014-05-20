#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <memory.h>
#include "fifo.h"

fifo* fifo_create(void);
void fifo_destroy(fifo* fifo_ptr);
int fifo_in(fifo* fifo_ptr, void* data, int datalen);
int fifo_out(fifo* fifo_ptr, void* data, int datalen);

fifo* fifo_create(void){
	int i=0;
	fifo *fifo_ptr = malloc(sizeof(fifo));
	memset(fifo_ptr, 0, sizeof(fifo));
	for(i=0; i<FIFO_LENGTH; i++){
		fifo_ptr->item[i] = &(fifo_ptr->buffer[i]);
	}
	fifo_ptr->write_ptr = 0;
	fifo_ptr->read_ptr = 0;
	fifo_ptr->flag = EMPTY;
	printf("fifo: create fifo \n");
	return fifo_ptr;
}

void fifo_destroy(fifo* fifo_ptr){
	free(fifo_ptr);
	printf("fifo: destroy fifo \n");
}

int fifo_in(fifo* fifo_ptr, void* data, int datalen){
	if(fifo_ptr->flag != FULL ){
		memcpy(fifo_ptr->item[fifo_ptr->write_ptr], data, datalen);
		fifo_ptr->write_ptr ++;
		fifo_ptr->write_ptr %= FIFO_LENGTH;
		if((fifo_ptr->write_ptr - fifo_ptr->read_ptr) == -1){
			fifo_ptr->flag = FULL;
		}else{
			fifo_ptr->flag = NORMAL;
		}
		printf("							write_ptr = %d \n", fifo_ptr->write_ptr);
		return 1;
	}else{
		printf("fifo is full, write invalide\n");
		return 0;
	}
}

int fifo_out(fifo* fifo_ptr, void* data, int datalen){

	if(fifo_ptr->flag != EMPTY){
		memcpy(data, fifo_ptr->item[fifo_ptr->read_ptr], datalen);
		fifo_ptr->read_ptr ++;
		fifo_ptr->read_ptr %= FIFO_LENGTH;
		if((fifo_ptr->write_ptr - fifo_ptr->read_ptr) == 0){
			fifo_ptr->flag = EMPTY;	
		}
				printf("							read_ptr = %d \n", fifo_ptr->read_ptr);
		return 1;
	}else{
		printf("fifo is empty, read invalide\n");
		return 0;
	}
}

