#include "FDC2214.h"
#include "myiic.h"
#include "delay.h"

//u16 LDC1314_Read2Byte(u8 reg_addr)
//{
//     u16 reg_data=0;
//     u16 temp=0;
//     IIC_Start();
//     IIC_Send_Byte(WRITE_ADDR); 
//     if(IIC_Wait_Ack())return 0;
//     IIC_Send_Byte(reg_addr); 
//	   if(IIC_Wait_Ack())return 0;
//     IIC_Start();
//	   IIC_Send_Byte(READ_ADDR);
//     if(IIC_Wait_Ack())return 0;
//     reg_data= IIC_Read_Byte(1);
//     reg_data=(reg_data<<8)&0xFF00; 
//     temp=IIC_Read_Byte(0);
//     IIC_Stop();	
//		 reg_data|=(temp&0x00FF);
//		 return reg_data;
//}

u16 FDC2214_Read2Byte(u8 reg_addr)
{
			u16 reg_data=0;
			u16 temp=0;
			IIC_Start();
			IIC_Send_Byte(WRITE_ADDR);
			if(IIC_Wait_Ack())return 0;
			IIC_Send_Byte(reg_addr);   
			if(IIC_Wait_Ack())return 0;
			IIC_Start();
			IIC_Send_Byte(READ_ADDR);
			if(IIC_Wait_Ack())return 0;
			reg_data= IIC_Read_Byte(1);
			reg_data=(reg_data<<8)&0xFF00;
			temp=IIC_Read_Byte(0);
			IIC_Stop();
			reg_data|=temp;
			return reg_data;
}


u8 FDC2214_Write2Byte(u8 reg_addr,u16 reg_data)
{        
   u8 data_high=(u8)((reg_data&0xFF00)>>8);
   u8 data_low=(u8)reg_data&0x00FF;
	 IIC_Start();
	 IIC_Send_Byte(WRITE_ADDR);   
   if(IIC_Wait_Ack())return 0;
   IIC_Send_Byte(reg_addr );    
	 if(IIC_Wait_Ack())return 0;        
   IIC_Send_Byte(data_high);
   if(IIC_Wait_Ack())return 0;        
   IIC_Send_Byte(data_low);
   if(IIC_Wait_Ack())return 0;                 
   IIC_Stop();
	 delay_ms(5);
   return 1;
}

void FDC2214_Init_Multi()
{        
	FDC2214_Write2Byte(RCOUNT0,0x8329);   //(CHx_RCOUNT*16)/55M ==9.76ms��,ÿ10ms���ҿ��Զ�һ��ֵ
	FDC2214_Write2Byte(RCOUNT1,0x8329);
  FDC2214_Write2Byte(RCOUNT2,0x8329);	
	FDC2214_Write2Byte(RCOUNT3,0x8329);
	
	FDC2214_Write2Byte(SETTLECOUNT0,0x0400);  //����4��ͨ����С�ȶ�ʱ��
	FDC2214_Write2Byte(SETTLECOUNT1,0x0400);
	FDC2214_Write2Byte(SETTLECOUNT2,0x0400);
	FDC2214_Write2Byte(SETTLECOUNT3,0x0400);
	
  FDC2214_Write2Byte(CLK_DIVIDERS0,0x1001); //ʱ�ӳ���1�����ô�����Ƶ����0.01M��8.5M֮��
	FDC2214_Write2Byte(CLK_DIVIDERS1,0x1001);
	FDC2214_Write2Byte(CLK_DIVIDERS2,0x1001);
	FDC2214_Write2Byte(CLK_DIVIDERS3,0x1001);

	FDC2214_Write2Byte(ERROR_CONFIG,0x0000); //�������жϱ�־λ
	FDC2214_Write2Byte(MUX_CONFIG,0xC20D);//ʹ��0,1,2,3ͨ�����Ҵ�������Ϊ10M
 
	FDC2214_Write2Byte(DRIVE_CURRENT0,0x8000); //����4��ͨ������������
  FDC2214_Write2Byte(DRIVE_CURRENT1,0x8000);
	FDC2214_Write2Byte(DRIVE_CURRENT2,0x8000);
	FDC2214_Write2Byte(DRIVE_CURRENT3,0x8000);
 
	FDC2214_Write2Byte(CONFIG,0x1401);	//ʹ��FDC2214,��ȡ�ڲ�ʱ��Ϊ�ο�ʱ��
}

u16 FDC2214_read_CHx(u8 CHx)	//��Channel x Conversion Result
{
	u8 reg_addr1 = 0;	
	u16 val = 0;				
	switch(CHx)
	{
		case 0:
		{
			reg_addr1 = DATA0_MSB;
		}break;
		case 1:
		{
			reg_addr1 = DATA1_MSB;
		}break;
		case 2:
		{
			reg_addr1 = DATA2_MSB;
		}break;
		case 3:
		{
			reg_addr1 = DATA3_MSB;
		}break;
		default: return 0;	//�������
	}
	
	val = FDC2214_Read2Byte(reg_addr1);	//��ȡ�Ĵ���ֵ
	return val;	
}

/*float LDC_Get_Fsensor(u8 CHx) //�����ֲṫʽ ���㴫����Ƶ��
{
	u8 reg_addr1 = 0;	
	u16 CHx_FIN_DIVIDER_value=1;  //�ɼ�����ȦƵ�� ����Ƶ
	u16 CHx_FREF_DIVIDER_value=20; ////�ⲿ���ʱ�ӷ�Ƶϵ��  �ⲿʱ��Ϊ40MHz
	u16 CHx_DATA_value=0; //ͨ������
	u16 CHx_OFFSET_value=0x2000;
	u16 OUTPUT_value=0; //���ݳ�ʼ���Ĵ���ʱ�޸�
	float val = 0;		
	switch(CHx)
	{
		case 0:
		{
			reg_addr1 = DATA0_MSB;
		}break;
		case 1:
		{
			reg_addr1 = DATA1_MSB;
		}break;
		case 2:
		{
			reg_addr1 = DATA2_MSB;
		}break;
		case 3:
		{
			reg_addr1 = DATA3_MSB;
		}break;
		default: return 0;	//�������
	}
	CHx_DATA_value = LDC1314_Read2Byte(reg_addr1);//��ȡͨ���Ĵ���ֵ  ��4λΪ�����־λ ��12λΪ��Ч����
	
	if((CHx_DATA_value & 0xF000)==0)
	{
		CHx_DATA_value &= 0x0FFF;
		val = 1.0*(40000000/2) * ( (float)CHx_DATA_value/(4096*8) + (CHx_OFFSET_value/65536) );
	}
	return val;	
}*/

 