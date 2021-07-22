#ifndef __UTILS_H
#define __UTILS_H
#include "sys.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "timer.h"

// 如果number是整数，则i与f相同；如果number是浮点数，则i是f的四舍五入整数；如果转换失败，返回0
typedef struct
{
	int i;
	float f;
} number;


// bytes：整数/小数字符串。不允许输入非法的数据格式，否则后果自负
// 注意，当用此函数连续处理串口缓冲区数据时，要在传参之前将缓冲区接收长度处手动置为\x00。
// bad case：第一次从uart缓冲区向主函数缓冲区buf读入222，由于缓冲区buf初始化为全零，所以直接将buf传给此函数时，string是222；
// 第二次从uart缓冲区向主函数缓冲区buf读入1，由于没有对buf做更多处理，string是122，这就导致了错误
number parse_string(char* string);

void time_init(void);
u32 get_time_ms(void);














#endif

