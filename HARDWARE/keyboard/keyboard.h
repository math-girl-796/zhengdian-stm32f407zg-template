#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include "sys.h"
#include "usbh_usr.h"
#include "delay.h"
#include <string.h>
#include "led.h"
#include "usb_conf.h" 

//////////////// ռ�ùܽ�PA11��PA12


void keyboard_init(void); 					// ��ʼ������

// ���һ�μ������ӣ����ͨ���쳣���������ˣ����޸���Ȼ��ݹ��飬�����Ȼ�쳣���ͼ����޸���ֱ���޺�Ϊֹ��
// ����ڼ�LED0�������˸
// �����Ӽ��̲��ᵼ�¿�������Űɣ�������һ��Ҳû�����ԣ�

// ��ʼ������Ҫ���д˺����ö��ֲ������ӳɹ������һ����Ӧ����취���ٶ�����иú�����ֱ�����ӳɹ���Ȼ���ٶ��ڼ��
void keyboard_check(void);

int keyboard_buf_state(void); 				// ���ػ��������ַ���������Χ��0~KEYBOARD_BUF_MAX_LEN֮��
u8 keyboard_LF(void); 						// �����û�����µ��Ƿ�Ϊ���з������������ַ������пɼ��ַ���ɡ����������Ϊ�գ�����0��
void keyboard_read_buf(u8* buf, int len); 	// �����̻�������ǰlen���ַ�����buf��Ȼ����ռ��̻�����
void keyboard_clear_buf(void);				// ��ռ��̻�����















#endif
