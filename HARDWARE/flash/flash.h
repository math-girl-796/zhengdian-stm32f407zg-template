#ifndef __FLASH_H
#define __FLASH_H

#include "w25qxx.h"
#include "delay.h"
#include "malloc.h" 
#include "exfuns.h"
#include "led.h"
#include <string.h>
#include "usart.h"
#include "fattester.h"

typedef union 
{
	float f;
	struct {
		u8 b0;
		u8 b1;
		u8 b2;
		u8 b3;
	}bytes;
} byte_float;

typedef union 
{
	u32 u;
	struct {
		u8 b0;
		u8 b1;
		u8 b2;
		u8 b3;
	}bytes;
} byte_u32;

u8 flash_init(void);
u8 flash_write_pid(char* filename, float kp, float ki, float kd);
u8 flash_read_pid(char* filename, float* kp, float* ki, float* kd);

u8 flash_write_cycle(char* filename, u32 cycle);
u8 flash_read_cycle(char* filename, u32* cycle);









#endif
