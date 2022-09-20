#ifndef _modbus_h_
#define _modbus_h_
#include "stm32f10x.h"
#include "modbus_crc.h"
#include "rs485.h"

#define find_addr   0x01  //����Ҫ��ѯ�Ĵӻ���ַ
#define start_addr  0x0000	//���Ƕ�ȡ�Ĵ�����ֵ����ʼ��ַ
#define read_num    2	//���ǲ鿴�Ĵ����ĸ����������ҪDMA�������ݣ����յ����ݸ�����read_num*2+5��


typedef struct 
{
	//��Ϊ�ӻ�ʱʹ��
    u8  myadd;        //���豸�ӻ���ַ
	u8  rcbuf[30];   //modbus���ܻ�����
	u8  recount;      //modbus�˿ڽ��յ������ݸ���
	u8  reflag;       //modbusһ֡���ݽ�����ɱ�־λ
	u8  sendbuf[40]; //modbus�ӷ��ͻ�����
	
	//��Ϊ������Ӳ���
	u8 Host_Txbuf[8];	//modbus��������	
	u8 slave_add;		//Ҫƥ��Ĵӻ��豸��ַ��������ʵ��ʱʹ�ã�
	u8 Host_send_flag;//�����豸����������ϱ�־λ
	int Host_Sendtime;//������һ֡���ݺ�ʱ�����
	u8 Host_time_flag;//����ʱ�䵽��־λ��=1��ʾ����������ʱ����
	u8 Host_End;//�������ݺ������
	u8 Host_out_flag;
}MODBUS;



extern MODBUS modbus;
extern u16 Reg[];
void Modbus_Init(void);
void Modbus_Func3(void);//���Ĵ�������
void Modbus_Func6(void);//��1���Ĵ�����д������
void Modbus_Func16(void);//������Ĵ�����д������
void Modbus_Event(void);




//void Host_RX(void);

//void RS485_Usart_SendArray(USART_TypeDef* pUSARTx,uint8_t *array,uint8_t num);



//��������


//����1�ӻ�������2��ʼ��ַ������3�Ĵ�������
void Host_Read03_slave(uint8_t slave,uint16_t StartAddr,uint16_t num);////����Ѱַ�ӻ���������

void HOST_ModbusRX(void);//�������մӻ�����Ϣ���д���

void Host_Func3(void);//��������ӻ����ص�����



//��һ���Ĵ�����д���ݵĲ�������
//����1�ӻ���ַ������2�����룬����3��ʼ��ַ������4����
void Host_write06_slave(uint8_t slave,uint8_t fun,uint16_t StartAddr,uint16_t num);

//�ӻ���������
void Host_Func6(void);



#endif

