#ifndef __MAIN_H
#define __MAIN_H

#include "sys.h"
#include "flash.h"
#include "key.h"
#include "keyboard.h"
#include "test.h"
#include <string.h>
#include "utils.h"

typedef struct 
{
	u16 x;
	u16 y;
}coord;

struct prog_state
{
	u8 show_vision;
} ;

void init(void);
void loop(void);
void print_param(void);

typedef struct
{
	float kp;
	float ki; 
	float kd;
} pid;



#endif
