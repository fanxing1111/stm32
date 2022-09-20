/*********************************************************************
** �ļ����ƣ�AD7799.h
** �� �� �ˣ�����ѫ
** �������ڣ�2017.03.31
** ���뻷����IAR For ARM 7.60
** �ļ�˵����AD7799��������ͷ�ļ���
*********************************************************************/
#ifndef _AD7799_H_
#define _AD7799_H_

//#include "main.h"
#include "sys.h"
/*********************************************************************
**							  �궨�� 								**
*********************************************************************/
#define PORT_ADC_CS1		GPIOB
#define PIN_ADC_CS1	    GPIO_Pin_12

#define PORT_ADC_SCK1		GPIOB
#define PIN_ADC_SCK1		GPIO_Pin_13

#define PORT_ADC_MOSI1	GPIOB
#define PIN_ADC_MOSI1		GPIO_Pin_15

#define PORT_ADC_MISO1	GPIOB
#define PIN_ADC_MISO1		GPIO_Pin_14


#define Bit_SET 1
//�����������
#define	ADC_CS1_H		   PORT_ADC_CS1->BSRR = PIN_ADC_CS1
#define	ADC_CS1_L		   PORT_ADC_CS1->BRR = PIN_ADC_CS1

#define	ADC_SCK1_H		 PORT_ADC_SCK1->BSRR = PIN_ADC_SCK1
#define	ADC_SCK1_L		 PORT_ADC_SCK1->BRR = PIN_ADC_SCK1

#define	ADC_MOSI1_H		 PORT_ADC_MOSI1->BSRR = PIN_ADC_MOSI1
#define	ADC_MOSI1_L		 PORT_ADC_MOSI1->BRR = PIN_ADC_MOSI1

#define	ADC_MISO1_IN   HAL_GPIO_ReadPin(PORT_ADC_MISO1,PIN_ADC_MISO1)



/****************************MODE�Ĵ�������ز���******************************/
#define AD7799_MODE_4_17HZ    0x000f    //ת������=4.17hz;  ����:74db(50,60hz)
#define AD7799_MODE_6_25HZ    0x000e    //ת������=6.25hz;  ����:72db(50,60hz)
#define AD7799_MODE_8_33HZ    0x000d    //ת������=8.33hz;  ����:70db(50,60hz)
#define AD7799_MODE_10HZ      0x000c    //ת������=10hz  ;  ����:69db(50,60hz)
#define AD7799_MODE_12_5HZ    0x000b    //ת������=12.5hz;  ����:66db(50,60hz)
#define AD7799_MODE_16_7HZ    0x000a    //ת������=16.7hz;  ����:65db(50,60hz)
#define AD7799_MODE_16_50HZ   0x0009    //ת������=16.7hz;  ����:80db(��50hz)
#define AD7799_MODE_19_6HZ    0x0008    //ת������=19.6hz;  ����:90db(��60hz)
#define AD7799_MODE_50HZ      0x0005    //ת������=50hz;    ����:-
#define AD7799_MODE_470HZ     0x0001    //ת������=470hz;   ����:-

//����ѡ��λ: bit10,9,8
#define AD7799_CONFIG_GAIN_1      0x0000    //����=1(�Ǳ�Ŵ�������)
#define AD7799_CONFIG_GAIN_2      0x0100    //����=2(�Ǳ�Ŵ�������)
#define AD7799_CONFIG_GAIN_4      0x0200    //����=4
#define AD7799_CONFIG_GAIN_8      0x0300    //����=8
#define AD7799_CONFIG_GAIN_16     0x0400    //����=16
#define AD7799_CONFIG_GAIN_32     0x0500    //����=32
#define AD7799_CONFIG_GAIN_64     0x0600    //����=64
#define AD7799_CONFIG_GAIN_128    0x0700    //����=128

#define AD_UB_U         1 //������
#define AD_UB_B         0 //˫����

#define AD_FULL         16777215
#define AD_BFULL        8388608  

#define AD_DELAY1                 5  
#define AD_DELAY2                 5   
#define AD_DELAY3                 1 
/*********************************************************************
**							 ��������								**
*********************************************************************/

/*********************************************************************
**							 ��������								**
*********************************************************************/
void AD7799_Delay_ms(uint16_t time);
void AD7799_Delay_us(uint16_t time);
void AD7799_IO_Init(void);

void ad7799_cs(void);
void ad7799_ncs(void);
uint8_t ad7799_rdyin(void);

void ad7799_rst(void);
void ad7799_writebyte(uint8_t word);
uint8_t ad7799_readbyte(void);
uint32_t ad7799_readword(void);

void config_admode(uint8_t ub,uint8_t mult_num);
void ad7799_init(uint8_t ch,uint8_t ub,uint8_t mult);
void ad7799ch_init(uint8_t ch,uint8_t ub,uint8_t mult);
void ad7799_choose_ch(uint8_t ch,uint8_t ub,uint8_t mult);
uint32_t read_ad7799(void);
void ad7799_config(void);
uint32_t AD7799_getID(void);
float AD7799_getV(uint8_t channel);
#endif
/*********************************************************************
**							 �ļ�����								**
*********************************************************************/

