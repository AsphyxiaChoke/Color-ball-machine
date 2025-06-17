#include "GameSet.h"
#include "stm32h7xx_hal_conf.h"
#include "stm32h7xx_hal_uart.h"
#define GAME_SET huart2
struct GameSetRx *Gamesetrx;

const char GameName[] = "星星科技";

void HAL_UART_RxGameSet(UART_HandleTypeDef *huart)
{
    if (huart == &GAME_SET)
    {
        uint8_t Res;
        Res = Gamesetrx->Res;
        if (Res == 0xaa)
        {
            Gamesetrx->RxCnt = 0;
            Gamesetrx->RxendCnt = 0;
        }
        else
        {
            if (Res == 0xff)
            {
                Gamesetrx->RxendCnt++;
                if (Gamesetrx->RxendCnt == 3)
                {
                    Gamesetrx->Rx_ok_flg = 1;
                    Gamesetrx->RxCnt = 0;
                    Gamesetrx->RxendCnt = 0;
                }
            }
            else
            {
                Gamesetrx->RxBuf[Gamesetrx->RxCnt++] = Res;
                if (Gamesetrx->RxCnt >= (sizeof(Gamesetrx->RxBuf) - 1))
                {
                    Gamesetrx->RxCnt = 0;
                    Gamesetrx->RxendCnt = 0;
                }
                Gamesetrx->RxBuf[Gamesetrx->RxCnt] = 0;
            }
        }
        HAL_UART_Receive_IT(&GAME_SET, (uint8_t *)&Gamesetrx->Res, 1); // 继续开启下一次接收中断
    }
}

void HAL_UART_TxGameSet(UART_HandleTypeDef *huart)
{
    if (huart == &GAME_SET)
    {
        if (Gamesetrx->TxCnt >= sizeof(Gamesetrx->TxBuf))
        {
            Gamesetrx->TxCnt = 0;
            Gamesetrx->TxLen = 0;
            Gamesetrx->Tx_busy_flg = 0;
        }
        else if (Gamesetrx->TxLen > Gamesetrx->TxCnt)
        {
            HAL_UART_Transmit_IT(&GAME_SET, (uint8_t *)&Gamesetrx->TxBuf[Gamesetrx->TxCnt++], 1);
        }
        else
        {
            Gamesetrx->TxCnt = 0;
            Gamesetrx->TxLen = 0;
            Gamesetrx->Tx_busy_flg = 0;
        }
    }
}

