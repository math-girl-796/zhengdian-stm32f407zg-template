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

u32 __ms_time_stamp = 0;
void time_init(void)
{
	TIM2_Int_Init_us(1000);
}
u32 get_time_ms(void)
{
	return __ms_time_stamp;
}

























