/*
 ******************************************************************************
 * @file    main.c
 * @author  fire
 * @version V1.0
 * @date    2016-xx-xx
 * @brief   uCOS-III 系统移植
 ******************************************************************************
 */
/*
*********************************************************************************************************
*                                            包含的文件
*********************************************************************************************************
*/
#include "main.h"

/*
*********************************************************************************************************
*                                         任务控制块TCB
*********************************************************************************************************
*/

static OS_TCB AppTaskWanTCB;
static OS_TCB AppTaskDrawTCB;
static OS_TCB AppTaskKeyTCB;
static OS_TCB AppTaskMainTCB;
/*
*********************************************************************************************************
*                                            任务堆栈
*********************************************************************************************************
*/
static CPU_STK AppTaskWanStk[APP_TASK_WAN_STK_SIZE];
static CPU_STK AppTaskDrawStk[APP_TASK_DRAW_STK_SIZE];
static CPU_STK AppTaskKeyStk[APP_TASK_KEY_STK_SIZE];
static CPU_STK AppTaskManiStk[APP_TASK_MAIN_STK_SIZE];
/*
*********************************************************************************************************
*                                            函数原型
*********************************************************************************************************
*/

static void AppWan(void *p_arg);
static void AppDraw(void *p_arg);
static void AppKey(void *p_arg);
static void AppMain(void *p_arg);

void Error_Handler(void)
{
    ;
}
//-----------------------------------------------------------------------------
// 初始化游戏分机数据
//-----------------------------------------------------------------------------
void Init_machine()
{
    uint32_t i;
    for (i = 0; i < MAX_FENJI; i++)
    {
        Child[i].pChildDisp = (ChildDispType *)(FPGAADDR + i * 128);
        Child[i].pChildInput = (ChildInputType *)(FPGAADDR + i * 16);
    }
    for (i = 0; i < 8 * 128; i++)
        *((uint8_t *)(FPGAADDR + i)) = 0x00; // 清除显示缓冲区
    for (i = 0; i < MAX_FENJI; i++)
    {
        Child[i].Count = 0; // 关闭计数器
        Child[i].CoinKeyCount = Child[i].pChildInput->KeyCount;
        Child[i].CoinInTmp = Child[i].CoinInTmp2 = 0;
        Child[i].CoinOutTmp = Child[i].CoinOutTmp2 = 0;
        Child[i].InitCoinOut = 0;
        Child[i].AgainInitCoinOut = 0;
    }
}
//-----------------------------------------------------------------------------
// 测试电池保存数据
//-----------------------------------------------------------------------------
#define SRAM_FLG 0x88354366
void Sram_check(uint32_t n)
{
    uint8_t *point;
    uint32_t i;
    if ((Sramflg != SRAM_FLG) || n)
    {
        point = (uint8_t *)(0x38800000);
        for (i = 0; i < 0x1000; i++)
        {
            *point = 0;
            point++;
        }
        Sramflg = SRAM_FLG;
        for (i = 0; i < MAX_FENJI; i++)
        {
            Child[i].Conv = 1;
        }
    }
}
//-----------------------------------------------------------------------------
// 初始化游戏数据
//-----------------------------------------------------------------------------
uint8_t CheckEEPROM(void)
{
    uint32_t Init_flg;
    ReadEEPROM(eeIICflg, (uint8_t *)&Init_flg, sizeof(Init_flg));
    if (Init_flg != IICINIT)
    {
        Init_flg = IICINIT;
        WriteEEPROM(eeIICflg, (uint8_t *)&Init_flg, sizeof(Init_flg));
        return 1;
    }
    return 0;
}

