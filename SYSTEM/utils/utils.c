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
//	int integral_flag = 1; // ���flagΪ1��˵�����ڴ����������֣����flagΪ0��˵�����ڴ���С������
//	float decimal_factor = 0.1;
//	
//	int length = strlen((char*) bytes);
//	
//	if (length == 0) return ret; // �������Ϊ�շ���0
//	
//	// ��ȡ����
//	if (bytes[0] == '-') sign = -1;
//	else sign = 1;
//	cursor ++;
//	
//	while(cursor < length)
//	{
//		// ����С����
//		if(bytes[cursor] == '.') 
//		{
//			integral_flag = 0;
//			cursor ++;
//			continue;
//		}
//		
//		// ������������
//		if (integral_flag == 1)
//		{
//			ret.f *= 10;
//			ret.f += bytes[cursor];
//			cursor ++;
//		}
//		
//		// ����С������
//		if (integral_flag == 0)
//		{
//			ret.f += bytes[cursor] * decimal_factor;
//			decimal_factor /= 10;
//			cursor ++;
//		}
//	}
}


