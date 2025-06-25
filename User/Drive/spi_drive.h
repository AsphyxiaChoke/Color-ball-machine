#ifndef __SPI_DRIVE_H__
#define __SPI_DRIVE_H__
#include "spi.h"

#define WriteEEPROM(x,y,m)  Write_spi(x,y,m)
#define ReadEEPROM(x,y,m)   Read_spi(x,y,m)
#define __24C04_Addr                (0)
#define eeDipSw     0x10

#define eeIICflg                (__24C04_Addr+0x0fc)    //IIC标志4字节
#define eeAccoutPassWord        (__24C04_Addr+0x100)    //查账密码4字节
#define eeConfigPassWord        (__24C04_Addr+0x104)    //设置密码4字节
#define eeSystemPassWord        (__24C04_Addr+0x108)    //系统密码4字节
#define eeMachine_line          (__24C04_Addr+0x10c)    //线号4字节
#define eeMachine_id            (__24C04_Addr+0x110)    //机号16字节
#define eeFutoryTime            (__24C04_Addr+0x120)    //系统时间，10byte
#define eeProgramVel            (__24C04_Addr+0x130)    //10byte
#define eeQiuwei                (__24C04_Addr+0x140)    //机芯球位10byte

#define eeCurrPayoff            (__24C04_Addr+0x200)    //公司当前账目

#define eeCHILD                 (__24C04_Addr+0x300)    //128字节
#define eeCOIN_IN               (__24C04_Addr+0x300)    //投币
#define eeCOIN_OUT              (__24C04_Addr+0x340)
#define eeKEY_IN                (__24C04_Addr+0x380) 
#define eeKEY_OUT               (__24C04_Addr+0x3c0) 
#define eeCredit                (__24C04_Addr+0x400) 
#define eeAllBet8               (__24C04_Addr+0x440)
#define eeAllWin8               (__24C04_Addr+0x4c0)  

#define eeGameRecord           (__24C04_Addr+0x550)    // GameRecordType 结构体地址
#define eeLuckJiang            (__24C04_Addr+0x700)    // LuckJiangType 结构体地址

#define IICINIT                 0x12345678

#define W_INT4 4 // 字节宽度
#define W_INT8 8

HAL_StatusTypeDef Write_spi(uint16_t addr, uint8_t *pdata, uint32_t size);
HAL_StatusTypeDef Read_spi(uint16_t addr,uint8_t * pdata,uint32_t size);
HAL_StatusTypeDef Read_spi_station(uint8_t * pdata,uint32_t size);

#endif
