#include "control.h"

// ����main.c
extern pid pid1;
extern pid pid2;
extern u32 last_ctrl_ms; // �ϴ��жϴ���������ʱ��
extern coord obj; // �Ӿ�����
extern coord tgt; // Ŀ������

extern coord err;
extern coord ierr;
extern coord derr;
extern coord lerr; // last_error
extern coord steer_compare; // �����compareֵ
extern struct prog_state prog_state;

/* �˴�д���ƴ��� */
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //����ж�
	{
		
		//��¼��ʼʱ��
		u32 ctrl_start_ms = get_time_ms();
		
		err.x = tgt.x - obj.x;
		err.y = tgt.y - obj.y;
		
		ierr.x += err.x;
		ierr.y += err.y;
		
		derr.x = err.x - lerr.x;
		derr.y = err.y - lerr.y;
		
		lerr = err;
		
		int derta_compare1 = (int) (pid1.kp * err.x + pid1.kd * derr.x + pid1.ki * ierr.x);
		int derta_compare2 = (int) (pid2.kp * err.y + pid2.kd * derr.y + pid2.ki * ierr.y);
		
		steer_compare.x = clip_1500_2500(steer_compare.x + derta_compare1);
		steer_compare.y = clip_500_1500(steer_compare.y + derta_compare2);
		
//		printf("tgt: %d  %d    obj: %d  %d\r\n", tgt.x, tgt.y, obj.x, obj.y);
//		printf("derta_compare: %d  %d\r\n", derta_compare1, derta_compare2);
//		printf("err: %d  %d\r\n compare: %d %d\r\n", err.x, err.y, steer_compare.x, steer_compare.y);
		
		steer1_set_compare(steer_compare.y);
		steer2_set_compare(steer_compare.x);
		
		led_switch(LED1);
				
		
		//��¼�жϴ���������ʱ��
		last_ctrl_ms = get_time_ms() - ctrl_start_ms;
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //����жϱ�־λ
}