void Init_UartSet(void)
{
    HAL_UART_RegisterCallback(&GAME_SET, HAL_UART_RX_COMPLETE_CB_ID, HAL_UART_RxGameSet); // 注册回调函数
    HAL_UART_RegisterCallback(&GAME_SET, HAL_UART_TX_COMPLETE_CB_ID, HAL_UART_TxGameSet);
    HAL_UART_Receive_IT(&GAME_SET, (uint8_t *)&Gamesetrx->Res, 1); // 启动接收中断
}
//----------------------------------------------------------------
// 发送数据
//----------------------------------------------------------------
void HTMSend(char *buf)
{
    OS_ERR err;
    while (Gamesetrx->Tx_busy_flg)
    {
        OSTimeDly(1, OS_OPT_TIME_DLY, &err);
    }
    strcpy(Gamesetrx->TxBuf, buf);
    Gamesetrx->TxLen = strlen(Gamesetrx->TxBuf);
    Gamesetrx->TxCnt = 0;
    Gamesetrx->Tx_busy_flg = 1;
    HAL_UART_Transmit_IT(&GAME_SET, (uint8_t *)&Gamesetrx->TxBuf[Gamesetrx->TxCnt++], 1);
    OSTimeDly(1, OS_OPT_TIME_DLY, &err);
}
//----------------------------------------------------------------
// string to dex
//----------------------------------------------------------------
uint32_t strtodex(char buf[18])
{
    uint8_t i;
    uint32_t d = 0;
    uint8_t len = strlen(buf);
    for (i = 0; i < len; i++)
    {
        if (buf[i] == 0)
            break;
        if ((buf[i] < 0x30) || (buf[i] > 0x39))
            return 0;
        d = d * 10 + (buf[i] - 0x30);
    }
    return d;
}
//----------------------------------------------------------------
// string to hex
//----------------------------------------------------------------
void strtohex(char *point, char buf[18], uint8_t size)
{
    uint8_t i, d;
    uint64_t tmp = 0;
    uint8_t len = strlen(buf);
    for (i = 0; i < len; i++)
    {
        if (buf[i] == 0)
            break;
        if (buf[i] >= 0x30 && buf[i] <= 0x39)
        {
            d = buf[i] - 0x30;
        }
        else if (buf[i] > 0x40 && buf[i] < 0x47)
        {
            d = buf[i] - 0x37;
        }
        tmp |= (uint64_t)d << (i * 4);
    }
    memcpy(point, &tmp, size);
}
//----------------------------------------------------------------
// 游戏初始化触屏
//----------------------------------------------------------------
void GameSetInit(HANDTYPE *handpoint)
{
    char buf[50];
    handpoint->gamestation = 0;
    handpoint->GameIndex = MAIN_MUNE;
    // sprintf(buf, "\xff\xff\xff");
    // HTMSend(buf);
    sprintf(buf, "page 0\xff\xff\xff");
    HTMSend(buf);
    sprintf(buf, "t0.txt=\"%s\"\xff\xff\xff", GameName);
    HTMSend(buf);
}
//----------------------------------------------------------------
// 游戏菜单
//----------------------------------------------------------------
void GameSetMune(HANDTYPE *handpoint)
{
    char buf[50];
    if (handpoint->rx_ok_flg)
    {
        handpoint->rx_ok_flg = 0;
        if (handpoint->page == 0)
        {
            handpoint->dim = OS_TICKS_PER_SEC * 3;
            sprintf(buf, "dim=100\xff\xff\xff");
            HTMSend(buf);
            switch (handpoint->ctr)
            {
            case 1: // 进输入查账密码
                handpoint->Oldpage = handpoint->page;
                handpoint->Oldctr = handpoint->ctr;
                handpoint->GameIndex = INPUT_PASS;
                handpoint->password = 0;
                sprintf(buf, "page 6\xff\xff\xff");
                HTMSend(buf);
                sprintf(buf, "t0.txt=\"输入查账密码\"\xff\xff\xff");
                HTMSend(buf);
                sprintf(buf, "t1.txt=\"\"\xff\xff\xff");
                HTMSend(buf);
                sprintf(buf, "t2.txt=\"\"\xff\xff\xff");
                HTMSend(buf);
                break;
            case 2: // 进入报账
                handpoint->Oldpage = handpoint->page;
                handpoint->Oldctr = handpoint->ctr;
                handpoint->GameIndex = AUDIT_INPUT;
                sprintf(buf, "page 2\xff\xff\xff");
                HTMSend(buf);
                sprintf(buf, "t2.txt=\"%ld\"\xff\xff\xff", Machine_line);
                HTMSend(buf);
                sprintf(buf, "t3.txt=\"%ld\"\xff\xff\xff", Machine_id);
                HTMSend(buf);
                sprintf(buf, "t4.txt=\"%ld\"\xff\xff\xff", handpoint->xuleihao);
                HTMSend(buf);
                sprintf(buf, "t5.txt=\"%lld\"\xff\xff\xff", Prevpayoff);
                HTMSend(buf);
                sprintf(buf, "t6.txt=\"%lld\"\xff\xff\xff", CurrPayoff);
                HTMSend(buf);
                sprintf(buf, "t7.txt=\"\"\xff\xff\xff");
                HTMSend(buf);
                break;
            case 3: // 进输入参数密码
                handpoint->Oldpage = handpoint->page;
                handpoint->Oldctr = handpoint->ctr;
                handpoint->GameIndex = INPUT_PASS;
                handpoint->password = 0;
                sprintf(buf, "page 6\xff\xff\xff");
                HTMSend(buf);
                sprintf(buf, "t0.txt=\"输入参数密码\"\xff\xff\xff");
                HTMSend(buf);
                sprintf(buf, "t1.txt=\"\"\xff\xff\xff");
                HTMSend(buf);
                sprintf(buf, "t2.txt=\"\"\xff\xff\xff");
                HTMSend(buf);
                break;
            case 4: // 进输入系统密码
                handpoint->Oldpage = handpoint->page;
                handpoint->Oldctr = handpoint->ctr;
                handpoint->GameIndex = INPUT_PASS;
                handpoint->password = 0;
                sprintf(buf, "page 6\xff\xff\xff");
                HTMSend(buf);
                sprintf(buf, "t0.txt=\"输入系统密码\"\xff\xff\xff");
                HTMSend(buf);
                sprintf(buf, "t1.txt=\"\"\xff\xff\xff");
                HTMSend(buf);
                sprintf(buf, "t2.txt=\"\"\xff\xff\xff");
                HTMSend(buf);
                break;
            case 5: // 进入球位设置
                SwtQiuFlag = 1;
                handpoint->Oldpage = handpoint->page;
                handpoint->Oldctr = handpoint->ctr;
                handpoint->GameIndex = SET_QIU;
                handpoint->SetVolume = 0;
                sprintf(buf, "page 5\xff\xff\xff");
                HTMSend(buf);
                // sprintf(buf, "tsw 255,0\xff\xff\xff");
                // HTMSend(buf);
                break;
            }
        }
    }
    if (handpoint->dim)
    {
        if (handpoint->dim == 1)
        {
            sprintf(buf, "dim=20\xff\xff\xff");
            HTMSend(buf);
        }
        handpoint->dim--;
    }
    if (handpoint->delay)
    {
        handpoint->delay--;
    }
    if ((handpoint->gamestation != Gamestation) || (handpoint->delay == 0))
    {
        handpoint->delay = OS_TICKS_PER_SEC / 2;
        handpoint->gamestation = Gamestation;
        if (handpoint->gamestation & 0xff)
        {
            switch (handpoint->gamestation & 0xff)
            {
            case MACHERR01:
                sprintf(buf, "t2.txt=\"机芯顶杆故障!\"\xff\xff\xff");
                break;
            case MACHERR02:
                sprintf(buf, "t2.txt=\"机芯帽子故障!\"\xff\xff\xff");
                break;
            case MACHERR03:
                sprintf(buf, "t2.txt=\"机芯翻球故障!\"\xff\xff\xff");
                break;
            case MACHERR04:
                sprintf(buf, "t2.txt=\"机芯落球故障!\"\xff\xff\xff");
                break;
            case MACHERR05:
                sprintf(buf, "t2.txt=\"中心盘故障!\"\xff\xff\xff");
                break;
            case MACHERR06:
                sprintf(buf, "t2.txt=\"机芯未联接!\"\xff\xff\xff");
                break;
            case MACHERR07:
                sprintf(buf, "t2.txt=\"球位故障!\"\xff\xff\xff");
                break;
            case MACHERR08:
                sprintf(buf, "t2.txt=\"门控故障!\"\xff\xff\xff");
                break;
            }
            HTMSend(buf);
        }
        else
        {
            sprintf(buf, "t2.txt=\"%d-%d-%02x\"\xff\xff\xff", ProcMem + 1, GameTime, GamePai);
            HTMSend(buf);
        }
    }
}
//----------------------------------------------------------------
// 输入密码
//----------------------------------------------------------------
void GameSetInputPass(HANDTYPE *handpoint)
{
    char buf[50];
    if (handpoint->rx_ok_flg)
    {
        handpoint->rx_ok_flg = 0;
        if (handpoint->page == 6)
        {

            if (handpoint->ctr == 1)
            {
                switch (handpoint->Oldctr)
                {
                case 1:
                    if (handpoint->password == AccoutPassWord)
                    {
                        uint8_t i;
                        ChildType *pChild;
                        int32_t tmp;
                        int64_t totalaudit[7];
                        memset(totalaudit, 0, sizeof(totalaudit));
                        handpoint->Oldpage = handpoint->page;
                        handpoint->Oldctr = handpoint->ctr;
                        handpoint->GameIndex = AUDIT_LOOK;
                        sprintf(buf, "page 1\xff\xff\xff");
                        HTMSend(buf);
                        for (i = 0; i < MAX_FENJI; i++)
                        {
                            pChild = &Child[i];
                            sprintf(buf, "t%d.txt=\"%ld\"\xff\xff\xff", 9 + 7 * i, pChild->CoinIn);
                            HTMSend(buf);
                            totalaudit[0] += pChild->CoinIn;
                            sprintf(buf, "t%d.txt=\"%ld\"\xff\xff\xff", 10 + 7 * i, pChild->CoinOut);
                            HTMSend(buf);
                            totalaudit[1] += pChild->CoinOut;
                            sprintf(buf, "t%d.txt=\"%ld\"\xff\xff\xff", 11 + 7 * i, pChild->KeyIn);
                            HTMSend(buf);
                            totalaudit[2] += pChild->KeyIn;
                            sprintf(buf, "t%d.txt=\"%ld\"\xff\xff\xff", 12 + 7 * i, pChild->KeyOut);
                            HTMSend(buf);
                            totalaudit[3] += pChild->KeyOut;
                            sprintf(buf, "t%d.txt=\"%ld\"\xff\xff\xff", 13 + 7 * i, pChild->PlayCnt);
                            HTMSend(buf);
                            totalaudit[4] += pChild->PlayCnt;
                            sprintf(buf, "t%d.txt=\"%ld\"\xff\xff\xff", 14 + 7 * i, pChild->WinCnt);
                            HTMSend(buf);
                            totalaudit[5] += pChild->WinCnt;
                            tmp = pChild->CoinIn + pChild->KeyIn - pChild->CoinOut - pChild->KeyOut;
                            sprintf(buf, "t%d.txt=\"%ld\"\xff\xff\xff", 15 + 7 * i, tmp);
                            HTMSend(buf);
                            totalaudit[6] += tmp;
                        }
                        sprintf(buf, "t%d.txt=\"%lld\"\xff\xff\xff", 9 + 7 * i, totalaudit[0]);
                        HTMSend(buf);
                        sprintf(buf, "t%d.txt=\"%lld\"\xff\xff\xff", 10 + 7 * i, totalaudit[1]);
                        HTMSend(buf);
                        sprintf(buf, "t%d.txt=\"%lld\"\xff\xff\xff", 11 + 7 * i, totalaudit[2]);
                        HTMSend(buf);
                        sprintf(buf, "t%d.txt=\"%lld\"\xff\xff\xff", 12 + 7 * i, totalaudit[3]);
                        HTMSend(buf);
                        sprintf(buf, "t%d.txt=\"%lld\"\xff\xff\xff", 13 + 7 * i, totalaudit[4]);
                        HTMSend(buf);
                        sprintf(buf, "t%d.txt=\"%lld\"\xff\xff\xff", 14 + 7 * i, totalaudit[5]);
                        HTMSend(buf);
                        sprintf(buf, "t%d.txt=\"%lld\"\xff\xff\xff", 15 + 7 * i, totalaudit[6]);
                        HTMSend(buf);
                    }
                    else
                    {
                        sprintf(buf, "t2.txt=\"密码错误！\"\xff\xff\xff");
                        HTMSend(buf);
                    }
                    break;
                case 3:
                    if (handpoint->password == ConfigPassWord)
                    {
                        handpoint->Oldpage = handpoint->page;
                        handpoint->Oldctr = handpoint->ctr;
                        handpoint->GameIndex = CONFIG_CRT1;
                        memcpy(&SetDipSw, &DipSw, sizeof(DipSw));
                        sprintf(buf, "page 3\xff\xff\xff");
                        HTMSend(buf);
                        sprintf(buf, "t3.txt=\"%ld\"\xff\xff\xff", GetCoinIn(&SetDipSw));
                        HTMSend(buf);
                        sprintf(buf, "t4.txt=\"%ld\"\xff\xff\xff", GetCoinOut(&SetDipSw));
                        HTMSend(buf);
                        sprintf(buf, "t5.txt=\"%ld\"\xff\xff\xff", GetNoteIn(&SetDipSw));
                        HTMSend(buf);
                        sprintf(buf, "t6.txt=\"%ld\"\xff\xff\xff", GetNoteOUt(&SetDipSw));
                        HTMSend(buf);
                        sprintf(buf, "t7.txt=\"%ld\"\xff\xff\xff", GetDec(&SetDipSw));
                        HTMSend(buf);
                        sprintf(buf, "t8.txt=\"%ld\"\xff\xff\xff", GetMinbet(&SetDipSw));
                        HTMSend(buf);
                        sprintf(buf, "t9.txt=\"%ld\"\xff\xff\xff", GetMaxbet(&SetDipSw));
                        HTMSend(buf);
                        sprintf(buf, "t10.txt=\"%ld\"\xff\xff\xff", GetGameTime(&SetDipSw));
                        HTMSend(buf);
                        sprintf(buf, "t11.txt=\"%ld\"\xff\xff\xff", GetRate(&SetDipSw));
                        HTMSend(buf);
                        sprintf(buf, "t12.txt=\"%ld\"\xff\xff\xff", GetChildOver(&SetDipSw));
                        HTMSend(buf);
                        sprintf(buf, "t13.txt=\"%ld\"\xff\xff\xff", GetGameOver(&SetDipSw));
                        HTMSend(buf);
                        sprintf(buf, "t14.txt=\"%ld\"\xff\xff\xff", GetLuckIndex(&SetDipSw) + 1);
                        HTMSend(buf);
                        sprintf(buf, "t15.txt=\"%ld\"\xff\xff\xff", GetMinBonus(&SetDipSw));
                        HTMSend(buf);
                        sprintf(buf, "t16.txt=\"%ld\"\xff\xff\xff", GetMaxBonus(&SetDipSw));
                        HTMSend(buf);
                        sprintf(buf, "t17.txt=\"%s\"\xff\xff\xff", (SetDipSw.PlayFunEn & PLAYING_FLG) ? "开" : "关");
                        HTMSend(buf);
                        sprintf(buf, "t18.txt=\"%ld\"\xff\xff\xff", GetJifenRate(&SetDipSw));
                        HTMSend(buf);
                        sprintf(buf, "t19.txt=\"%s\"\xff\xff\xff", (SetDipSw.PlayFunEn & LUCKBIT_FLG) ? "开" : "关");
                        HTMSend(buf);
                        sprintf(buf, "t20.txt=\"%s\"\xff\xff\xff", (SetDipSw.PlayFunEn & MINGPAI_FLG) ? "开" : "关");
                        HTMSend(buf);
                        sprintf(buf, "t21.txt=\"%s\"\xff\xff\xff", (SetDipSw.PlayFunEn & FANBEI_FLG) ? "开" : "关");
                        HTMSend(buf);
                        sprintf(buf, "t22.txt=\"%s\"\xff\xff\xff", (SetDipSw.PlayFunEn & ALLWIN_FLG) ? "开" : "关");
                        HTMSend(buf);
                        sprintf(buf, "t23.txt=\"%s\"\xff\xff\xff", (SetDipSw.PlayFunEn & SONGDENG_FLG) ? "开" : "关");
                        HTMSend(buf);
                    }
                    else
                    {
                        sprintf(buf, "t2.txt=\"密码错误！\"\xff\xff\xff");
                        HTMSend(buf);
                    }
                    break;
                case 4:
                    if (handpoint->password == SystemPassWord)
                    {
                        sprintf(buf, "systemconfig.va0.val=0\xff\xff\xff");
                        HTMSend(buf);
                        handpoint->Oldpage = handpoint->page;
                        handpoint->Oldctr = handpoint->ctr;
                        handpoint->GameIndex = CONFIG_CRT2;
                        handpoint->line_num = 0;
                        handpoint->machine_id = 0;
                        ReadSysTime(&SysTime);
                        memset(SetFutoryTime, 0, sizeof(SetFutoryTime));
                        memset(SetProgramVel, 0, sizeof(SetProgramVel));
                        memcpy(&SetTime, &SysTime, sizeof(SetTime));
                        sprintf(buf, "page 4\xff\xff\xff");
                        HTMSend(buf);
                        sprintf(buf, "t2.txt=\"%ld\"\xff\xff\xff", Machine_line);
                        HTMSend(buf);
                        sprintf(buf, "t3.txt=\"%ld\"\xff\xff\xff", Machine_id);
                        HTMSend(buf);
                        sprintf(buf, "t4.txt=\"%s\"\xff\xff\xff", FutoryTime);
                        HTMSend(buf);
                        sprintf(buf, "t5.txt=\"%s\"\xff\xff\xff", ProgramVel);
                        HTMSend(buf);
                        sprintf(buf, "t6.txt=\"%04d-%02d-%02d\"\xff\xff\xff", SetTime.year, SetTime.month, SetTime.day);
                        HTMSend(buf);
                        sprintf(buf, "t15.txt=\"%02d.%02d.%02d\"\xff\xff\xff", SetTime.hour, SetTime.min, SetTime.sec);
                        HTMSend(buf);
                        sprintf(buf, "t7.txt=\"%ld\"\xff\xff\xff", AccoutPassWord);
                        HTMSend(buf);
                        sprintf(buf, "t8.txt=\"%ld\"\xff\xff\xff", ConfigPassWord);
                        HTMSend(buf);
                        sprintf(buf, "t9.txt=\"%ld\"\xff\xff\xff", SystemPassWord);
                        HTMSend(buf);
                        sprintf(buf, "t10.txt=\"%ld\"\xff\xff\xff", "89545623");
                        HTMSend(buf);
                        sprintf(buf, "t11.txt=\"\"\xff\xff\xff");
                        HTMSend(buf);
                        sprintf(buf, "t12.txt=\"\"\xff\xff\xff");
                        HTMSend(buf);
                        sprintf(buf, "t13.txt=\"\"\xff\xff\xff");
                        HTMSend(buf);
                    }
                    else if (handpoint->password == 12345678)
                    {
                        sprintf(buf, "systemconfig.va0.val=1\xff\xff\xff");
                        HTMSend(buf);
                        handpoint->Oldpage = handpoint->page;
                        handpoint->Oldctr = handpoint->ctr;
                        handpoint->GameIndex = CONFIG_CRT2;
                        handpoint->line_num = 0;
                        handpoint->machine_id = 0;
                        memset(SetFutoryTime, 0, sizeof(SetFutoryTime));
                        memset(SetProgramVel, 0, sizeof(SetProgramVel));
                        memcpy(&SetTime, &SysTime, sizeof(SetTime));
                        sprintf(buf, "page 4\xff\xff\xff");
                        HTMSend(buf);
                        sprintf(buf, "t2.txt=\"%ld\"\xff\xff\xff", Machine_line);
                        HTMSend(buf);
                        sprintf(buf, "t3.txt=\"%ld\"\xff\xff\xff", Machine_id);
                        HTMSend(buf);
                        sprintf(buf, "t4.txt=\"%s\"\xff\xff\xff", FutoryTime);
                        HTMSend(buf);
                        sprintf(buf, "t5.txt=\"%s\"\xff\xff\xff", ProgramVel);
                        HTMSend(buf);
                        sprintf(buf, "t6.txt=\"%04d-%02d-%02d\"\xff\xff\xff", SetTime.year, SetTime.month, SetTime.day);
                        HTMSend(buf);
                        sprintf(buf, "t15.txt=\"%02d.%02d.%02d\"\xff\xff\xff", SetTime.hour, SetTime.min, SetTime.sec);
                        HTMSend(buf);
                        sprintf(buf, "t7.txt=\"%ld\"\xff\xff\xff", AccoutPassWord);
                        HTMSend(buf);
                        sprintf(buf, "t8.txt=\"%ld\"\xff\xff\xff", ConfigPassWord);
                        HTMSend(buf);
                        sprintf(buf, "t9.txt=\"%ld\"\xff\xff\xff", SystemPassWord);
                        HTMSend(buf);
                        sprintf(buf, "t10.txt=\"%ld\"\xff\xff\xff", handpoint->xuleihao);
                        HTMSend(buf);
                        sprintf(buf, "t11.txt=\"\"\xff\xff\xff");
                        HTMSend(buf);
                        sprintf(buf, "t12.txt=\"\"\xff\xff\xff");
                        HTMSend(buf);
                        sprintf(buf, "t13.txt=\"\"\xff\xff\xff");
                        HTMSend(buf);
                    }
                    else
                    {
                        sprintf(buf, "t2.txt=\"密码错误！\"\xff\xff\xff");
                        HTMSend(buf);
                    }
                    break;
                default:
                    break;
                }
            }
            else if (handpoint->ctr == 2)
            {
                handpoint->password = strtodex(handpoint->data);
                switch (handpoint->Oldctr)
                {
                case 1:
                    sprintf(buf, "t0.txt=\"输入查账密码\"\xff\xff\xff");
                    HTMSend(buf);
                    break;
                case 3:
                    sprintf(buf, "t0.txt=\"输入参数密码\"\xff\xff\xff");
                    HTMSend(buf);
                    break;
                case 4:
                    sprintf(buf, "t0.txt=\"输入系统密码\"\xff\xff\xff");
                    HTMSend(buf);
                    break;
                }
            }
            else if (handpoint->ctr == 4)
            {
                handpoint->GameIndex = MAIN_MUNE;
                sprintf(buf, "page 0\xff\xff\xff");
                HTMSend(buf);
                sprintf(buf, "t0.txt=\"%s\"\xff\xff\xff", GameName);
                HTMSend(buf);
            }
        }
    }
}
//----------------------------------------------------------------
// 查账
//----------------------------------------------------------------
void GameSetAudit(HANDTYPE *handpoint)
{
    char buf[50];
    if (handpoint->rx_ok_flg)
    {
        handpoint->rx_ok_flg = 0;
        if (handpoint->page == 1)
        {
            if (handpoint->ctr == 1)
            {
                handpoint->gamestation = 0;
                handpoint->GameIndex = MAIN_MUNE;
                sprintf(buf, "page 0\xff\xff\xff");
                HTMSend(buf);
                sprintf(buf, "t0.txt=\"%s\"\xff\xff\xff", GameName);
                HTMSend(buf);
            }
        }
    }
}
//----------------------------------------------------------------
// 报账
//----------------------------------------------------------------
void GameSetBaoAudit(HANDTYPE *handpoint)
{
    char buf[50];
    if (handpoint->rx_ok_flg)
    {
        handpoint->rx_ok_flg = 0;
        if (handpoint->page == 2)
        {
            if (handpoint->ctr == 8)
            {
                handpoint->password = strtodex(handpoint->data);
            }
            else if (handpoint->ctr == 1) // 确定
            {
                // 判断报账
            }
            else if (handpoint->ctr == 2) // 返回
            {
                handpoint->gamestation = 0;
                handpoint->GameIndex = MAIN_MUNE;
                sprintf(buf, "page 0\xff\xff\xff");
                HTMSend(buf);
                sprintf(buf, "t0.txt=\"%s\"\xff\xff\xff", GameName);
                HTMSend(buf);
            }
        }
    }
}
//----------------------------------------------------------------
// 基本设置
//----------------------------------------------------------------
void GameSetBaseSet(HANDTYPE *handpoint)
{
    char buf[50];
    if (handpoint->rx_ok_flg)
    {
        handpoint->rx_ok_flg = 0;
        if (handpoint->page == 3)
        {
            switch (handpoint->ctr)
            {
            case 4: // 投币设置
                handpoint->password = strtodex(handpoint->data);
                if (handpoint->password > 1000)
                {
                    SetDipSw.CoinIn = 1000;
                    sprintf(buf, "t3.txt=\"%ld\"\xff\xff\xff", GetCoinIn(&SetDipSw));
                    HTMSend(buf);
                }
                else
                {
                    SetDipSw.CoinIn = handpoint->password;
                }
                break;
            case 5: // 退币
                handpoint->password = strtodex(handpoint->data);
                if (handpoint->password > 1000)
                {
                    SetDipSw.CoinOut = 1000;
                    sprintf(buf, "t4.txt=\"%ld\"\xff\xff\xff", GetCoinOut(&SetDipSw));
                    HTMSend(buf);
                }
                else
                {
                    SetDipSw.CoinOut = handpoint->password;
                }
                break;
            case 6: // 上分
                handpoint->password = strtodex(handpoint->data);
                if (handpoint->password > 1000)
                {
                    SetDipSw.NoteIn = 1000;
                    sprintf(buf, "t5.txt=\"%ld\"\xff\xff\xff", GetNoteIn(&SetDipSw));
                    HTMSend(buf);
                }
                else
                {
                    SetDipSw.NoteIn = handpoint->password;
                }
                break;
            case 7: // 下分
                handpoint->password = strtodex(handpoint->data);
                if (handpoint->password > 1000)
                {
                    SetDipSw.NoteOut = 1000;
                    sprintf(buf, "t6.txt=\"%ld\"\xff\xff\xff", GetNoteOUt(&SetDipSw));
                    HTMSend(buf);
                }
                else
                {
                    SetDipSw.NoteOut = handpoint->password;
                }
                break;
            case 8: // 限红
                handpoint->password = strtodex(handpoint->data);
                if (handpoint->password > 100000)
                {
                    SetDipSw.DecIndex = 100000;
                    sprintf(buf, "t7.txt=\"%ld\"\xff\xff\xff", GetDec(&SetDipSw));
                    HTMSend(buf);
                }
                else
                {
                    SetDipSw.DecIndex = handpoint->password;
                }
                break;
            case 9: // 最小押分
                handpoint->password = strtodex(handpoint->data);
                if (handpoint->password > 1000)
                {
                    SetDipSw.Minbet = 1000;
                    sprintf(buf, "t8.txt=\"%ld\"\xff\xff\xff", GetMinbet(&SetDipSw));
                    HTMSend(buf);
                }
                else
                {
                    SetDipSw.Minbet = handpoint->password;
                }
                break;
            case 10: // 最大押分
                handpoint->password = strtodex(handpoint->data);
                if (handpoint->password > 10000)
                {
                    SetDipSw.Maxbet = 10000;
                    sprintf(buf, "t9.txt=\"%ld\"\xff\xff\xff", GetMaxbet(&SetDipSw));
                    HTMSend(buf);
                }
                else
                {
                    SetDipSw.Maxbet = handpoint->password;
                }
                break;
            case 11: // 游戏时间
                handpoint->password = strtodex(handpoint->data);
                if (handpoint->password > 240)
                {
                    SetDipSw.TimeIndex = 240;
                    sprintf(buf, "t10.txt=\"%ld\"\xff\xff\xff", GetGameTime(&SetDipSw));
                    HTMSend(buf);
                }
                else
                {
                    SetDipSw.TimeIndex = handpoint->password;
                }
                break;
            case 12: // 牌路选择
                handpoint->password = strtodex(handpoint->data);
                if (handpoint->password > 12)
                {
                    SetDipSw.RateIndex = 12;
                    sprintf(buf, "t11.txt=\"%ld\"\xff\xff\xff", GetRate(&SetDipSw));
                    HTMSend(buf);
                }
                else
                {
                    SetDipSw.RateIndex = handpoint->password;
                }
                break;
            case 13: // 破台分数
                handpoint->password = strtodex(handpoint->data);
                if (handpoint->password > 100000)
                {
                    SetDipSw.OverIndex = 100000;
                    sprintf(buf, "t12.txt=\"%ld\"\xff\xff\xff", GetChildOver(&SetDipSw));
                    HTMSend(buf);
                }
                else
                {
                    SetDipSw.OverIndex = handpoint->password;
                }
                break;
            case 14: // 爆机分数
                handpoint->password = strtodex(handpoint->data);
                if (handpoint->password > 1000000)
                {
                    SetDipSw.GameOver = 1000000;
                    sprintf(buf, "t13.txt=\"%ld\"\xff\xff\xff", GetGameOver(&SetDipSw));
                    HTMSend(buf);
                }
                else
                {
                    SetDipSw.GameOver = handpoint->password;
                }
                break;
            case 15: // 幸运率
                handpoint->password = strtodex(handpoint->data);
                if (handpoint->password == 0)
                {
                    SetDipSw.LuckIndex = 0;
                    sprintf(buf, "t14.txt=\"%ld\"\xff\xff\xff", GetLuckIndex(&SetDipSw) + 1);
                    HTMSend(buf);
                }
                else if (handpoint->password > 16)
                {
                    SetDipSw.LuckIndex = 15;
                    sprintf(buf, "t14.txt=\"%ld\"\xff\xff\xff", GetLuckIndex(&SetDipSw) + 1);
                    HTMSend(buf);
                }
                else
                {
                    SetDipSw.LuckIndex = handpoint->password - 1;
                }
                break;
            case 16: // 最小彩金
                handpoint->password = strtodex(handpoint->data);
                if (handpoint->password < 10)
                {
                    SetDipSw.MinBonus = 10;
                    sprintf(buf, "t15.txt=\"%ld\"\xff\xff\xff", GetMinBonus(&SetDipSw));
                    HTMSend(buf);
                }
                else if (handpoint->password > 10000)
                {
                    SetDipSw.MinBonus = 10000;
                    sprintf(buf, "t15.txt=\"%ld\"\xff\xff\xff", GetMinBonus(&SetDipSw));
                    HTMSend(buf);
                }
                else
                {
                    SetDipSw.MinBonus = handpoint->password;
                }
                break;
            case 17: // 最大彩金
                handpoint->password = strtodex(handpoint->data);
                if (handpoint->password > 100000)
                {
                    SetDipSw.MaxBonus = 100000;
                    sprintf(buf, "t16.txt=\"%ld\"\xff\xff\xff", GetMaxBonus(&SetDipSw));
                    HTMSend(buf);
                }
                else if (handpoint->password < 1000)
                {
                    SetDipSw.MaxBonus = 1000;
                    sprintf(buf, "t16.txt=\"%ld\"\xff\xff\xff", GetMaxBonus(&SetDipSw));
                    HTMSend(buf);
                }
                else
                {
                    SetDipSw.MaxBonus = handpoint->password;
                }
                break;
            case 18: // 演示开关
                if (SetDipSw.PlayFunEn & PLAYING_FLG)
                {
                    SetDipSw.PlayFunEn &= (~PLAYING_FLG);
                }
                else
                {
                    SetDipSw.PlayFunEn |= PLAYING_FLG;
                }
                sprintf(buf, "t17.txt=\"%s\"\xff\xff\xff", (SetDipSw.PlayFunEn & PLAYING_FLG) ? "开" : "关");
                HTMSend(buf);
                break;
            case 19: // 积分率
                handpoint->password = strtodex(handpoint->data);
                if (handpoint->password > 100)
                {
                    SetDipSw.Jifenrate = 100;
                    sprintf(buf, "t18.txt=\"%ld\"\xff\xff\xff", GetJifenRate(&SetDipSw));
                    HTMSend(buf);
                }
                else
                {
                    SetDipSw.Jifenrate = handpoint->password;
                }
                break;
            case 20: // 幸运位开关
                if (SetDipSw.PlayFunEn & LUCKBIT_FLG)
                {
                    SetDipSw.PlayFunEn &= (~LUCKBIT_FLG);
                }
                else
                {
                    SetDipSw.PlayFunEn |= LUCKBIT_FLG;
                }
                sprintf(buf, "t19.txt=\"%s\"\xff\xff\xff", (SetDipSw.PlayFunEn & LUCKBIT_FLG) ? "开" : "关");
                HTMSend(buf);
                break;
            case 21: // 明牌开关
                if (SetDipSw.PlayFunEn & MINGPAI_FLG)
                {
                    SetDipSw.PlayFunEn &= (~MINGPAI_FLG);
                }
                else
                {
                    SetDipSw.PlayFunEn |= MINGPAI_FLG;
                }
                sprintf(buf, "t20.txt=\"%s\"\xff\xff\xff", (SetDipSw.PlayFunEn & MINGPAI_FLG) ? "开" : "关");
                HTMSend(buf);
                break;
            case 22: // 加倍开关
                if (SetDipSw.PlayFunEn & FANBEI_FLG)
                {
                    SetDipSw.PlayFunEn &= (~FANBEI_FLG);
                }
                else
                {
                    SetDipSw.PlayFunEn |= FANBEI_FLG;
                }
                sprintf(buf, "t21.txt=\"%s\"\xff\xff\xff", (SetDipSw.PlayFunEn & FANBEI_FLG) ? "开" : "关");
                HTMSend(buf);
                break;
            case 23: // 全中开关
                if (SetDipSw.PlayFunEn & ALLWIN_FLG)
                {
                    SetDipSw.PlayFunEn &= (~ALLWIN_FLG);
                }
                else
                {
                    SetDipSw.PlayFunEn |= ALLWIN_FLG;
                }
                sprintf(buf, "t22.txt=\"%s\"\xff\xff\xff", (SetDipSw.PlayFunEn & ALLWIN_FLG) ? "开" : "关");
                HTMSend(buf);
            case 24: // 送灯开关
                if (SetDipSw.PlayFunEn & SONGDENG_FLG)
                {
                    SetDipSw.PlayFunEn &= (~SONGDENG_FLG);
                }
                else
                {
                    SetDipSw.PlayFunEn |= SONGDENG_FLG;
                }
                sprintf(buf, "t23.txt=\"%s\"\xff\xff\xff", (SetDipSw.PlayFunEn & SONGDENG_FLG) ? "开" : "关");
                HTMSend(buf);
                break;
            case 1: // 保存
                __24C04_FLAG |= S_DIP;
            case 2: // 退出
                handpoint->gamestation = 0;
                handpoint->GameIndex = MAIN_MUNE;
                sprintf(buf, "page 0\xff\xff\xff");
                HTMSend(buf);
                sprintf(buf, "t0.txt=\"%s\"\xff\xff\xff", GameName);
                HTMSend(buf);
                break;
            }
        }
    }
}
//----------------------------------------------------------------
// 时间设置
//----------------------------------------------------------------
void GameSetDateype(char *buf)
{
    uint32_t year, month, day;
    if (sscanf(buf, "%d-%d-%d", &year, &month, &day) != 3)
        return;
    if (year > 2000)
    {
        SetTime.year = year;
    }
    if (month < 13 && month > 0)
    {
        SetTime.month = month;
    }
    if (day < 32 && day > 0)
    {
        SetTime.day = day;
    }
}
void GameSetTimetype(char *buf)
{
    uint32_t hour, min, sec;
    if (sscanf(buf, "%d.%d.%d", &hour, &min, &sec) != 3)
        return;
    if (hour < 24)
    {
        SetTime.hour = hour;
    }
    if (min < 60)
    {
        SetTime.min = min;
    }
    if (sec < 60)
    {
        SetTime.sec = sec;
    }
}
//----------------------------------------------------------------
// 设置系统时间
//----------------------------------------------------------------
void GameSystemSet(HANDTYPE *handpoint)
{
    char buf[50];
    if (handpoint->rx_ok_flg)
    {
        handpoint->rx_ok_flg = 0;
        if (handpoint->page == 4)
        {
            switch (handpoint->ctr)
            {
            case 3: // 设置线号
                handpoint->line_num = strtodex(handpoint->data);
                sprintf(buf, "t2.txt=\"%d\"\xff\xff\xff", handpoint->line_num);
                HTMSend(buf);
                break;
            case 4: // 设置机号
                handpoint->machine_id = strtodex(handpoint->data);
                sprintf(buf, "t3.txt=\"%d\"\xff\xff\xff", handpoint->line_num);
                HTMSend(buf);
                break;
            case 5: // 出厂日期
                strcpy(SetFutoryTime, handpoint->data);
                sprintf(buf, "t4.txt=\"%s\"\xff\xff\xff", SetFutoryTime);
                HTMSend(buf);
                break;
            case 6: // 程序版本
                strcpy(SetProgramVel, handpoint->data);
                sprintf(buf, "t5.txt=\"%s\"\xff\xff\xff", SetFutoryTime);
                HTMSend(buf);
            case 7: // 系统日期
                GameSetDateype(handpoint->data);
                sprintf(buf, "t6.txt=\"%04d-%02d-%02d\"\xff\xff\xff", SetTime.year, SetTime.month, SetTime.day);
                HTMSend(buf);
                break;
            case 19: // 系统时间
                GameSetTimetype(handpoint->data);
                sprintf(buf, "t15.txt=\"%02d.%02d.%02d\"\xff\xff\xff", SetTime.hour, SetTime.min, SetTime.sec);
                HTMSend(buf);
                break;
            case 8: // 查账密码
                handpoint->password = strtodex(handpoint->data);
                SetAccoutPassWord = handpoint->password;
                break;
            case 9: // 参数密码
                handpoint->password = strtodex(handpoint->data);
                SetConfigPassWord = handpoint->password;
                break;
            case 10: // 系统密码
                handpoint->password = strtodex(handpoint->data);
                SetSystemPassWord = handpoint->password;
                break;
            case 12: // 球位设置
                break;
            case 13: // 机号重置
                break;
            case 14: // 机芯开锁
                break;
            case 1:                      // 确定
                if (handpoint->line_num) // 保存线号
                {
                    __24C04_FLAG |= S_MACHINELINE;
                }
                if (handpoint->machine_id) // 保存机号
                {
                    __24C04_FLAG |= S_MACHINEID;
                }
                if (strlen(SetFutoryTime)) // 保存出厂日期
                {
                    __24C04_FLAG |= S_FUTORYTIME;
                }
                if (strlen(SetProgramVel)) // 保存版本号
                {
                    __24C04_FLAG |= S_PROGRAMVEL;
                }
                if (SetAccoutPassWord != AccoutPassWord) // 保存查账密码
                {
                    __24C04_FLAG |= S_ACCOUTPASS;
                }
                if (SetConfigPassWord != ConfigPassWord) // 保存参数密码
                {
                    __24C04_FLAG |= S_CONFIGPASS;
                }
                if (SetSystemPassWord != SystemPassWord) // 保存系统密码
                {
                    __24C04_FLAG |= S_SYSTEMPASS;
                }
                if ((SetTime.hour != SysTime.hour) || (SetTime.min != SysTime.min) || (SetTime.sec != SysTime.sec) ||
                    (SetTime.year != SysTime.year) || (SetTime.month != SysTime.month) || (SetTime.day != SysTime.day))
                {
                    WriteSysTime(&SetTime);
                }
            case 2: // 取消
                handpoint->gamestation = 0;
                handpoint->GameIndex = MAIN_MUNE;
                sprintf(buf, "page 0\xff\xff\xff");
                HTMSend(buf);
                sprintf(buf, "t0.txt=\"%s\"\xff\xff\xff", GameName);
                HTMSend(buf);
                break;
            }
        }
    }
}
//----------------------------------------------------------------
// 设置球位
//----------------------------------------------------------------
const char *QiuName[] = {
    "空", "孙悟空", "猪八戒", "沙和尚", "唐僧", "玉帝"};
