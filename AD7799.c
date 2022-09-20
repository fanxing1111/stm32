/*********************************************************************
** �ļ����ƣ�AD7799.C
** �� �� �ˣ�����ѫ
** �������ڣ�2017.03.31
** ���뻷����IAR For ARM 7.60
** �ļ�˵����AD7799��������
*********************************************************************/
#include "AD7799.h"
//#include "gpio.h"
//#include "data_deal.h"//����Ԥ��
/******************************************************************************
��������
******************************************************************************/
uint64_t T_cnt;//��¼ϵͳ����ʱ��

/******************************************************************************
	��ʱ����
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
AD7799�˿ڳ�ʼ��
******************************************************************************/
void AD7799_IO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	
 
	GPIO_InitStructure.GPIO_Pin =  PIN_ADC_CS1|PIN_ADC_SCK1|PIN_ADC_MOSI1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = PIN_ADC_MISO1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  //��������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

 	//GPIO_SetBits(GPIOB,PIN_ADC_CS1|PIN_ADC_SCK1|PIN_ADC_MOSI1);
	
}

/******************************************************************************
AD7799 Ƭѡ����
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
	AD7799 ����״̬��λ
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
	AD7799 дһ���ֽ�
******************************************************************************/
void ad7799_writebyte(uint8_t word)   
{ 
  uint8_t i;
  {
          for(i=0;i<8;i++)
          {
                  if(word & 0x80)			//�������
                          ADC_MOSI1_H;
                  else 		
                          ADC_MOSI1_L;
                  ADC_SCK1_L;				//ʱ������
                  word <<= 1;				//׼����һλ����
                  ADC_SCK1_H;				//ʱ������
          } 
  }
}

/******************************************************************************
	AD7799 ��ȡһ���ֽ�
******************************************************************************/
uint8_t ad7799_readbyte(void)
{
	uint8_t i;
	uint8_t rd_data=0;	
        ADC_MOSI1_H;
        for(i=0;i<8;i++)
        {
                ADC_SCK1_L;				//ʱ������
                rd_data <<= 1;
                if(GPIO_ReadInputDataBit(GPIOB,PIN_ADC_MISO1) == Bit_SET)		//��������
                        rd_data |= 0x01;
                
                ADC_SCK1_H;				//ʱ������
        } 
	return rd_data;
}
/******************************************************************************
	AD7799 ��ȡ24λ����
******************************************************************************/
uint32_t ad7799_readword()
{
	uint8_t i;
	uint32_t rd_data=0;		
        ADC_MOSI1_H;
        for(i=0;i<24;i++)
        {
                ADC_SCK1_L;				//ʱ������
                rd_data <<= 1;
                if(GPIO_ReadInputDataBit(GPIOB,PIN_ADC_MISO1) == Bit_SET)		//��������
                        rd_data |= 0x01;
                
                ADC_SCK1_H;				//ʱ������
        } 		
	return rd_data;
}

/******************************************************************************
	AD7799 ���ù���ģʽ 
//ub:����˫���ԣ�0Ϊ˫���ԡ�1Ϊ������
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
	AD7799 ��ʼ��
******************************************************************************/
void ad7799_init(uint8_t ch,uint8_t ub,uint8_t mult)
{
  ad7799_writebyte(0xff);
  ad7799_writebyte(0xff);
  ad7799_writebyte(0xff);
  ad7799_writebyte(0xff);
  
  ad7799_cs();
  
  ad7799_writebyte(0x10);  //ѡ�����üĴ���      
  config_admode(ub,mult);  //ѡ���ԺͷŴ���  
  if(ch==1)
    ad7799_writebyte(0x30); //ѡ��ͨ��1
  else if (ch==2)
    ad7799_writebyte(0x31); //ѡ��ͨ��2
  else if (ch==3)
    ad7799_writebyte(0x32); //ѡ��ͨ��3  
  
  
  ad7799_writebyte(0x08);  //ѡ��ģʽ�Ĵ���
  ad7799_writebyte(0x80);  //�ڲ�0У׼
  ad7799_writebyte(0x0f);  //����Ƶ�� 
  AD7799_Delay_ms(10);
  
  ad7799_writebyte(0x08);  //ѡ��ģʽ�Ĵ���
  ad7799_writebyte(0xa0);  //�ڲ���У׼
  ad7799_writebyte(0x0f);  
  AD7799_Delay_ms(10);
  
  ad7799_writebyte(0x08);  //ѡ��ģʽ�Ĵ���
  ad7799_writebyte(0x00);  //����ת��ģʽ
  ad7799_writebyte(0x0f);  //����Ƶ��   
  
  ad7799_ncs();
}
/******************************************************************************
	AD7799 ͨ����ʼ��
******************************************************************************/
void ad7799ch_init(uint8_t ch,uint8_t ub,uint8_t mult)
{
  ad7799_writebyte(0xff);//��Ҫ72��ʱ�ӣ�������
  ad7799_writebyte(0xff);
  ad7799_writebyte(0xff);
  ad7799_writebyte(0xff);
  
  ad7799_cs();
  
  ad7799_writebyte(0x10); //ѡ�����üĴ���   
  if(ch==1)
    ad7799_writebyte(0x30); //ѡ��ͨ��1
  else
    ad7799_writebyte(0x31); //ѡ��ͨ��2
  
  config_admode(ub,mult);//ѡ���ԺͷŴ��� 
  
  ad7799_writebyte(0x08); //ѡ��ģʽ�Ĵ���
  ad7799_writebyte(0x00);//����ת��ģʽ
  ad7799_writebyte(0x0f);//����Ƶ��   
  
  ad7799_ncs();
}
/******************************************************************************
	AD7799 ѡ��ͨ��
******************************************************************************/
void ad7799_choose_ch(uint8_t ch,uint8_t ub,uint8_t mult)
{  
  ad7799_cs();
  
  ad7799_writebyte(0x10); //ѡ�����üĴ���      
  config_admode(ub,mult);   
  if(ch==1)   
    ad7799_writebyte(0x30); //ѡ��ͨ��1
  else
    ad7799_writebyte(0x31); //ѡ��ͨ��2  
  
   ad7799_ncs();
}
extern uint64_t T_cnt;//��¼ϵͳ����ʱ��
/******************************************************************************
	AD7799 ������
******************************************************************************/
uint32_t read_ad7799(void)
{
  uint8_t  flag;
  long addata;  
  uint64_t L_T_cnt=T_cnt; 
  ad7799_cs();
//  while(ad7799_rdyin())
//	{
//			if(T_cnt-L_T_cnt>3000)//���3Sʱ�䣬�����жϻ���
//			{
////				  uint32_t erro_code=0x01;
////			    set_data_channel(Data_erro_ord,(uint8_t *)&erro_code);//װ�뱨�����
//					return 0;//����һ��0
//			}
//	}
  flag=0x80;
  while(((flag&0x80)==0x80))
  { 
    AD7799_Delay_ms(AD_DELAY1); 
    ad7799_writebyte(0x40);//״̬�Ĵ���
    flag = ad7799_readbyte();//״̬�Ĵ���
  }
  AD7799_Delay_ms(AD_DELAY2);//Ϊʲô��Ҫ��
  ad7799_writebyte(0x58);//���ݼĴ���
  addata = ad7799_readword();//���ض�ȡ����
  AD7799_Delay_ms(AD_DELAY3); //Ϊʲô��Ҫ��
  
  ad7799_ncs();
  
  return  addata;
}
/******************************************************************************
	AD7799 ����
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
		v = 2.5*read_ad7799()/0x1000000;    //�����治Ϊ1�������������
	    return v;
}










