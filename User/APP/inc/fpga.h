#ifndef __FPGA_H__
#define __FPGA_H__
#include "stdint.h"
#include <stdio.h>
#include <string.h>
#define FPGAADDR 0x60000000
#define REGADDR (FPGAADDR + 0X400)
#define CTRUARTADDR (FPGAADDR + 0X00)
#define FLASHADDR (FPGAADDR + 0X500)
#pragma pack(1)
typedef struct
{
    uint8_t Led;
    uint8_t Send_crt;
    uint8_t check_crt;
    uint8_t RESERVD1;
    uint32_t HexToDec;
    uint16_t RESERVD[4];
    uint16_t crc_reg;
} WFPGA_TypeDef;

typedef struct
{
    uint32_t RESERVED1;
    uint32_t HexToDec;
    uint16_t RESERVED0[4];
    uint16_t crc_reg;
    uint16_t RES[9];
    uint16_t crc_addr_cnt;
    uint8_t RESERVED2;
    uint8_t uart_en;
    uint16_t FpgaCtrl0;
    uint16_t FpgaCtrl1;
    uint16_t FpgaCtrl2;
    uint16_t FpgaCtrl3;
} RFPGA_TypeDef;

typedef struct
{
    uint32_t Machine_id;
    uint8_t Ver[4];
    uint16_t Game;
    uint16_t Index;
    uint16_t RESERVED1;
    uint16_t RESERVED2;
    uint8_t Ser[16];
} ROM_TYPEDEF;

#define Fpga_wreg ((volatile WFPGA_TypeDef *)(REGADDR))
#define Fpga_rreg ((volatile RFPGA_TypeDef *)(REGADDR))
#define Fpga_flash ((volatile ROM_TYPEDEF *)(FLASHADDR))

typedef struct
{
    uint8_t UartBuf[128];
} UART_TYPEDEF;

#define Fpga_uart ((volatile UART_TYPEDEF *)(CTRUARTADDR))

#pragma pack()

#endif
