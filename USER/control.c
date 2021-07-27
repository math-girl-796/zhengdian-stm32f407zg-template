#include "control.h"

// 来自main.c
extern pid pid1;
extern pid pid2;
extern u32 last_ctrl_ms; // 上次中断处理函数运行时间
extern coord obj; // 视觉坐标
extern coord tgt; // 目标坐标

extern coord err;
extern coord ierr;
extern coord derr;
extern coord lerr; // last_error
extern coord steer_compare; // 舵机的compare值
extern struct prog_state prog_state;
