void ClearZero(uint8_t n)
{
    uint8_t i;
    DipSwType *dipswpoint;
    ChildType *pChild;
    if (n)
    {
        dipswpoint = &DipSw;
        dipswpoint->NoteIn = 100;
        dipswpoint->NoteOut = 100;
        dipswpoint->CoinIn = 1;
        dipswpoint->CoinOut = 1;
        dipswpoint->DecIndex = 1000;
        dipswpoint->RateIndex = 0;
        dipswpoint->OverIndex = 100000;
        dipswpoint->GameOver = 1000000;
        dipswpoint->Minbet = 10;
        dipswpoint->Maxbet = 9999;
        dipswpoint->MinJifen = 0;
        dipswpoint->Jifenrate = 0;
        dipswpoint->LuckIndex = 0;
        dipswpoint->Playing = 0;
        dipswpoint->PlayFunEn = 0;
        dipswpoint->TimeIndex = 30;
        dipswpoint->MaxBonus = 10000;
        dipswpoint->Maxlinebigbonus = 50000;
        dipswpoint->Minlinebigbonus = 1000;
        dipswpoint->Maxlinemidbonus = 10000;
        dipswpoint->Minlinemidbonus = 1000;
        dipswpoint->Maxlinelitbonus = 1000;
        dipswpoint->Minlinelitbonus = 100;

        WriteEEPROM(eeDipSw, (uint8_t *)dipswpoint, sizeof(DipSw));

        Machine_line = 888;
        Machine_id = 1234567;
        WriteEEPROM(eeMachine_line, (uint8_t *)&Machine_line, sizeof(Machine_line));
        WriteEEPROM(eeMachine_id, (uint8_t *)&Machine_id, sizeof(Machine_id));

        CurrPayoff = 0;
        WriteEEPROM(eeCurrPayoff, (uint8_t *)&CurrPayoff, sizeof(CurrPayoff));

        AccoutPassWord = 0;
        ConfigPassWord = 0;
        SystemPassWord = 16881688;
        WriteEEPROM(eeAccoutPassWord, (uint8_t *)&AccoutPassWord, sizeof(AccoutPassWord));
        WriteEEPROM(eeConfigPassWord, (uint8_t *)&ConfigPassWord, sizeof(ConfigPassWord));
        WriteEEPROM(eeSystemPassWord, (uint8_t *)&SystemPassWord, sizeof(SystemPassWord));

        memset(FutoryTime, 0, sizeof(FutoryTime));
        memset(ProgramVel, 0, sizeof(ProgramVel));
        WriteEEPROM(eeFutoryTime, (uint8_t *)FutoryTime, sizeof(FutoryTime));
        WriteEEPROM(eeProgramVel, (uint8_t *)ProgramVel, sizeof(ProgramVel));

        Qiuwei[0] = 1;
        Qiuwei[1] = 2;
        Qiuwei[2] = 3;
        Qiuwei[3] = 4;
        Qiuwei[4] = 5;
        Qiuwei[5] = 0;
        Qiuwei[6] = 0;
        Qiuwei[7] = 0;
        Qiuwei[8] = 0;
        Qiuwei[9] = 0;
        WriteEEPROM(eeQiuwei, Qiuwei, sizeof(Qiuwei));

        for (i = 0; i < MAX_FENJI; i++)
        {
            pChild = &Child[i];
            pChild->Credit = 0;
            pChild->CoinIn = 0;
            pChild->CoinOut = 0;
            pChild->KeyIn = 0;
            pChild->KeyOut = 0;
            pChild->AllBet8 = 0;
            pChild->AllWin8 = 0;

            WriteEEPROM(eeCredit + 4 * i, (uint8_t *)&(pChild->Credit), W_INT4);
            WriteEEPROM(eeCOIN_IN + 4 * i, (uint8_t *)&(pChild->CoinIn), W_INT4);
            WriteEEPROM(eeCOIN_OUT + 4 * i, (uint8_t *)&(pChild->CoinOut), W_INT4);
            WriteEEPROM(eeKEY_IN + 4 * i, (uint8_t *)&(pChild->KeyIn), W_INT4);
            WriteEEPROM(eeKEY_OUT + 4 * i, (uint8_t *)&(pChild->KeyOut), W_INT4);
            WriteEEPROM(eeAllBet8 + 8 * i, (uint8_t *)&(pChild->AllBet8), W_INT8);
            WriteEEPROM(eeAllWin8 + 8 * i, (uint8_t *)&(pChild->AllWin8), W_INT8);
        }
    }
    else
    {
        ReadEEPROM(eeDipSw, (uint8_t *)&DipSw, sizeof(DipSw));
        ReadEEPROM(eeMachine_line, (uint8_t *)&Machine_line, sizeof(Machine_line));
        ReadEEPROM(eeMachine_id, (uint8_t *)&Machine_id, sizeof(Machine_id));
        ReadEEPROM(eeCurrPayoff, (uint8_t *)&CurrPayoff, sizeof(CurrPayoff));
        ReadEEPROM(eeAccoutPassWord, (uint8_t *)&AccoutPassWord, sizeof(AccoutPassWord));
        ReadEEPROM(eeConfigPassWord, (uint8_t *)&ConfigPassWord, sizeof(ConfigPassWord));
        ReadEEPROM(eeSystemPassWord, (uint8_t *)&SystemPassWord, sizeof(SystemPassWord));
        ReadEEPROM(eeFutoryTime, (uint8_t *)FutoryTime, sizeof(FutoryTime));
        ReadEEPROM(eeProgramVel, (uint8_t *)ProgramVel, sizeof(ProgramVel));
        ReadEEPROM(eeQiuwei, Qiuwei, sizeof(Qiuwei));
        for (i = 0; i < MAX_FENJI; i++)
        {
            pChild = &Child[i];
            ReadEEPROM(eeCredit + 4 * i, (uint8_t *)&(pChild->Credit), W_INT4);
            ReadEEPROM(eeCOIN_IN + 4 * i, (uint8_t *)&(pChild->CoinIn), W_INT4);
            ReadEEPROM(eeCOIN_OUT + 4 * i, (uint8_t *)&(pChild->CoinOut), W_INT4);
            ReadEEPROM(eeKEY_IN + 4 * i, (uint8_t *)&(pChild->KeyIn), W_INT4);
            ReadEEPROM(eeKEY_OUT + 4 * i, (uint8_t *)&(pChild->KeyOut), W_INT4);
            ReadEEPROM(eeAllBet8 + 8 * i, (uint8_t *)&(pChild->AllBet8), W_INT8);
            ReadEEPROM(eeAllWin8 + 8 * i, (uint8_t *)&(pChild->AllWin8), W_INT8);
        }
    }
    Gamestation = 0;
}

