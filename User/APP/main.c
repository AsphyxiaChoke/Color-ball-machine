/*
 ******************************************************************************
 * @file    main.c
 * @author  fire
 * @version V1.0
 * @date    2016-xx-xx
 * @brief   uCOS-III ϵͳ��ֲ
 ******************************************************************************
 */
/*
*********************************************************************************************************
*                                            �������ļ�
*********************************************************************************************************
*/
#include "main.h"

/*
*********************************************************************************************************
*                                         ������ƿ�TCB
*********************************************************************************************************
*/

static OS_TCB AppTaskWanTCB;
static OS_TCB AppTaskDrawTCB;
static OS_TCB AppTaskKeyTCB;
static OS_TCB AppTaskMainTCB;
/*
*********************************************************************************************************
*                                            �����ջ
*********************************************************************************************************
*/
static CPU_STK AppTaskWanStk[APP_TASK_WAN_STK_SIZE];
static CPU_STK AppTaskDrawStk[APP_TASK_DRAW_STK_SIZE];
static CPU_STK AppTaskKeyStk[APP_TASK_KEY_STK_SIZE];
static CPU_STK AppTaskManiStk[APP_TASK_MAIN_STK_SIZE];
/*
*********************************************************************************************************
*                                            ����ԭ��
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
// ��ʼ����Ϸ�ֻ�����
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
        *((uint8_t *)(FPGAADDR + i)) = 0x00; // �����ʾ������
    for (i = 0; i < MAX_FENJI; i++)
    {
        Child[i].Count = 0; // �رռ�����
        Child[i].CoinKeyCount = Child[i].pChildInput->KeyCount;
        Child[i].CoinInTmp = Child[i].CoinInTmp2 = 0;
        Child[i].CoinOutTmp = Child[i].CoinOutTmp2 = 0;
        Child[i].InitCoinOut = 0;
        Child[i].AgainInitCoinOut = 0;
    }
}
//-----------------------------------------------------------------------------
// ���Ե�ر�������
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
// ��ʼ����Ϸ����
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
* ������ : main
*********************************************************************************************************
*/
int main(void)
{
    OS_ERR err;
    MPU_Config();
    HAL_Init();                   // HAL��ʼ��,��ע�� 1
    BSP_SystemClkCfg();           // ��ʼ��CPUƵ��Ϊ 400Mhz
    CPU_Init();                   // ��ʼ�� CPU �����ʱ��������ж�ʱ���������������
    Mem_Init();                   // ��ʼ���ڴ������������ڴ�غ��ڴ�ر�
    MX_GPIO_Init();               // ��ʼ��GPIOʱ��
    MX_FMC_Init();                // ��ʼ��FMC�ӿ�
    MX_RTC_Init();                // RTC��ʼ��
    BKP_SRAM_Init();              // ��ʼ��������
    MX_RNG_Init();                // �������ʼ��
    MX_USART1_UART_Init();        // ������ʾ����
    MX_USART2_UART_Init();        // Һ������������
    MX_USART3_UART_Init();        // ���Ŵ���
    MX_UART4_Init();              // ���ڴ���
    MX_UART5_Init();              // ��о����
    MX_USART6_UART_Init();        //
    MX_SPI1_Init();               // ��ʼ��SPI�ӿ�
    ClearZero(CheckEEPROM());     //
    Init_machine();               // ��ʼ���ֻ��ӿ�
    MX_TIM2_Init();               // ��ʼ���˱�ʱ���ж�
    HAL_TIM_Base_MspInit(&htim2); // �����ж�
    CPU_IntDis();                 // ��ֹ�����ж�
    OSInit(&err);                 // ��ʼ��uC/OS-IIIϵͳ
    /* ������ʼ���� */
    OSTaskCreate((OS_TCB *)&AppTaskDrawTCB,                           // ������ƿ��ַ
                 (CPU_CHAR *)"App Task Draw",                         // ��������
                 (OS_TASK_PTR)AppDraw,                                // ������
                 (void *)0,                                           // ���ݸ����������β�p_arg����ʵ��
                 (OS_PRIO)APP_TASK_DRAW_PRIO,                         // ��������ȼ�
                 (CPU_STK *)&AppTaskDrawStk[0],                       // �����ջ�Ļ���ַ
                 (CPU_STK_SIZE)APP_TASK_DRAW_STK_SIZE / 10,           // �����ջ�ռ�ʣ��1/10ʱ����������
                 (CPU_STK_SIZE)APP_TASK_DRAW_STK_SIZE,                // �����ջ�ռ䣨��λ��sizeof(CPU_STK)��
                 (OS_MSG_QTY)5u,                                      // ����ɽ��յ������Ϣ��
                 (OS_TICK)0u,                                         // �����ʱ��Ƭ��������0��Ĭ��ֵOSCfg_TickRate_Hz/10��
                 (void *)0,                                           // ������չ��0����չ��
                 (OS_OPT)(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), // ����ѡ��
                 (OS_ERR *)&err);                                     // ���ش�������

    OSStart(&err); // �����������������uC/OS-III���ƣ�
}

/**********************************************************************************************************
 * ������ AppDraw
 **********************************************************************************************************/

