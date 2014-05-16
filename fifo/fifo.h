#ifndef __FIFO_H__
#define __FIFO_H__

#define FIFO_LENGTH 20

typedef struct require_fifo{
	int item[FIFO_LENGTH];
	int read_ptr;
	int write_ptr;
}fifo;

extern fifo* fifo_create(void);
extern void fifo_destroy(fifo* fifo_ptr);
extern void fifo_in(fifo* fifo_ptr, int data);
extern int fifo_out(fifo* fifo_ptr);

#endif
