/*image_mgr_t image_mgr;
 *
 * Copyright @ 3greentech, 2013-2015
 */
#ifndef __MOTOR_H__
#define __MOTOR_H__


#include "typedef.h"


#define DRIVEPHASE	2
#define MICRODIV	8
#define MSPEED		400 /* as 1ms = 1000HZ* up to 1300HZ as known*/
#define GAPLEN		10000 /*as 10000 um*/
#define STEP_100UM	32

//#define GPIO_ROTOR			11
//#define GPIO_INPUT			7
//#define GPIO_DIRECTION			12
#define LEVEL_HIGH			0
#define LEVEL_LOW			1


/* One circle need 50 step and one step need four time cycle. */
#define SCREW_ONE_CIRCLE	50
#define DIR_DOWN			0
#define DIR_UP				1
#define PRINT_INIT_PLACE	1250

#define MOTOR_WORK_DOWN		0x0
#define MOTOR_WORK_UP		0x1

void motor_to_low();
int motor_init();
int  motor_layer(int len);
int  motor_move_dis(int direction,int len);
void motor_finish();
#endif
