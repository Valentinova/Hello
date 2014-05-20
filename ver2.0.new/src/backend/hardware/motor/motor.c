/*
 * Copyright 3green tech
 */
#include <bcm2835.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "motor.h"
#define GPIO_ROTOR	RPI_GPIO_P1_11
#define GPIO_DIRECTION	RPI_GPIO_P1_15
#define GPIO_INPUT	RPI_GPIO_P1_07
#define GPIO_INPUT1	RPI_GPIO_P1_22

int motor_walks(int direction, unsigned int step_count);

static void motor_delay_ms(int time)
{
	//usleep(time);
	bcm2835_delayMicroseconds(time);
}

int motor_init()
{
	int ret = 0;

	if(!bcm2835_init())
		return 1;

	/* set mode to output */
	bcm2835_gpio_fsel(GPIO_ROTOR, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(GPIO_DIRECTION, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(GPIO_INPUT, BCM2835_GPIO_FSEL_INPT);

	bcm2835_gpio_fsel(GPIO_INPUT1, BCM2835_GPIO_FSEL_INPT);
	bcm2835_gpio_set_pud(GPIO_INPUT,BCM2835_GPIO_PUD_UP);
	bcm2835_gpio_set_pud(GPIO_INPUT1,BCM2835_GPIO_PUD_UP);
	/* wait for a stable status */
	motor_delay_ms(1); 

	bcm2835_gpio_write(GPIO_ROTOR, HIGH);

	bcm2835_gpio_write(GPIO_DIRECTION,DIR_UP);
	motor_delay_ms(1);
	printf("motor: waiting for initial position...\n");
	while(bcm2835_gpio_lev(GPIO_INPUT1) == LEVEL_HIGH)
	{
		motor_walks(DIR_UP,100);
	}
	printf("motor: init done \n");
	return ret;
}

void motor_to_low()
{
	printf("motor: waiting for printing position...\n");
	while(bcm2835_gpio_lev(GPIO_INPUT) == LEVEL_HIGH)
	{
		motor_walks(DIR_DOWN,100);
	}
	printf("motor: done \n");
}

/* static void motor_prepare_print(void) */
/* { */
/* 	motor_delay_ms(D1); */
/* 	bcm2835_gpio_write(GPIO_DIRECTION, DIR_DOWN); */
/* 	motor_delay_ms(D1); */

/* 	while (bcm2835_gpio_lev(GPIO_INPUT) != LEVEL_HIGH) { */
/* 		motor_walk_one_step(1); */
/* 	} */
/* } */

/* static void motor_print_finish(void) */
/* { */
/* 	motor_delay_ms(D1); */
/* 	bcm2835_gpio_write(GPIO_DIRECTION, DIR_UP); */
/* 	motor_delay_ms(D1); */

/* 	while (bcm2835_gpio_lev(GPIO_INPUT) != LEVEL_HIGH) { */
/* 		motor_walk_one_step(1); */
/* 	} */
/* } */

void walk_one_step()
{

	bcm2835_gpio_write(GPIO_ROTOR, LEVEL_HIGH);
	motor_delay_ms(MSPEED/2);/*delay half circle*/
	bcm2835_gpio_write(GPIO_ROTOR, LEVEL_LOW);
	motor_delay_ms(MSPEED/2);

}

int  motor_walks(int direction, unsigned int step_count)
{
	int i = 0;

	motor_delay_ms(1);    // wait for a stable status
	bcm2835_gpio_write(GPIO_DIRECTION, direction);
	motor_delay_ms(1);

	for (i = 0; i < step_count; ++i) 
	{
	    	
	if((bcm2835_gpio_lev(GPIO_INPUT1) != LEVEL_HIGH)&&(direction==DIR_UP))
		return 0;
	     walk_one_step();
	}
	return 1;
}

int motor_layer(int len)
{
	int steps,gap_steps;
	/*calcualte the step which needed
	 * the mini length is 100um
	 */
	steps = len*STEP_100UM/10;
	gap_steps = GAPLEN*STEP_100UM/100;

	if(!motor_walks(DIR_UP,steps+gap_steps))
		return 0;
	if(!motor_walks(DIR_DOWN,gap_steps))
		return 0;
	return 1;
}

int motor_move_dis(int direction, int len)
{
	int steps = 0;
	
	steps = len*STEP_100UM/100;
	printf("the direction %d,%d\n",direction,len);
	if(!motor_walks(direction,steps))
		return 0;
	else
		return 1;
}

void motor_finish()
{
	printf("motor: waiting for finishing position...\n");
	while(bcm2835_gpio_lev(GPIO_INPUT1) == LEVEL_HIGH)
	{
		motor_walks(DIR_UP,100);
	}
	printf("motor: done \n");
}
