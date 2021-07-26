#include "utils.h"
number parse_string(char* string)
{
	number ret;
	ret.f = 0;
	ret.i = 0;
	
	ret.f = atof(string);
	ret.i = (int) round(ret.f);
	
	return ret;
}

extern u32 __ms_time_stamp;
void time_init(void)
{
	TIM2_Int_Init_us(1000);
}
u32 get_time_ms(void)
{
	return __ms_time_stamp;
}

u32 clip_500_2500(u32 input)
{
	if (input > 2500) return 2500;
	else if (input < 500) return 500;
	else return input;
}

u32 clip_500_1500(u32 input)
{
	if (input > 1500) return 1500;
	else if (input < 500) return 500;
	else return input;
}

u32 clip_1500_2500(u32 input)
{
	if (input > 2500) return 2500;
	else if (input < 1500) return 1500;
	else return input;
}



















