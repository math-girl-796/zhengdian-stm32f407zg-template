#include "utils.h"
number parse_string(char* string)
{
	number ret;
	ret.f = 0;
	ret.i = 0;
	
	ret.f = atof(string);
	ret.i = (int) round(ret.f);
	
	return ret;
	
//	int sign;
//	int cursor = 0;
//	
//	int integral_flag = 1; // 如果flag为1，说明正在处理整数部分；如果flag为0，说明正在处理小数部分
//	float decimal_factor = 0.1;
//	
//	int length = strlen((char*) bytes);
//	
//	if (length == 0) return ret; // 如果输入为空返回0
//	
//	// 读取符号
//	if (bytes[0] == '-') sign = -1;
//	else sign = 1;
//	cursor ++;
//	
//	while(cursor < length)
//	{
//		// 处理小数点
//		if(bytes[cursor] == '.') 
//		{
//			integral_flag = 0;
//			cursor ++;
//			continue;
//		}
//		
//		// 处理整数部分
//		if (integral_flag == 1)
//		{
//			ret.f *= 10;
//			ret.f += bytes[cursor];
//			cursor ++;
//		}
//		
//		// 处理小数部分
//		if (integral_flag == 0)
//		{
//			ret.f += bytes[cursor] * decimal_factor;
//			decimal_factor /= 10;
//			cursor ++;
//		}
//	}
}