/*
*********************************************************************************************************
* 函数名 : main
*********************************************************************************************************
*/
int main(void)
{
    OS_ERR err;
    MPU_Config();
    HAL_Init();                   // HAL初始化,见注意 1
    BSP_SystemClkCfg();           // 初始化CPU频率为 400Mhz
    CPU_Init();                   // 初始化 CPU 组件（时间戳、关中断时间测量和主机名）
    Mem_Init();                   // 初始化内存管理组件（堆内存池和内存池表）
    MX_GPIO_Init();               // 初始化GPIO时钟
    MX_FMC_Init();                // 初始化FMC接口
    MX_RTC_Init();                // RTC初始化
    BKP_SRAM_Init();              // 初始化备份区
    MX_RNG_Init();                // 随机数初始化
    MX_USART1_UART_Init();        // 主机显示串口
    MX_USART2_UART_Init();        // 液晶控制屏串口
    MX_USART3_UART_Init();        // 短信串口
    MX_UART4_Init();              // 网口串口
    MX_UART5_Init();              // 机芯串口
    MX_USART6_UART_Init();        //
    MX_SPI1_Init();               // 初始化SPI接口
    ClearZero(CheckEEPROM());     //
    Init_machine();               // 初始化分机接口
    MX_TIM2_Init();               // 初始化退币时间中断
    HAL_TIM_Base_MspInit(&htim2); // 启动中断
    CPU_IntDis();                 // 禁止所有中断
    OSInit(&err);                 // 初始化uC/OS-III系统
    /* 创建起始任务 */
    OSTaskCreate((OS_TCB *)&AppTaskDrawTCB,                           // 任务控制块地址
                 (CPU_CHAR *)"App Task Draw",                         // 任务名称
                 (OS_TASK_PTR)AppDraw,                                // 任务函数
                 (void *)0,                                           // 传递给任务函数（形参p_arg）的实参
                 (OS_PRIO)APP_TASK_DRAW_PRIO,                         // 任务的优先级
                 (CPU_STK *)&AppTaskDrawStk[0],                       // 任务堆栈的基地址
                 (CPU_STK_SIZE)APP_TASK_DRAW_STK_SIZE / 10,           // 任务堆栈空间剩下1/10时限制其增长
                 (CPU_STK_SIZE)APP_TASK_DRAW_STK_SIZE,                // 任务堆栈空间（单位：sizeof(CPU_STK)）
                 (OS_MSG_QTY)5u,                                      // 任务可接收的最大消息数
                 (OS_TICK)0u,                                         // 任务的时间片节拍数（0表默认值OSCfg_TickRate_Hz/10）
                 (void *)0,                                           // 任务扩展（0表不扩展）
                 (OS_OPT)(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), // 任务选项
                 (OS_ERR *)&err);                                     // 返回错误类型

    OSStart(&err); // 启动多任务管理（交由uC/OS-III控制）
}

