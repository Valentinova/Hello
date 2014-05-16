#ifndef __FIFO_H__
#define __FIFO_H__

#define FIFO_LENGTH 20
#define EMPTY	0x00
#define FULL	0x01
#define NORMAL  0x02

typedef struct require_fifo{
	int item[FIFO_LENGTH];
	int read_ptr;
	int write_ptr;
	int flag;
}fifo;

extern fifo* fifo_create(void);
extern void fifo_destroy(fifo* fifo_ptr);
extern void fifo_in(fifo* fifo_ptr, int data);
extern int fifo_out(fifo* fifo_ptr);

#endif