uint8_t QiuNumIndex[10][1] = {
    {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}};
void GameSetQui(HANDTYPE *handpoint)
{
    char buf[50];

    if (handpoint->rx_ok_flg)
    {
        handpoint->rx_ok_flg = 0;
        if (handpoint->page == 5)
        {
            if ((0 < handpoint->ctr) && (handpoint->ctr <= 0x0A)) // 控制出球
            {
                
                ChuQiuHao = handpoint->ctr - 1;
                QiuTest = 1;
                handpoint->SetVolume = 1;
                // sprintf(buf, "tsw 255,0\xff\xff\xff");
                //HTMSend(buf);
                QiuNumIndex[handpoint->ctr][0]++;
                if (QiuNumIndex[handpoint->ctr][0] >= 6)
                {
                    QiuNumIndex[handpoint->ctr][0] = 0;
                }
                sprintf(buf, "t%d.txt=\"%s\"\xff\xff\xff", handpoint->ctr, QiuName[QiuNumIndex[handpoint->ctr][0]]);
                HTMSend(buf);
            }
            else if ((0x10 <= handpoint->ctr) && (handpoint->ctr < 0x20))
            {
                uint8_t qiu;
                uint8_t wei = handpoint->ctr - 0x10;
                qiu = SetQiuwei[wei];
                qiu++;
                if (qiu >= sizeof(QiuName) / sizeof(QiuName[0]))
                    qiu = 0;
                SetQiuwei[wei] = qiu;
                sprintf(buf, "t%d.txt=\"%s\"\xff\xff\xff", wei + 1, QiuName[qiu]);
                HTMSend(buf);
            }
            else if (handpoint->ctr == 0x20) // 保存球位
            {
                memcpy(Qiuwei, SetQiuwei, sizeof(Qiuwei));
                SwtQiuFlag = 0;
                __24C04_FLAG |= S_QIUWEI;
                handpoint->gamestation = 0;
                handpoint->GameIndex = MAIN_MUNE;
                sprintf(buf, "page 0\xff\xff\xff");
                HTMSend(buf);
                sprintf(buf, "t0.txt=\"%s\"\xff\xff\xff", GameName);
                HTMSend(buf);
            }
            else if (handpoint->ctr == 0x21) // 取消保存
            {
                SwtQiuFlag = 0;
                handpoint->gamestation = 0;
                handpoint->GameIndex = MAIN_MUNE;
                sprintf(buf, "page 0\xff\xff\xff");
                HTMSend(buf);
                sprintf(buf, "t0.txt=\"%s\"\xff\xff\xff", GameName);
                HTMSend(buf);

            }
        }
    }
    if (handpoint->SetVolume)
    {
        if (QiuTest == 0)
        {
            // sprintf(buf, "tsw 255,1\xff\xff\xff");
            // HTMSend(buf);
        }
    }
    else if (QiuState)
    {
        // sprintf(buf, "tsw 255,1\xff\xff\xff");
        // HTMSend(buf);
    }
}
//----------------------------------------------------------------
// 头彩金设置
//----------------------------------------------------------------
void GameSetHardBonus(HANDTYPE *handpoint)
{
}
//----------------------------------------------------------------
// 参数设置功能选择
//----------------------------------------------------------------
void GameSetProc(HANDTYPE *handpoint)
{
    CPU_SR cpu_sr;
    if (Gamesetrx->Rx_ok_flg)
    {
        CPU_CRITICAL_ENTER();
        handpoint->page = Gamesetrx->RxBuf[0];
        handpoint->ctr = Gamesetrx->RxBuf[1];
        strcpy(handpoint->data, &Gamesetrx->RxBuf[2]);
        handpoint->rx_ok_flg = 1;
        Gamesetrx->Rx_ok_flg = 0;
        CPU_CRITICAL_EXIT();
    }
    switch (handpoint->GameIndex)
    {
    case INIT_WAIT: // 开机初始化
        GameSetInit(handpoint);
        break;
    case MAIN_MUNE: // 菜单
        GameSetMune(handpoint);
        break;
    case AUDIT_LOOK: // 查账
        GameSetAudit(handpoint);
        break;
    case AUDIT_INPUT:
        GameSetBaoAudit(handpoint);
        break;
    case INPUT_PASS: // 输入密码
        GameSetInputPass(handpoint);
        break;
    case CONFIG_CRT1: // 基本设置
        GameSetBaseSet(handpoint);
        break;
    case CONFIG_CRT2: // 系统设置
        GameSystemSet(handpoint);
        break;
    case SET_QIU: // 设置球位
        GameSetQui(handpoint);
        break;
    case SETBONUS:
        GameSetHardBonus(handpoint);
        break;
    default:
        GameSetInit(handpoint);
        break;
    }
}
