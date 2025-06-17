#ifndef __MASTERDRIVER_H__
#define __MASTERDRIVER_H__
#include "includes.h"

typedef struct
{
    char RxBuf[32];
    uint8_t RxStart;
    uint8_t RxEnd;
    char TxBuf[256];
    uint8_t TxCnt;
    uint8_t TxLen;
    uint8_t Tx_busy_flg;
} MasterUartTYPE;

typedef struct 
{
    uint8_t Hardflg;
    uint8_t Buf[20];
    uint8_t cnt;
    uint32_t linkcnt;
}MasterBufTYPE;

typedef struct
{
    uint8_t CmdBuf[4];
    uint8_t Cmdstart;
    uint8_t Cmdend;
    uint8_t Cmd;
}MasterCmdTYPE;

extern MasterUartTYPE *MasterUart;
extern MasterCmdTYPE *MasterCmdpoint;

void Init_MasterUartSet(void);
uint8_t USART_SendDataString(uint8_t datacmd, uint8_t *pData, uint8_t buflen);
void RevMaster(MasterBufTYPE *masterbuf);
void MasterUart_pro(MasterCmdTYPE *cmdpoint);
void SetMastercmd(MasterCmdTYPE *cmdpoint, uint8_t cmd);

#endif
