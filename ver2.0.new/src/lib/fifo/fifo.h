/*
 * This is a loop fifo for multi-thread commuting.
 * Note: you can set the fifo length through the value of FIFO_LENGTH.
 *		 apparently, you should use mutex-lock to protect funtion of fifo_in and fifo_out.
 * */
#ifndef __FIFO_H__
#define __FIFO_H__

#define FIFO_LENGTH 10
#define FIFO_ITEM_SIZE 1500 //cmd size is 1024 + 8 
#define EMPTY	0x00
#define FULL	0x01
#define NORMAL  0x02

typedef struct _fifo_item_{
	char buf[FIFO_ITEM_SIZE];
}fifo_item;

typedef struct _fifo_{
	fifo_item *item[FIFO_LENGTH];
	fifo_item buffer[FIFO_LENGTH];
	int read_ptr;
	int write_ptr;
	int flag;
}fifo;

extern fifo* fifo_create(void);
extern void fifo_destroy(fifo* fifo_ptr);
extern int fifo_in(fifo* fifo_ptr, void* data, int datalen);
extern int fifo_out(fifo* fifo_ptr, void* data, int datalen);

#endif
