#include "sys.h"
#include "rs485.h"	  
#include "delay.h"
#include "modbus.h"
#include "usart.h"

uint16_t res=0;  //���ڽ���ʱ����


//485���ڳ�ʼ��
//��ʼ��IO ����1 
//bound:������
void Modbus_uart1_init(u32 bound){
    //GPIO�˿�����
		GPIO_InitTypeDef GPIO_InitStructure;//GPIO�ṹ��ָ��
		USART_InitTypeDef USART_InitStructure;//���ڽṹ��ָ��
		NVIC_InitTypeDef NVIC_InitStructure;//�жϷ���ṹ��ָ��
		//1��ʹ�ܴ���ʱ�ӣ���������ʱ�� ����2���ص�APB1��
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��
		

	
	//���ͽ������ŵ�����
	 //USART1_TX   PA.9��ͼ ��֪����Ϊ���츴�������
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
		GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��PA9
   
    //USART2_RX	  PA.10����ͼ��֪�������룩
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
		GPIO_Init(GPIOA, &GPIO_InitStructure);  //��ʼ��PA10
		
		//485�շ���������PA11
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; //PA.11
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//��ͨ���������
		GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��PA11

   //4��USART ��ʼ������

		USART_InitStructure.USART_BaudRate = bound;//һ������Ϊ9600;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
		USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
		USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

		USART_Init(USART1, &USART_InitStructure); //��ʼ������
 
   //5��Usart1 NVIC ����
		NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//��ռ���ȼ�1
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//�����ȼ�1
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
		NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
   
	  //6���������������ж�
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//ʹ�ܴ��ڽ����ж�
		USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);// ʹ�ܴ��ڿ����ж�(���ڼ��һ֡���ݽ������)	
		USART_Cmd(USART1, ENABLE);// ʹ�ܴ���		
		RS485_RX_ENABLE;//ʹ�ܽ������ţ���̬�´��ڽ���״̬������ 485 оƬ�������ģʽ
}


//USART1�ж�
 void USART1_IRQHandler(void)
{
	uint16_t clr;
			if (USART_GetITStatus( USART1, USART_IT_RXNE) != RESET) //�յ�һ���ֽڵ�����
			{//������յ�������
				modbus.rcbuf[res] = USART_ReceiveData(USART1);
				res++;		
			}	
			if (USART_GetITStatus(USART1, USART_IT_IDLE) != RESET) //�յ�һ֡������
			{//�����յ������ݷ��͵����ڵ����������Ա�۲������Ƿ���ȷ
				USART_ClearFlag(USART1,USART_FLAG_IDLE);
				clr = USART1->SR;	//����������Ϊ������ж�			
				clr = USART1->DR;				
				modbus.recount=res;
				modbus.reflag=1;
				res=0;
			}
}



