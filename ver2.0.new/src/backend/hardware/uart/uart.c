#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <termios.h>
#include "uart.h"

static int uart_dev_scan(int *fd)
{
	/* Open port for read and write not making it a controlling terminal */
	int ret;
	*fd = open(SERIAL_DEV_0, O_RDWR | O_NOCTTY);

	if (*fd == -1) {
		printf("openPort: Unable to open port %s\n", SERIAL_DEV_0);

		*fd = open(SERIAL_DEV_1, O_RDWR | O_NOCTTY);

		if (*fd == -1) {
			printf("openPort: Unable to open port %s\n", SERIAL_DEV_1);
			ret = -1;
		}
		else {
			ret = 0;
		}
	}
	else {
		ret = 0;
	}

	return ret;

}

int uart_init()
{
	int fd;
	/* Port options */
	struct termios options;

	/* Open port for read and write not making it a controlling terminal */

	if ((uart_dev_scan(&fd)) == -1) {
		printf("Init the uart fail!\n");		
		return -1;
	}

	tcgetattr(fd, &options);
	memset(&options, 0, sizeof(options));

	cfsetispeed(&options, BAUD_RATE);
   	cfsetospeed(&options, BAUD_RATE);

	options.c_iflag = 0;
	options.c_oflag = 0;
	options.c_cflag |= (CLOCAL | CREAD | CS8);
	options.c_cflag &= ~PARENB; /* Parity disabled */
	options.c_cflag &= ~CSTOPB;
	options.c_cflag &= ~CSIZE;
	options.c_lflag=0;

	cfmakeraw(&options);

	options.c_cc[VMIN] = RD_NUM;	
	options.c_cc[VTIME] = 10;	

 	tcflush(fd, TCIFLUSH);
	tcsetattr(fd, TCSANOW, &options);

	/* 
	 * Sleep for UART to power up and set options
	 * Note: pls don't change the usleep.
	 */
	usleep(1000);

	return fd;
}

static int uart_write(int fd, const u8 *buf, u32 len)
{
	int num = write(fd, buf, len);

	if (num <= 0) {
		printf("Error writing\n");
		num =  -1;
	}

	return num;
}
#if 0
static int uart_read(int fd, u8 *buf, u32 len)
{
	int num = read(fd, buf, len);

	if (num <= 0) {
		printf("Error reading\n");
		num = -1;
	}

	return num;
}
#endif

int uart_dlppoweron(int fd)
{
	int ret;
	ret = uart_write(fd, start_cmd, CAMERA_CMD_LEN);

	if (ret != 13) {
		printf("start camera fail, num = %d\n", ret);
		return -1;
	}
	return 0;

}


int uart_dlppoweroff(int fd)
{
	int ret;
	ret = uart_write(fd, stop_cmd, CAMERA_CMD_LEN);

	if (ret != 13) {
		printf("stop camera fail, num = %d\n", ret);
		return -1;
	}
	return 0;

}



