#include "sys.h"
#include "rs485.h"	  
#include "delay.h"
#include "modbus.h"
#include "usart.h"

uint16_t res=0;  //用于接收时计数


//485串口初始化
//初始化IO 串口1 
//bound:波特率
void Modbus_uart1_init(u32 bound){
    //GPIO端口设置
		GPIO_InitTypeDef GPIO_InitStructure;//GPIO结构体指针
		USART_InitTypeDef USART_InitStructure;//串口结构体指针
		NVIC_InitTypeDef NVIC_InitStructure;//中断分组结构体指针
		//1、使能串口时钟，串口引脚时钟 串口2挂载到APB1上
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
		

	
	//发送接收引脚的设置
	 //USART1_TX   PA.9由图 可知设置为推挽复用输出）
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
		GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化PA9
   
    //USART2_RX	  PA.10（有图可知浮空输入）
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
		GPIO_Init(GPIOA, &GPIO_InitStructure);  //初始化PA10
		
		//485收发控制引脚PA11
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; //PA.11
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//普通的推挽输出
		GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化PA11

   //4、USART 初始化设置

		USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
		USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
		USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

		USART_Init(USART1, &USART_InitStructure); //初始化串口
 
   //5、Usart1 NVIC 配置
		NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//抢占优先级1
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//子优先级1
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
		NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
   
	  //6、开启接收数据中断
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//使能串口接收中断
		USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);// 使能串口空闲中断(用于检测一帧数据接收完毕)	
		USART_Cmd(USART1, ENABLE);// 使能串口		
		RS485_RX_ENABLE;//使能接收引脚（常态下处于接收状态）控制 485 芯片进入接收模式
}


//USART1中断
 void USART1_IRQHandler(void)
{
	uint16_t clr;
			if (USART_GetITStatus( USART1, USART_IT_RXNE) != RESET) //收到一个字节的数据
			{//保存接收到的数据
				modbus.rcbuf[res] = USART_ReceiveData(USART1);
				res++;		
			}	
			if (USART_GetITStatus(USART1, USART_IT_IDLE) != RESET) //收到一帧的数据
			{//将接收到的数据发送到串口调试助手上以便观察数据是否正确
				USART_ClearFlag(USART1,USART_FLAG_IDLE);
				clr = USART1->SR;	//读这两个是为了清除中断			
				clr = USART1->DR;				
				modbus.recount=res;
				modbus.reflag=1;
				res=0;
			}
}



