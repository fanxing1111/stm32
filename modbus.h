#ifndef _modbus_h_
#define _modbus_h_
#include "stm32f10x.h"
#include "modbus_crc.h"
#include "rs485.h"

#define find_addr   0x01  //这是要查询的从机地址
#define start_addr  0x0000	//这是读取寄存器数值的起始地址
#define read_num    2	//这是查看寄存器的个数（如果想要DMA接收数据，接收的数据个数：read_num*2+5）


typedef struct 
{
	//作为从机时使用
    u8  myadd;        //本设备从机地址
	u8  rcbuf[30];   //modbus接受缓冲区
	u8  recount;      //modbus端口接收到的数据个数
	u8  reflag;       //modbus一帧数据接受完成标志位
	u8  sendbuf[40]; //modbus接发送缓冲区
	
	//作为主机添加部分
	u8 Host_Txbuf[8];	//modbus发送数组	
	u8 slave_add;		//要匹配的从机设备地址（做主机实验时使用）
	u8 Host_send_flag;//主机设备发送数据完毕标志位
	int Host_Sendtime;//发送完一帧数据后时间计数
	u8 Host_time_flag;//发送时间到标志位，=1表示到发送数据时间了
	u8 Host_End;//接收数据后处理完毕
	u8 Host_out_flag;
}MODBUS;



extern MODBUS modbus;
extern u16 Reg[];
void Modbus_Init(void);
void Modbus_Func3(void);//读寄存器数据
void Modbus_Func6(void);//往1个寄存器中写入数据
void Modbus_Func16(void);//往多个寄存器中写入数据
void Modbus_Event(void);




//void Host_RX(void);

//void RS485_Usart_SendArray(USART_TypeDef* pUSARTx,uint8_t *array,uint8_t num);



//主机部分


//参数1从机，参数2起始地址，参数3寄存器个数
void Host_Read03_slave(uint8_t slave,uint16_t StartAddr,uint16_t num);////主机寻址从机参数设置

void HOST_ModbusRX(void);//主机接收从机的消息进行处理

void Host_Func3(void);//主机计算从机返回的数据



//向一个寄存器中写数据的参数设置
//参数1从机地址，参数2功能码，参数3起始地址，参数4数据
void Host_write06_slave(uint8_t slave,uint8_t fun,uint16_t StartAddr,uint16_t num);

//从机返回数据
void Host_Func6(void);



#endif