/**********************************************************************************************************
 * 函数名 AppDraw
 **********************************************************************************************************/

static void AppDraw(void *p_arg)
{
    OS_ERR err;
    HANDTYPE HandCtr;
    struct GameSetRx Gameset;
    uint8_t led_flash = 0;
    uint8_t led_flash_time = 0;
    (void)p_arg;
    BSP_Init();                           // 板级初始化
    OSTimeDly(10, OS_OPT_TIME_DLY, &err); // 延时1s
    /* 创建按键任务 */
    OSTaskCreate((OS_TCB *)&AppTaskKeyTCB,                            // 任务控制块地址
                 (CPU_CHAR *)"App Task key",                          // 任务名称
                 (OS_TASK_PTR)AppKey,                                 // 任务函数
                 (void *)0,                                           // 传递给任务函数（形参p_arg）的实参
                 (OS_PRIO)APP_TASK_KEY_PRIO,                          // 任务的优先级
                 (CPU_STK *)&AppTaskKeyStk[0],                        // 任务堆栈的基地址
                 (CPU_STK_SIZE)APP_TASK_KEY_STK_SIZE / 10,            // 任务堆栈空间剩下1/10时限制其增长
                 (CPU_STK_SIZE)APP_TASK_KEY_STK_SIZE,                 // 任务堆栈空间（单位：sizeof(CPU_STK)）
                 (OS_MSG_QTY)5u,                                      // 任务可接收的最大消息数
                 (OS_TICK)0u,                                         // 任务的时间片节拍数（0表默认值OSCfg_TickRate_Hz/10）
                 (void *)0,                                           // 任务扩展（0表不扩展）
                 (OS_OPT)(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), // 任务选项
                 (OS_ERR *)&err);                                     // 返回错误类型

    /* 创建游戏主任务 */
    OSTaskCreate((OS_TCB *)&AppTaskMainTCB,                           // 任务控制块地址
                 (CPU_CHAR *)"App Task Main",                         // 任务名称
                 (OS_TASK_PTR)AppMain,                                // 任务函数
                 (void *)0,                                           // 传递给任务函数（形参p_arg）的实参
                 (OS_PRIO)APP_TASK_MAIN_PRIO,                         // 任务的优先级
                 (CPU_STK *)&AppTaskManiStk[0],                       // 任务堆栈的基地址
                 (CPU_STK_SIZE)APP_TASK_MAIN_STK_SIZE / 10,           // 任务堆栈空间剩下1/10时限制其增长
                 (CPU_STK_SIZE)APP_TASK_MAIN_STK_SIZE,                // 任务堆栈空间（单位：sizeof(CPU_STK)）
                 (OS_MSG_QTY)5u,                                      // 任务可接收的最大消息数
                 (OS_TICK)0u,                                         // 任务的时间片节拍数（0表默认值OSCfg_TickRate_Hz/10）
                 (void *)0,                                           // 任务扩展（0表不扩展）
                 (OS_OPT)(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), // 任务选项
                 (OS_ERR *)&err);                                     // 返回错误类型

    // /* 创建Led3任务 */
    // OSTaskCreate((OS_TCB *)&AppTaskWanTCB,                           // 任务控制块地址
    //              (CPU_CHAR *)"App Task Wan",                         // 任务名称
    //              (OS_TASK_PTR)AppWan,                            // 任务函数
    //              (void *)0,                                           // 传递给任务函数（形参p_arg）的实参
    //              (OS_PRIO)APP_TASK_WAN_PRIO,                         // 任务的优先级
    //              (CPU_STK *)&AppTaskWanStk[0],                       // 任务堆栈的基地址
    //              (CPU_STK_SIZE)APP_TASK_WAN_STK_SIZE / 10,           // 任务堆栈空间剩下1/10时限制其增长
    //              (CPU_STK_SIZE)APP_TASK_WAN_STK_SIZE,                // 任务堆栈空间（单位：sizeof(CPU_STK)）
    //              (OS_MSG_QTY)5u,                                      // 任务可接收的最大消息数
    //              (OS_TICK)0u,                                         // 任务的时间片节拍数（0表默认值OSCfg_TickRate_Hz/10）
    //              (void *)0,                                           // 任务扩展（0表不扩展）
    //              (OS_OPT)(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), // 任务选项
    //              (OS_ERR *)&err);                                     // 返回错误类型
    // LED1_OFF; // 关闭RGB LED
    Gamesetrx = &Gameset;
    Gamesetrx->Rx_ok_flg = 0;
    Gamesetrx->RxCnt = 0;
    Gamesetrx->RxendCnt = 0;
    Gamesetrx->Tx_busy_flg = 0;
    Gamesetrx->TxLen = 0;
    Gamesetrx->TxCnt = 0;
    HandCtr.GameIndex = INIT_WAIT;
    HandCtr.page = 0;
    HandCtr.ctr = 0;
    HandCtr.rx_ok_flg = 0;
    Init_UartSet();
    OSTimeDly(OS_TICKS_PER_SEC, OS_OPT_TIME_DLY, &err); // 延时1s
    ReadSysTime(&SysTime);
    while (1)
    {
        GameSetProc(&HandCtr);
        led_flash_time++;
        if (led_flash_time >= 50)
        {
            led_flash_time = 0;
            if (led_flash == 1)
            {
                LED1_OFF;
                led_flash = 0;
            }
            else
            {
                LED1_ON;
                led_flash = 1;
            }
        }
        OSTimeDly(1, OS_OPT_TIME_DLY, &err); // 延时1s
    }
}
/*
*********************************************************************************************************
*按键任务
*********************************************************************************************************
*/
static void AppKey(void *p_arg)
{
    OS_ERR err;
    uint8_t tmp;
    uint8_t led_flash = 0;
    uint8_t led_cnt = 0;
    MasterUartTYPE masteruart;
    MasterBufTYPE masterbuf;
    MasterCmdTYPE mastercmd;
    ChildCmdTYPE childcmd;
    (void)p_arg; // 没有用到形参，防止编译器报错
    MasterCmdpoint = &mastercmd;
    MasterCmdpoint->Cmdstart = 0;
    MasterCmdpoint->Cmdend = 0;
    MasterCmdpoint->Cmd = 0;
    masterbuf.Hardflg = 0;
    masterbuf.cnt = 0;
    masterbuf.linkcnt = 10000;
    MasterUart = &masteruart;
    MasterUart->TxCnt = 0;
    MasterUart->TxLen = 0;
    MasterUart->Tx_busy_flg = 0;
    MasterUart->RxStart = 0;
    MasterUart->RxEnd = 0;
    ChildCmdpoint = &childcmd;
    ChildCmdpoint->Cmdstart = 0;
    ChildCmdpoint->Cmdend = 0;
    ChildCmdpoint->Cmd = 0;
    Init_MasterUartSet();
    KeyPress = 0;
    for (tmp = 0; tmp < MAX_FENJI; tmp++)
    {
        memcpy(&KeyTaskTbl[tmp], InitKeyTaskTbl, sizeof(InitKeyTaskTbl));
        memset(Child[tmp].ChildKeyCount, 0, sizeof(Child[tmp].ChildKeyCount));
        Child[tmp].ChildKeyPress = 0;
    }
    OSTimeDly(OS_TICKS_PER_SEC, OS_OPT_TIME_DLY, &err);
    while (DEF_TRUE) // 任务体，通常都写成一个死循环
    {
        Master_key_proc();
        if (KeyPress)
        {
            uartprintf("key=%x", KeyPress);
            KeyPress = 0;
        }
        for (tmp = 0; tmp < MAX_FENJI; tmp++)
        {
            Child_key_proc(tmp);
        }
        led_cnt++;
        //if ((led_cnt & 0x1f) == 0x1f)
        if (led_cnt >= 50)
        {
            led_cnt = 0;
            if (led_flash)
            {
                led_flash = 0;
                Fpga_wreg->Led = 1;
            }
            else
            {
                led_flash = 1;
                Fpga_wreg->Led = 0;
            }
        }
        if (led_cnt & 1)
        {
            MasterUart_pro(MasterCmdpoint);
            ReadChildcmd(ChildCmdpoint);
            for (tmp = 0; tmp < MAX_FENJI; tmp++)
            {
                SendChild(&Child[tmp], ChildCmdpoint, tmp + 1);
            }
        }
        RevMaster(&masterbuf);
        //OSTimeDly(2, OS_OPT_TIME_DLY, &err); // 延时1s
        OSTimeDly(1, OS_OPT_TIME_DLY, &err); // 延时1s
    }
}
/**********************************************************************************************************
 *AppMain 任务
 **********************************************************************************************************/
