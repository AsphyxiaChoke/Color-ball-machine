#include "MasterDriver.h"
#include "stm32h7xx_hal_conf.h"
#include "stm32h7xx_hal_uart.h"

#define UART_STAR_CHAR 0x1a
#define UART_MID_CHAR 0x1b
#define UART_STOP_CHAR 0x1c
#define UART_SUM 0x5A

#define MASTER_UART huart1
//----------------------------------------------------------
// 命令控制全局指针
//----------------------------------------------------------
MasterUartTYPE *MasterUart;
MasterCmdTYPE *MasterCmdpoint;

void Master_Rx(UART_HandleTypeDef *huart)
{
    if (huart == &MASTER_UART)
    {
        HAL_UART_Receive_IT(&MASTER_UART, (uint8_t *)&MasterUart->RxBuf[MasterUart->RxStart++], 1);
        MasterUart->RxStart &= 0x1f;
    }
}

void Master_Tx(UART_HandleTypeDef *huart)
{
    if (huart == &MASTER_UART)
    {
        if (MasterUart->TxCnt >= (sizeof(MasterUart->TxBuf) - 1))
        {
            MasterUart->TxCnt = 0;
            MasterUart->TxLen = 0;
            MasterUart->Tx_busy_flg = 0;
        }
        else if (MasterUart->TxCnt < MasterUart->TxLen)
        {
            HAL_UART_Transmit_IT(&MASTER_UART, (uint8_t *)&MasterUart->TxBuf[MasterUart->TxCnt++], 1);
        }
        else
        {
            MasterUart->TxCnt = 0;
            MasterUart->TxLen = 0;
            MasterUart->Tx_busy_flg = 0;
        }
    }
}
//---------------------------------------------------------------------
// 初始化主机通信接口
//---------------------------------------------------------------------
void Init_MasterUartSet(void)
{
    MasterUart->RxStart = 0;
    MasterUart->RxEnd = 0;
    HAL_UART_RegisterCallback(&MASTER_UART, HAL_UART_RX_COMPLETE_CB_ID, Master_Rx);
    HAL_UART_RegisterCallback(&MASTER_UART, HAL_UART_TX_COMPLETE_CB_ID, Master_Tx);
    HAL_UART_Receive_IT(&MASTER_UART, (uint8_t *)&MasterUart->RxBuf[MasterUart->RxStart++], 1); // 启动接收中断
}
//------------------------------------------------
// 读接收缓存数据量 无返回 0
//------------------------------------------------
uint8_t ReadRxNum(void)
{
    uint8_t start, end;
    start = MasterUart->RxStart;
    end = MasterUart->RxEnd;
    if (start != end)
    {
        return (start - end) & 0x1f;
    }
    return 0;
}
//------------------------------------------------
// 读接收缓存数据
//------------------------------------------------
uint8_t ReadRxBuf(void)
{
    uint8_t ch;
    ch = MasterUart->RxBuf[MasterUart->RxEnd];
    MasterUart->RxEnd++;
    MasterUart->RxEnd &= 0x1f;
    return ch;
}
//------------------------------------------------
// 发送数据
//------------------------------------------------
uint8_t USART_SendDataString(uint8_t datacmd, uint8_t *pData, uint8_t buflen)
{
    OS_ERR err;
    uint8_t ch;
    uint16_t i, sendlen;
    while (MasterUart->Tx_busy_flg)
    {
        OSTimeDly(10, OS_OPT_TIME_DLY, &err);
    }

    MasterUart->TxBuf[0] = UART_STAR_CHAR;
    MasterUart->TxBuf[1] = datacmd;
    MasterUart->TxBuf[2] = buflen;
    sendlen = 3;
    for (i = 0; i < buflen; i++, pData++)
    {
        ch = *pData;
        switch (ch)
        {
        case UART_STAR_CHAR:
            MasterUart->TxBuf[sendlen++] = 1;
            MasterUart->TxBuf[sendlen++] = UART_MID_CHAR;
            break;
        case UART_MID_CHAR:
            MasterUart->TxBuf[sendlen++] = 2;
            MasterUart->TxBuf[sendlen++] = UART_MID_CHAR;
            break;
        case UART_STOP_CHAR:
            MasterUart->TxBuf[sendlen++] = 3;
            MasterUart->TxBuf[sendlen++] = UART_MID_CHAR;
            break;
        default:
            MasterUart->TxBuf[sendlen++] = ch;
            break;
        }
    }
    MasterUart->TxBuf[sendlen++] = UART_STOP_CHAR;
    MasterUart->TxLen = sendlen;
    MasterUart->TxCnt = 0;
    MasterUart->Tx_busy_flg = 1;
    HAL_UART_Transmit_IT(&MASTER_UART, (uint8_t *)&MasterUart->TxBuf[MasterUart->TxCnt++], 1);
    return 0;
}
//-------------------------------------------------
// 接收数据
//-------------------------------------------------
void RevMaster(MasterBufTYPE *masterbuf)
{
    uint8_t i, ch;
    i = ReadRxNum();
    if (i)
    {
        uint8_t j;
        for (j = 0; j < i; j++)
        {
            ch = ReadRxBuf();
            switch (ch)
            {
            case 0x1a:
                masterbuf->Hardflg = 1;
                masterbuf->cnt = 0;
                break;
            case 0x1b:
                if (masterbuf->Hardflg && (masterbuf->cnt > 0))
                {
                    switch (masterbuf->Buf[masterbuf->cnt - 1])
                    {
                    case 0x01:
                        masterbuf->Buf[masterbuf->cnt - 1] = 0x1a;
                        break;
                    case 0x02:
                        masterbuf->Buf[masterbuf->cnt - 1] = 0x1b;
                        break;
                    case 0x03:
                        masterbuf->Buf[masterbuf->cnt - 1] = 0x1c;
                        break;
                    default:
                        masterbuf->Buf[masterbuf->cnt] = ch;
                        masterbuf->cnt++;
                        if (masterbuf->cnt >= sizeof(masterbuf->Buf))
                        {
                            masterbuf->cnt = 0;
                        }
                        break;
                    }
                }
                else
                {
                    masterbuf->cnt = 0;
                    masterbuf->Hardflg = 0;
                }
                break;
            case 0x1c:
            {
                if (masterbuf->Hardflg && (masterbuf->Buf[0] == 0x03) && (masterbuf->cnt > 2))
                {
// 数据处理添加
#warning "主机接收数据处理！"
                }
                masterbuf->cnt = 0;
                masterbuf->Hardflg = 0;
            }
            break;
            default:
                masterbuf->Buf[masterbuf->cnt] = ch;
                masterbuf->cnt++;
                if (masterbuf->cnt >= sizeof(masterbuf->Buf))
                {
                    masterbuf->Hardflg = 0;
                    masterbuf->cnt = 0;
                }
                break;
            }
        }
    }
}
//-------------------------------------------------
// 发送命令数据
//-------------------------------------------------
void MasterUart_pro(MasterCmdTYPE *cmdpoint)
{
    if (cmdpoint->Cmdstart != cmdpoint->Cmdend)
    {
        cmdpoint->Cmd = cmdpoint->CmdBuf[cmdpoint->Cmdend++];
        cmdpoint->Cmdend &= 0x03;
    }
    switch (cmdpoint->Cmd)
    {
    case PC_RESTART:
        MasterRuncmd(cmdpoint->Cmd);
        break;
    case PC_GAMESTART:
    case PC_STARTBET:
    case PC_GAMETIME:
    case PC_STOPBET:
        MasterRuncmd(cmdpoint->Cmd);
        break;
    case PC_RESULT:
    case PC_STARTBONUS:
    case PC_STOPBONUS:
    case PC_STARTWIN:
    case PC_STOPWIN:
    case PC_GAMEEND:
        MasterRunsult(cmdpoint->Cmd);
        break;
    case PC_HISTORY:
        MasterRunHistory();
        break;
    case PC_LUCKHISTORY:
        MasterRunLuckHistory();
        break;
    }
}
//-------------------------------------------------
// 设置要发送命令
//-------------------------------------------------
void SetMastercmd(MasterCmdTYPE *cmdpoint, uint8_t cmd)
{
    uint32_t time;
    OS_ERR err;
    cmdpoint->CmdBuf[cmdpoint->Cmdstart++] = cmd;
    cmdpoint->Cmdstart &= 0x03;
    for (time = OSTimeGet(&err) + 4; time > OSTimeGet(&err);)
    {
        BackProcess();
        CoinKeyProc();
    }
}
