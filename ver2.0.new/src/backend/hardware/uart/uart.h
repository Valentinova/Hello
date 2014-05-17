/*
 * Copyright@3Green-tehc.com 2013-2015
 * ...
 */ 

#ifndef __UART_H__
#define __UART_H__

#include "../../../inc/typedef.h"

#define SERIAL_DEV_0	"/dev/ttyAMA0"
#define SERIAL_DEV_1	"/dev/ttyAMA1"

#define RD_NUM			0x1

#define CAMERA_CMD_LEN	13

#define CAMERA_WORK_STOP		0x0
#define CAMERA_WORK_START		0x1	

#define CAMERA_STA_IDLE			0x0
#define CAMETA_STA_STOP			0x1
#define CAMERA_STA_SHUTDOWN		0x2
#define CAMERA_STA_WORKING		0x3

/* For COSTAR projector c185 settings. */
#define BAUD_RATE	B19200

/*
 * camera cmd defined here.
 */ 
static const u8 start_cmd[CAMERA_CMD_LEN] = {
	0xBE, 0xEF, 0x10, 0x05, 0x00, 0xC6, 0xFF, 0x11, 0x11,
	0x01, 0x00, 0x01, 0x00
};

static const u8 stop_cmd[CAMERA_CMD_LEN] = {
	0xBE, 0xEF, 0x03, 0x06, 0x00, 0xDC, 0xDB, 0x69, 0x00,
	0x00, 0x00, 0x00, 0x00
};

static const u8 auto_input_on_cmd[CAMERA_CMD_LEN] = {
	0xBE, 0xEF, 0x03, 0x06, 0x00, 0x89, 0xDB, 0x6C, 0x00,
	0x00, 0x00, 0x00, 0x00
};

static const u8 auto_input_off_cmd[CAMERA_CMD_LEN] = {
	0xBE, 0xEF, 0x03, 0x06, 0x00, 0x58, 0xDA, 0x6D, 0x00,
	0x00, 0x00, 0x00, 0x00
};

static const u8 auto_sig_src_cmd[CAMERA_CMD_LEN] = {
	0xBE, 0xEF, 0x02, 0x06, 0x00, 0x57, 0xD0, 0x2E, 0x00,
	0x00, 0x00, 0x00, 0x00
};

static const u8 auto_sig_sync_cmd[CAMERA_CMD_LEN] = {
	0xBE, 0xEF, 0x02, 0x06, 0x00, 0x86, 0xD1, 0x2F, 0x00,
	0x00, 0x00, 0x00, 0x00
};

int uart_init();
int uart_dlppoweroff(int fd);
int uart_dlppoweron(int fd);
#endif