static void AppMain(void *p_arg)
{
    OS_ERR err;
    (void)p_arg; // 没有用到形参，防止编译器报错
    Scene();

    // 复位转球机
    #ifdef  __USE_JIXIN__
    Init_JiXinUartSet();
    MachineReSet(); 
    #else
    #warning "没有使用顶球机芯"
    #endif
    
    // 游戏初始化
    while (DEF_TRUE)
    {
        for (ProcMem = 0;;)
        {
            switch (ProcMem)
            {
            case 0:
                ReStart();
                break;
            case 1:
                Start();
                break;
            case 2:
                WaitBET();
                break;
            case 3:
                StartRun();
                break;
            case 4:
                WinProc();
                break;
            case 5:
                GameEnd();
                break;
            case 6:
                GameEnd2();
                break;
            case 7:
                #ifdef  __USE_JIXIN__
                GameSetBall();
                #else
                ProcMem = 1;
                #endif
                break;
            default:
                ProcMem = 0;
                break;
            }
            OSTimeDly(1, OS_OPT_TIME_DLY, &err); // 延时
        }
    }
}
/**********************************************************************************************************
 *AppWan 任务
 **********************************************************************************************************/

static void AppWan(void *p_arg)
{
    OS_ERR err;

    (void)p_arg; // 没有用到形参，防止编译器报错

    //    LED_RGBOFF;                                      //关闭RGB LED

    while (DEF_TRUE) // 任务体，通常都写成一个死循环
    {
        //        LED3_TOGGLE;                                 //LED3闪烁
        // printf("LED3_TOGGLE\r\n");
        OSTimeDly(10000, OS_OPT_TIME_DLY, &err); // 延时10s
    }
}