static void AppDraw(void *p_arg)
{
    OS_ERR err;
    HANDTYPE HandCtr;
    struct GameSetRx Gameset;
    uint8_t led_flash = 0;
    uint8_t led_flash_time = 0;
    (void)p_arg;
    BSP_Init();                           // �弶��ʼ��
    OSTimeDly(10, OS_OPT_TIME_DLY, &err); // ��ʱ1s
    /* ������������ */
    OSTaskCreate((OS_TCB *)&AppTaskKeyTCB,                            // ������ƿ��ַ
                 (CPU_CHAR *)"App Task key",                          // ��������
                 (OS_TASK_PTR)AppKey,                                 // ������
                 (void *)0,                                           // ���ݸ����������β�p_arg����ʵ��
                 (OS_PRIO)APP_TASK_KEY_PRIO,                          // ��������ȼ�
                 (CPU_STK *)&AppTaskKeyStk[0],                        // �����ջ�Ļ���ַ
                 (CPU_STK_SIZE)APP_TASK_KEY_STK_SIZE / 10,            // �����ջ�ռ�ʣ��1/10ʱ����������
                 (CPU_STK_SIZE)APP_TASK_KEY_STK_SIZE,                 // �����ջ�ռ䣨��λ��sizeof(CPU_STK)��
                 (OS_MSG_QTY)5u,                                      // ����ɽ��յ������Ϣ��
                 (OS_TICK)0u,                                         // �����ʱ��Ƭ��������0��Ĭ��ֵOSCfg_TickRate_Hz/10��
                 (void *)0,                                           // ������չ��0����չ��
                 (OS_OPT)(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), // ����ѡ��
                 (OS_ERR *)&err);                                     // ���ش�������

    /* ������Ϸ������ */
    OSTaskCreate((OS_TCB *)&AppTaskMainTCB,                           // ������ƿ��ַ
                 (CPU_CHAR *)"App Task Main",                         // ��������
                 (OS_TASK_PTR)AppMain,                                // ������
                 (void *)0,                                           // ���ݸ����������β�p_arg����ʵ��
                 (OS_PRIO)APP_TASK_MAIN_PRIO,                         // ��������ȼ�
                 (CPU_STK *)&AppTaskManiStk[0],                       // �����ջ�Ļ���ַ
                 (CPU_STK_SIZE)APP_TASK_MAIN_STK_SIZE / 10,           // �����ջ�ռ�ʣ��1/10ʱ����������
                 (CPU_STK_SIZE)APP_TASK_MAIN_STK_SIZE,                // �����ջ�ռ䣨��λ��sizeof(CPU_STK)��
                 (OS_MSG_QTY)5u,                                      // ����ɽ��յ������Ϣ��
                 (OS_TICK)0u,                                         // �����ʱ��Ƭ��������0��Ĭ��ֵOSCfg_TickRate_Hz/10��
                 (void *)0,                                           // ������չ��0����չ��
                 (OS_OPT)(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), // ����ѡ��
                 (OS_ERR *)&err);                                     // ���ش�������

    // /* ����Led3���� */
    // OSTaskCreate((OS_TCB *)&AppTaskWanTCB,                           // ������ƿ��ַ
    //              (CPU_CHAR *)"App Task Wan",                         // ��������
    //              (OS_TASK_PTR)AppWan,                            // ������
    //              (void *)0,                                           // ���ݸ����������β�p_arg����ʵ��
    //              (OS_PRIO)APP_TASK_WAN_PRIO,                         // ��������ȼ�
    //              (CPU_STK *)&AppTaskWanStk[0],                       // �����ջ�Ļ���ַ
    //              (CPU_STK_SIZE)APP_TASK_WAN_STK_SIZE / 10,           // �����ջ�ռ�ʣ��1/10ʱ����������
    //              (CPU_STK_SIZE)APP_TASK_WAN_STK_SIZE,                // �����ջ�ռ䣨��λ��sizeof(CPU_STK)��
    //              (OS_MSG_QTY)5u,                                      // ����ɽ��յ������Ϣ��
    //              (OS_TICK)0u,                                         // �����ʱ��Ƭ��������0��Ĭ��ֵOSCfg_TickRate_Hz/10��
    //              (void *)0,                                           // ������չ��0����չ��
    //              (OS_OPT)(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), // ����ѡ��
    //              (OS_ERR *)&err);                                     // ���ش�������
    // LED1_OFF; // �ر�RGB LED
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
    OSTimeDly(OS_TICKS_PER_SEC, OS_OPT_TIME_DLY, &err); // ��ʱ1s
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
        OSTimeDly(1, OS_OPT_TIME_DLY, &err); // ��ʱ1s
    }
}
/*
*********************************************************************************************************
*��������
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
    (void)p_arg; // û���õ��βΣ���ֹ����������
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
    while (DEF_TRUE) // �����壬ͨ����д��һ����ѭ��
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
        //OSTimeDly(2, OS_OPT_TIME_DLY, &err); // ��ʱ1s
        OSTimeDly(1, OS_OPT_TIME_DLY, &err); // ��ʱ1s
    }
}
/**********************************************************************************************************
 *AppMain ����
 **********************************************************************************************************/
static void AppMain(void *p_arg)
{
    OS_ERR err;
    (void)p_arg; // û���õ��βΣ���ֹ����������
    Scene();

    // ��λת���
    #ifdef  __USE_JIXIN__
    Init_JiXinUartSet();
    MachineReSet(); 
    #else
    #warning "û��ʹ�ö����о"
    #endif
    
    // ��Ϸ��ʼ��
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
            OSTimeDly(1, OS_OPT_TIME_DLY, &err); // ��ʱ
        }
    }
}
/**********************************************************************************************************
 *AppWan ����
 **********************************************************************************************************/

static void AppWan(void *p_arg)
{
    OS_ERR err;

    (void)p_arg; // û���õ��βΣ���ֹ����������

    //    LED_RGBOFF;                                      //�ر�RGB LED

    while (DEF_TRUE) // �����壬ͨ����д��һ����ѭ��
    {
        //        LED3_TOGGLE;                                 //LED3��˸
        // printf("LED3_TOGGLE\r\n");
        OSTimeDly(10000, OS_OPT_TIME_DLY, &err); // ��ʱ10s
    }
}
