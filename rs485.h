#ifndef __RS485_H
#define __RS485_H
#include "sys.h" 

#define RS485_RX_ENABLE GPIO_ResetBits(GPIOA,GPIO_Pin_11) //����ʹ�ܡ��͵�ƽ��Ч
#define RS485_TX_ENABLE GPIO_SetBits(GPIOA,GPIO_Pin_11) //����ʹ�ܡ��ߵ�ƽ��Ч

void Modbus_uart1_init(u32 bound);//��ʼ������




#endif


