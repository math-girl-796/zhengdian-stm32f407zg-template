#ifndef __UTILS_H
#define __UTILS_H
#include "sys.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "timer.h"

// ���number����������i��f��ͬ�����number�Ǹ���������i��f�������������������ת��ʧ�ܣ�����0
typedef struct
{
	int i;
	float f;
} number;


// bytes������/С���ַ���������������Ƿ������ݸ�ʽ���������Ը�
// ע�⣬���ô˺������������ڻ���������ʱ��Ҫ�ڴ���֮ǰ�����������ճ��ȴ��ֶ���Ϊ\x00��
// bad case����һ�δ�uart��������������������buf����222�����ڻ�����buf��ʼ��Ϊȫ�㣬����ֱ�ӽ�buf�����˺���ʱ��string��222��
// �ڶ��δ�uart��������������������buf����1������û�ж�buf�����ദ��string��122����͵����˴���
number parse_string(char* string);

void time_init(void);
u32 get_time_ms(void);














#endif

