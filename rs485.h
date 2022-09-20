#ifndef __RS485_H
#define __RS485_H
#include "sys.h" 

#define RS485_RX_ENABLE GPIO_ResetBits(GPIOA,GPIO_Pin_11) //接收使能、低电平有效
#define RS485_TX_ENABLE GPIO_SetBits(GPIOA,GPIO_Pin_11) //发送使能、高电平有效

void Modbus_uart1_init(u32 bound);//初始化函数




#endif


