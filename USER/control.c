#include "control.h"

// ����main.c
extern float kp, ki, kd;// pid����
extern u32 ctrl_cycle; // ��ʱ����ʱ���ڣ���λus
extern struct obj obj; // �Ӿ�����

/* �˴�д���ƴ��� */
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //����ж�
	{
		led_switch(LED0);
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //����жϱ�־λ
}























