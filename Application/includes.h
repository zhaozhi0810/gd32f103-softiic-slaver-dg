
#ifndef INCLUDES_H
#define INCLUDES_H

//�����ܵ�ͷ�ļ�����

#include <gd32f10x.h>
#include <stdio.h>
//#include <stdarg.h>
#include <limits.h>
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <atomic.h>
#include <event_groups.h>
#include <stream_buffer.h>


#include <string.h>

//���ܿ��ƺ궨�壺

#define BTNS_USE_INT   //����ɨ��ʹ���жϷ�ʽ
//#define HWTD_USE_INT   //Ӳ�����Ź�ʹ���ⲿ�жϣ��½��ص�ƽ����
#define KLEDS_PWM  //�������ȿ���ʹ��pwm��ʽ��2022-10-10

#define HWTD_USE_INT   //Ӳ�����Ź�ʹ���ⲿ�жϣ��½��ص�ƽ����
#define LEDS_FLASH_TASK  //led���Ƶ���˸�ɵ�Ƭ������,��Ϊ��ʱ������

extern const char* g_build_time_str;

#define DEBUG_COM_NUM 0   //���Դ��ں�
#define TOCPU_COM_NUM 1   //��cpuͨ�ŵĴ���


#define UNUSED(x)  

#include "uart.h"
#include "iic.h"
#include "AT9236.h"
#include "main.h"
#include "gd32f103c_eval.h"
#include "gpio.h"

#include "gd32f10x_it.h"
#include "systick.h"
#include "soft_iic_master_app.h"
#include "software_iic_slave.h"

#include "uart_debug_handle.h"
//#include "keys_scan.h"


//��������汾2023-01-15
extern uint8_t GetMcuVersion(void);

extern uint8_t more_debug_info;   //��ӡ����ĵ�����Ϣ��0�򲻴�ӡ�����Դ��ڿ���
#endif
