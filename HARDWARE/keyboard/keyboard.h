#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include "sys.h"
#include "usbh_usr.h"
#include "delay.h"
#include <string.h>
#include "led.h"
#include "usb_conf.h" 

//////////////// 占用管脚PA11、PA12


void keyboard_init(void); 					// 初始化键盘

// 检查一次键盘连接，如果通信异常或者死机了，就修复。然后递归检查，如果仍然异常，就继续修复，直到修好为止。
// 检查期间LED0会快速闪烁
// 不连接键盘不会导致卡死（大概吧，这玩意一般也没法测试）

// 初始化后需要运行此函数好多轮才能连接成功，因此一上来应该想办法快速多次运行该函数，直到连接成功，然后再定期检查
void keyboard_check(void);

int keyboard_buf_state(void); 				// 返回缓冲区中字符个数，范围在0~KEYBOARD_BUF_MAX_LEN之间
u8 keyboard_LF(void); 						// 返回用户最后按下的是否为换行符。缓冲区中字符由所有可见字符组成。如果缓冲区为空，返回0。
void keyboard_read_buf(u8* buf, int len); 	// 将键盘缓冲区中前len个字符读入buf，然后清空键盘缓冲区
void keyboard_clear_buf(void);				// 清空键盘缓冲区















#endif
