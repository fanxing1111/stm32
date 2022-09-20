/*********************************************************************
** 文件名称：AD7799.h
** 创 建 人：姜继勋
** 创建日期：2017.03.31
** 编译环境：IAR For ARM 7.60
** 文件说明：AD7799驱动程序头文件。
*********************************************************************/
#ifndef _AD7799_H_
#define _AD7799_H_

//#include "main.h"
#include "sys.h"
/*********************************************************************
**							  宏定义 								**
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
//输出操作定义
#define	ADC_CS1_H		   PORT_ADC_CS1->BSRR = PIN_ADC_CS1
#define	ADC_CS1_L		   PORT_ADC_CS1->BRR = PIN_ADC_CS1

#define	ADC_SCK1_H		 PORT_ADC_SCK1->BSRR = PIN_ADC_SCK1
#define	ADC_SCK1_L		 PORT_ADC_SCK1->BRR = PIN_ADC_SCK1

#define	ADC_MOSI1_H		 PORT_ADC_MOSI1->BSRR = PIN_ADC_MOSI1
#define	ADC_MOSI1_L		 PORT_ADC_MOSI1->BRR = PIN_ADC_MOSI1

#define	ADC_MISO1_IN   HAL_GPIO_ReadPin(PORT_ADC_MISO1,PIN_ADC_MISO1)



/****************************MODE寄存器的相关参数******************************/
#define AD7799_MODE_4_17HZ    0x000f    //转换速率=4.17hz;  抑制:74db(50,60hz)
#define AD7799_MODE_6_25HZ    0x000e    //转换速率=6.25hz;  抑制:72db(50,60hz)
#define AD7799_MODE_8_33HZ    0x000d    //转换速率=8.33hz;  抑制:70db(50,60hz)
#define AD7799_MODE_10HZ      0x000c    //转换速率=10hz  ;  抑制:69db(50,60hz)
#define AD7799_MODE_12_5HZ    0x000b    //转换速率=12.5hz;  抑制:66db(50,60hz)
#define AD7799_MODE_16_7HZ    0x000a    //转换速率=16.7hz;  抑制:65db(50,60hz)
#define AD7799_MODE_16_50HZ   0x0009    //转换速率=16.7hz;  抑制:80db(仅50hz)
#define AD7799_MODE_19_6HZ    0x0008    //转换速率=19.6hz;  抑制:90db(仅60hz)
#define AD7799_MODE_50HZ      0x0005    //转换速率=50hz;    抑制:-
#define AD7799_MODE_470HZ     0x0001    //转换速率=470hz;   抑制:-

//增益选择位: bit10,9,8
#define AD7799_CONFIG_GAIN_1      0x0000    //增益=1(仪表放大器不用)
#define AD7799_CONFIG_GAIN_2      0x0100    //增益=2(仪表放大器不用)
#define AD7799_CONFIG_GAIN_4      0x0200    //增益=4
#define AD7799_CONFIG_GAIN_8      0x0300    //增益=8
#define AD7799_CONFIG_GAIN_16     0x0400    //增益=16
#define AD7799_CONFIG_GAIN_32     0x0500    //增益=32
#define AD7799_CONFIG_GAIN_64     0x0600    //增益=64
#define AD7799_CONFIG_GAIN_128    0x0700    //增益=128

#define AD_UB_U         1 //单极性
#define AD_UB_B         0 //双极性

#define AD_FULL         16777215
#define AD_BFULL        8388608  

#define AD_DELAY1                 5  
#define AD_DELAY2                 5   
#define AD_DELAY3                 1 
/*********************************************************************
**							 变量申明								**
*********************************************************************/

/*********************************************************************
**							 函数申明								**
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
**							 文件结束								**
*********************************************************************/

