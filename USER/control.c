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
























