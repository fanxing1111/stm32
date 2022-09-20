/*********************************************************************
** 文件名称：AD7799.C
** 创 建 人：姜继勋
** 创建日期：2017.03.31
** 编译环境：IAR For ARM 7.60
** 文件说明：AD7799驱动程序。
*********************************************************************/
#include "AD7799.h"
//#include "gpio.h"
//#include "data_deal.h"//报错预警
/******************************************************************************
变量定义
******************************************************************************/
uint64_t T_cnt;//记录系统运行时间

/******************************************************************************
	延时函数
******************************************************************************/
void AD7799_Delay_ms(uint16_t time)
{
	uint16_t temp;
	while(time --)
	{
		temp = 8022;
		while(temp --);
	}
}

void AD7799_Delay_us(uint16_t time)
{
	uint8_t temp;
	while(time --)
	{
		temp = 7;
		while(temp --);
	}
}
/******************************************************************************
AD7799端口初始化
******************************************************************************/
void AD7799_IO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	
 
	GPIO_InitStructure.GPIO_Pin =  PIN_ADC_CS1|PIN_ADC_SCK1|PIN_ADC_MOSI1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = PIN_ADC_MISO1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  //上拉输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

 	//GPIO_SetBits(GPIOB,PIN_ADC_CS1|PIN_ADC_SCK1|PIN_ADC_MOSI1);
	
}

/******************************************************************************
AD7799 片选操作
******************************************************************************/
void ad7799_cs(void)
{
   ADC_CS1_L;
}

void ad7799_ncs(void)
{
   ADC_CS1_H;
}

uint8_t ad7799_rdyin(void)
{
  uint8_t sts=GPIO_ReadInputDataBit(GPIOB,PIN_ADC_MISO1);
  return sts;
}

/******************************************************************************
	AD7799 总线状态复位
******************************************************************************/
void ad7799_rst(void)
{
  uint8_t resettime=32;
  
  AD7799_Delay_ms(200);  
  while(resettime--)
  {
          AD7799_Delay_us(10);
          ADC_SCK1_L;
          AD7799_Delay_us(10);
          ADC_SCK1_H;
  }
  AD7799_Delay_ms(200);
}


/******************************************************************************
	AD7799 写一个字节
******************************************************************************/
void ad7799_writebyte(uint8_t word)   
{ 
  uint8_t i;
  {
          for(i=0;i<8;i++)
          {
                  if(word & 0x80)			//输出数据
                          ADC_MOSI1_H;
                  else 		
                          ADC_MOSI1_L;
                  ADC_SCK1_L;				//时钟下沿
                  word <<= 1;				//准备下一位数据
                  ADC_SCK1_H;				//时钟上沿
          } 
  }
}

/******************************************************************************
	AD7799 读取一个字节
******************************************************************************/
uint8_t ad7799_readbyte(void)
{
	uint8_t i;
	uint8_t rd_data=0;	
        ADC_MOSI1_H;
        for(i=0;i<8;i++)
        {
                ADC_SCK1_L;				//时钟下沿
                rd_data <<= 1;
                if(GPIO_ReadInputDataBit(GPIOB,PIN_ADC_MISO1) == Bit_SET)		//接收数据
                        rd_data |= 0x01;
                
                ADC_SCK1_H;				//时钟上沿
        } 
	return rd_data;
}
/******************************************************************************
	AD7799 读取24位数据
******************************************************************************/
uint32_t ad7799_readword()
{
	uint8_t i;
	uint32_t rd_data=0;		
        ADC_MOSI1_H;
        for(i=0;i<24;i++)
        {
                ADC_SCK1_L;				//时钟下沿
                rd_data <<= 1;
                if(GPIO_ReadInputDataBit(GPIOB,PIN_ADC_MISO1) == Bit_SET)		//接收数据
                        rd_data |= 0x01;
                
                ADC_SCK1_H;				//时钟上沿
        } 		
	return rd_data;
}

