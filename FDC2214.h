#ifndef __FDC2214_H
#define __FDC2214_H
#include "sys.h"

#define READ_ADDR                  0x55						//读写操作（当ADDER=1时，地址为OX2B,当ADDER=0时，地址为OX2A） R=1, W=0
#define WRITE_ADDR                 0x54

#define DATA0_MSB                     0x00				//数据存储寄存器
#define DATA1_MSB                     0x02
#define DATA2_MSB                     0x04
#define DATA3_MSB                     0x06

#define RCOUNT0                    0x08          //转换时间
#define RCOUNT1                    0x09
#define RCOUNT2                    0x0A
#define RCOUNT3                    0x0B
#define OFFSET0                    0X0C						//转换偏移
#define OFFSET1                    0X0D
#define OFFSET2                    0X0E
#define OFFSET3                    0X0F
#define SETTLECOUNT0               0x10   				//等待时间
#define SETTLECOUNT1               0x11
#define SETTLECOUNT2               0x12
#define SETTLECOUNT3               0x13
#define CLK_DIVIDERS0              0x14   				//时钟分频
#define CLK_DIVIDERS1              0x15
#define CLK_DIVIDERS2              0x16
#define CLK_DIVIDERS3              0x17

#define STATUS                     0x18
#define ERROR_CONFIG               0x19
#define CONFIG                     0x1A							//寄存器配置
#define MUX_CONFIG                 0x1B						//多通道采集序列
#define RESET_DEV                  0x1C						//功能复位

#define DRIVE_CURRENT0             0x1E							//驱动电流
#define DRIVE_CURRENT1             0x1F
#define DRIVE_CURRENT2             0x20
#define DRIVE_CURRENT3             0x21

#define MANUFACTURER_ID            0x7E             //出厂商ID
#define DEVICE_ID 								 0x7F 						//设备ID

void FDC2214_Init_Multi(void);
u16 FDC2214_Read2Byte(u8 reg_addr);
u8 FDC2214_Write2Byte(u8 reg_addr,u16 reg_data);
u16 FDC2214_read_CHx(u8 CHx);

#endif