/******************************************************************************
	AD7799 配置工作模式 
//ub:单、双极性，0为双极性、1为单极性
******************************************************************************/
void config_admode(uint8_t ub,uint8_t mult_num)
{
  uint8_t mode=0;
  
  if(ub==1)
  {
    mode |=0x10;
  }
  else if(ub==0)
    mode &=0xEF;
  
  switch(mult_num)
  {
  case 1:mode &=0xf8;mode |=0x02;
    break;
  case 2:mode &=0xf8;mode |=0x01;
    break;
  case 4:mode &=0xf8;mode |=0x02;
    break;
  case 8:mode &=0xf8;mode |=0x03;
    break;
  case 16:mode &=0xf8;mode |=0x04;
    break;
  case 32:mode &=0xf8;mode |=0x05;
    break;
  case 64:mode &=0xf8;mode |=0x06;
    break;
  case 128:
    mode |=0x07;
    break;    
  }  
  ad7799_writebyte(mode);
}
/******************************************************************************
	AD7799 初始化
******************************************************************************/
void ad7799_init(uint8_t ch,uint8_t ub,uint8_t mult)
{
  ad7799_writebyte(0xff);
  ad7799_writebyte(0xff);
  ad7799_writebyte(0xff);
  ad7799_writebyte(0xff);
  
  ad7799_cs();
  
  ad7799_writebyte(0x10);  //选择配置寄存器      
  config_admode(ub,mult);  //选择极性和放大倍数  
  if(ch==1)
    ad7799_writebyte(0x30); //选择通道1
  else if (ch==2)
    ad7799_writebyte(0x31); //选择通道2
  else if (ch==3)
    ad7799_writebyte(0x32); //选择通道3  
  
  
  ad7799_writebyte(0x08);  //选择模式寄存器
  ad7799_writebyte(0x80);  //内部0校准
  ad7799_writebyte(0x0f);  //更新频率 
  AD7799_Delay_ms(10);
  
  ad7799_writebyte(0x08);  //选择模式寄存器
  ad7799_writebyte(0xa0);  //内部满校准
  ad7799_writebyte(0x0f);  
  AD7799_Delay_ms(10);
  
  ad7799_writebyte(0x08);  //选择模式寄存器
  ad7799_writebyte(0x00);  //连续转换模式
  ad7799_writebyte(0x0f);  //更新频率   
  
  ad7799_ncs();
}
/******************************************************************************
	AD7799 通道初始化
******************************************************************************/
void ad7799ch_init(uint8_t ch,uint8_t ub,uint8_t mult)
{
  ad7799_writebyte(0xff);//需要72个时钟？？？？
  ad7799_writebyte(0xff);
  ad7799_writebyte(0xff);
  ad7799_writebyte(0xff);
  
  ad7799_cs();
  
  ad7799_writebyte(0x10); //选择配置寄存器   
  if(ch==1)
    ad7799_writebyte(0x30); //选择通道1
  else
    ad7799_writebyte(0x31); //选择通道2
  
  config_admode(ub,mult);//选择极性和放大倍数 
  
  ad7799_writebyte(0x08); //选择模式寄存器
  ad7799_writebyte(0x00);//连续转换模式
  ad7799_writebyte(0x0f);//更新频率   
  
  ad7799_ncs();
}
/******************************************************************************
	AD7799 选择通道
******************************************************************************/
void ad7799_choose_ch(uint8_t ch,uint8_t ub,uint8_t mult)
{  
  ad7799_cs();
  
  ad7799_writebyte(0x10); //选择配置寄存器      
  config_admode(ub,mult);   
  if(ch==1)   
    ad7799_writebyte(0x30); //选择通道1
  else
    ad7799_writebyte(0x31); //选择通道2  
  
   ad7799_ncs();
}
extern uint64_t T_cnt;//记录系统运行时间
/******************************************************************************
	AD7799 读数据
******************************************************************************/
uint32_t read_ad7799(void)
{
  uint8_t  flag;
  long addata;  
  uint64_t L_T_cnt=T_cnt; 
  ad7799_cs();
//  while(ad7799_rdyin())
//	{
//			if(T_cnt-L_T_cnt>3000)//溢出3S时间，基本判断坏了
//			{
////				  uint32_t erro_code=0x01;
////			    set_data_channel(Data_erro_ord,(uint8_t *)&erro_code);//装入报错代码
//					return 0;//返回一个0
//			}
//	}
  flag=0x80;
  while(((flag&0x80)==0x80))
  { 
    AD7799_Delay_ms(AD_DELAY1); 
    ad7799_writebyte(0x40);//状态寄存器
    flag = ad7799_readbyte();//状态寄存器
  }
  AD7799_Delay_ms(AD_DELAY2);//为什么需要？
  ad7799_writebyte(0x58);//数据寄存器
  addata = ad7799_readword();//返回读取数据
  AD7799_Delay_ms(AD_DELAY3); //为什么需要？
  
  ad7799_ncs();
  
  return  addata;
}
/******************************************************************************
	AD7799 配置
******************************************************************************/
void ad7799_config(void)
{
//  ad7799_init(1,1,AD_PREUB,PRE_MULT);
//  ad7799_init(1,2,AD_PREUB,DIFPRE_MULT);
//
//  adjust_zero(1,1,AD_PREUB,PRE_MULT);
//  adjust_zero(1,2,AD_PREUB,DIFPRE_MULT);   
//  timer_delay1s(15);   
//  adjust_zero(1,1,AD_PREUB,PRE_MULT);
//  adjust_zero(1,2,AD_PREUB,DIFPRE_MULT);   
}

uint32_t AD7799_getID(void)
{
	unsigned char data=0x00;
	unsigned long receivedData = 0x00;	
	ad7799_cs();  
	ad7799_writebyte((1 << 6)|(((4) & 0x7) << 3));
	data=ad7799_readbyte();
	ad7799_ncs();
	receivedData += data;
  return receivedData;
}
float AD7799_getV(uint8_t channel)
{
		float v; 
		ad7799_init(channel,AD_UB_U,AD7799_CONFIG_GAIN_1);
		v = 2.5*read_ad7799()/0x1000000;    //若增益不为1，还需除以增益
	    return v;
}










