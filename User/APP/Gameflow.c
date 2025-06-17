#include "Gameflow.h"
//---------------------------
// 数据保存
//---------------------------
void BackProcess(void)
{
    // WriteEEPROM(eeCurrPayoff, (uint8_t *)&CurrPayoff, sizeof(CurrPayoff));
    if(__24C04_FLAG & S_DIP)// 游戏设置
    {
        __24C04_FLAG &= (~S_DIP);
        WriteEEPROM(eeDipSw, (uint8_t *)&SetDipSw, sizeof(DipSw));
    }
    if(__24C04_FLAG & S_ACCOUTPASS)// 查账密码
    {
        __24C04_FLAG &= (~S_ACCOUTPASS);
        WriteEEPROM(eeAccoutPassWord, (uint8_t *)&SetAccoutPassWord, sizeof(AccoutPassWord));  
    }
    if(__24C04_FLAG & S_CONFIGPASS)// 参数密码
    {
        __24C04_FLAG &= (~S_CONFIGPASS);
        WriteEEPROM(eeConfigPassWord, (uint8_t *)&SetConfigPassWord, sizeof(ConfigPassWord));
    }
    if(__24C04_FLAG & S_SYSTEMPASS)// 系统密码
    {
        __24C04_FLAG &= (~S_SYSTEMPASS);
        WriteEEPROM(eeSystemPassWord, (uint8_t *)&SetSystemPassWord, sizeof(SystemPassWord));
    }
    if(__24C04_FLAG & S_FUTORYTIME)// 出厂时间
    {
        __24C04_FLAG &= (~S_FUTORYTIME);
        WriteEEPROM(eeFutoryTime, (uint8_t *)SetFutoryTime, sizeof(FutoryTime));
    }
    if(__24C04_FLAG & S_PROGRAMVEL)// 游戏版本
    {
        __24C04_FLAG &= (~S_PROGRAMVEL);
        WriteEEPROM(eeProgramVel, (uint8_t *)SetProgramVel, sizeof(ProgramVel));
    }
    if(__24C04_FLAG & S_MACHINELINE)// 线号
    {
        __24C04_FLAG &= (~S_MACHINELINE);
        WriteEEPROM(eeMachine_line, (uint8_t *)&Machine_line, sizeof(Machine_line));
    }
    if(__24C04_FLAG & S_MACHINEID)// 台号
    {
        __24C04_FLAG &= (~S_MACHINEID);
        WriteEEPROM(eeMachine_id, (uint8_t *)&Machine_id, sizeof(Machine_id));
    }
    if(__24C04_FLAG & S_QIUWEI) // 球位
    {
        __24C04_FLAG &= (~S_QIUWEI);
        WriteEEPROM(eeQiuwei, Qiuwei, sizeof(Qiuwei));
    }
    
}
//------------------------------------
// 投退，上下分处理
//------------------------------------
uint32_t CoinKeyIndex = 0;
void CoinKeyProc(void)
{
    if (CoinKeyIndex >= MAX_FENJI)
        CoinKeyIndex = 0;
    Buzzer(&Child[CoinKeyIndex]);
    CoinKeyProc1(&Child[CoinKeyIndex++]);
}
//------------------------------------
// 押分处理
//-------------------------------------
uint32_t BetKeyIndex = 0;
void BetKeyProc(void)
{
    if (GameTime > 0)
    {
        if (BetKeyIndex >= MAX_FENJI)
        {
            BetKeyIndex = 0;
        }
        BetKeyProc1(&Child[BetKeyIndex]);
        DispBetLED(&Child[BetKeyIndex], Child[BetKeyIndex].Credit);
        BetKeyIndex++;
    }
}
__weak void CheckBill(void)
{
    ;
}
//----------------------------
// 游戏重新开始,新的一轮游戏开始
//----------------------------
void ReStart(void)
{
    uint32_t i;
    #ifdef  __USE_JIXIN__
    if(SwtQiuFlag)
    {
        OldProcMem=ProcMem;
        ProcMem=7;
        return;
    }
    #endif
    for (i = 0; i < MAX_FENJI; i++)
    {
        ReturnBet(&Child[i]);
        Child[i].Flag &= ~(QUICK_WIN | QUICK_BONUS | COMPUTER | WINLAMP_FLAG);
    }
    memset(DingFen, 0, sizeof(DingFen));
    SetMastercmd(MasterCmdpoint, PC_RESTART);
    SetChildcmd(ChildCmdpoint, PC_RESTART);
    TotalGame++;
    if (TotalGame >= TOTAL_LUNSHU)
    {
        TotalGame = 0;
    }
    GameCount = 0;

    {
        memset(History, 0, sizeof(History));
        memset(LuckHistory,0,sizeof(LuckHistory));
        HeiCnt = 0;
        HongCnt = 0;
        CaoCnt = 0;
        FangCnt = 0;
        WangCnt = 0;
    }
    ChildTotalGame = TotalGame;
    ChildGameCount = GameCount;
    SetMastercmd(MasterCmdpoint, PC_HISTORY);
    SetChildcmd(ChildCmdpoint, PC_HISTORY);
    SetMastercmd(MasterCmdpoint,PC_LUCKHISTORY);
    SetChildcmd(ChildCmdpoint,PC_LUCKHISTORY);
    ProcMem++;
}
//---------------------------
// 每局游戏开始
//---------------------------
void Start(void)
{
    OS_ERR err;
    uint8_t i;
    uint32_t currtime;
    #ifdef  __USE_JIXIN__
    if(SwtQiuFlag)
    {
        OldProcMem=ProcMem;
        ProcMem=7;
        return;
    }
    #endif
    for (i = 0; i < MAX_FENJI; i++)
    {
        ReturnBet(&Child[i]);
        Child[i].Flag &= ~(QUICK_WIN | QUICK_BONUS | COMPUTER | WINLAMP_FLAG);
    }
    memset(DingFen, 0, sizeof(DingFen));
    ReadEEPROM(eeDipSw, (uint8_t *)&DipSw, sizeof(DipSw));
    GameTime = GetGameTime(&DipSw);
    GamePai = GetGameResult(PlayPaiBuf, GameCount);
    //Luck = 0;
    Luck = GetGameLuck();
    ChildGameCount = GameCount;
    SetMastercmd(MasterCmdpoint, PC_GAMESTART);
    SetChildcmd(ChildCmdpoint, PC_GAMESTART);

    #ifdef  __USE_JIXIN__
    MachineOutQiu();
    #else
    #warning "添加洗球并出球"
    #endif

    for (currtime = OSTimeGet(&err) + 20; currtime > OSTimeGet(&err);)
    {
        CoinKeyProc();
        BackProcess();
    }
    ProcMem++;
}
//--------------------------
// 押分等待
//--------------------------
void WaitBET(void)
{
    OS_ERR err;
    uint32_t time, sec_time, bonus_time;
    SetMastercmd(MasterCmdpoint, PC_STARTBET);
    SetChildcmd(ChildCmdpoint, PC_STARTBET);
    for (time = sec_time = bonus_time = OSTimeGet(&err); GameTime; time = OSTimeGet(&err))
    {
        BackProcess();
        CoinKeyProc();
        BetKeyProc();
        if ((time - sec_time) >= OS_TICKS_PER_SEC)
        {
            sec_time += OS_TICKS_PER_SEC;
            SetGameTime((GameTime > 0) ? GameTime - 1 : 0);
        }
        if (time - bonus_time >= OS_TICKS_PER_SEC / 2)
        {
            bonus_time = time;
            LineBonus = __random()%100000;
        }
    }
    SetMastercmd(MasterCmdpoint, PC_STOPBET);
    SetChildcmd(ChildCmdpoint, PC_STOPBET);
    ProcMem++;
}
//-------------------------
// 开奖
//-------------------------
void StartRun(void)
{
    OS_ERR err;
    uint32_t time;
     
    #ifdef  __USE_JIXIN__
    MachineOpenQiu();
    #else
    #warning "添加开球"
    #endif

    SetMastercmd(MasterCmdpoint, PC_RESULT);
    SetChildcmd(ChildCmdpoint, PC_RESULT);
    for (time = OSTimeGet(&err) + OS_TICKS_PER_SEC * 9; time > OSTimeGet(&err);)
    {
        BackProcess();
        CoinKeyProc();
    }
    ProcMem++;
}
//------------------------
// 中奖
//------------------------
int32_t GetBecomeChip(int32_t n)
{
    if (n >= 100)
        return 100;
    if (n >= 10)
        return 10;
    if (n >= 1)
        return 1;
    return 0;
}
void WinProc(void)
{
    OS_ERR err;
    uint8_t pai, flash, i;
    uint32_t win_wait, time, prev_flash_time;
    ChildType *pChild;
    //第一次中奖，开奖结果
    SetMastercmd(MasterCmdpoint, PC_STARTWIN);
    SetChildcmd(ChildCmdpoint, PC_STARTWIN);
    for (i = 0; i < MAX_FENJI; i++)
    {
        uint32_t all_win = 0;
        pChild = &Child[i];
        if (pChild->Flag & COMPUTER)
        {
            all_win = pChild->ResultWin;
            pChild->Win = all_win;
            pChild->ResultWin = 0;
            if (all_win)
                pChild->Flag |= WINLAMP_FLAG;
        }
    }
    prev_flash_time = time = OSTimeGet(&err);
    flash = 0;
    {
        for (i = 0; i < MAX_FENJI; i++)
        {
            pChild = &Child[i];
            ChildKeyTUI(pChild);
            ChildKeyREP(pChild);
            ChildKeyBET0(pChild);
            ChildKeyBET1(pChild);
            ChildKeyBET2(pChild);
            ChildKeyBET3(pChild);
            ChildKeyBET4(pChild);
        }
    }
    //第二次中奖，中特殊奖
    History[GameCount] = GamePai;
    LuckHistory[GameCount] = Luck;
    pai = GamePai & 0x3f;
    if ((pai & 0x0F) >= 0x0E)
    {
        WangCnt++;
    }
    else
    {
        switch ((pai >> 4) & 0x03)
        {
        case 0x00:
            HeiCnt++;
            break;
        case 0x01:
            HongCnt++;
            break;
        case 0x02:
            CaoCnt++;
            break;
        default:
            FangCnt++;
            break;
        }
    }
    SetMastercmd(MasterCmdpoint, PC_HISTORY);
    SetChildcmd(ChildCmdpoint, PC_HISTORY);
    SetMastercmd(MasterCmdpoint, PC_LUCKHISTORY);
    SetChildcmd(ChildCmdpoint, PC_LUCKHISTORY);
		SetMastercmd(MasterCmdpoint, PC_STARTWIN);
    SetChildcmd(ChildCmdpoint, PC_STARTWIN);
    for (win_wait = OSTimeGet(&err) + 10 * OS_TICKS_PER_SEC, flash = 0; win_wait > time;)
    {
        time = OSTimeGet(&err);
        BackProcess();
        CoinKeyProc();
        if (time - prev_flash_time >= OS_TICKS_PER_SEC / 2)
        {
            prev_flash_time += OS_TICKS_PER_SEC / 2;
            flash ^= 1;
            for (i = 0; i < MAX_FENJI; i++)
            {
                uint8_t result;
                result = ((GamePai & 0x0f) >= 0x0e) ? 4 : (GamePai & 0x30) >> 4;
                FlashBet(result, flash);
                FlashWin(flash);
                FlashBetLED(&Child[i], flash);
                FlashWinLamp(&Child[i], flash);
            }
        }
    }
    { // 正确处理闪烁
        for (; flash != 0;)
        {
            BackProcess();
            CoinKeyProc();
            if ((time = OSTimeGet(&err)) - prev_flash_time >= OS_TICKS_PER_SEC / 2)
            {
                prev_flash_time += OS_TICKS_PER_SEC / 2;
                flash ^= 1;
                for (i = 0; i < MAX_FENJI; i++)
                {
                    uint8_t result;
                    result = ((GamePai & 0x0f) >= 0x0e) ? 4 : (GamePai & 0x30) >> 4;
                    FlashBet(result, flash);
                    FlashWin(flash);
                    FlashBetLED(&Child[i], flash);
                    FlashWinLamp(&Child[i], flash);
                }
            }
        }
    }
    // 慢速得分
    for (i = 0; i < MAX_FENJI; i++)
    {
        ClearBet(&Child[i]);
    }
    //SetMastercmd(MasterCmdpoint, PC_STARTWIN);
    //SetChildcmd(ChildCmdpoint, PC_STARTWIN);
    {
        uint32_t addtime;
        time = OSTimeGet(&err);
        addtime = time;
        for (win_wait = OSTimeGet(&err) + 5 * OS_TICKS_PER_SEC; (time = OSTimeGet(&err)) < win_wait;)
        {
            if (addtime <= time)
            {
                for (i = 0; i < MAX_FENJI; i++)
                {
                    uint32_t chip;
                    int32_t win = (int32_t)((pChild = &Child[i])->Win);
                    if (win <= 0)
                        continue;
                    chip = ((pChild->Flag & QUICK_WIN) == 0) ? ((win <= 0) ? 0 : 1) : GetBecomeChip(win);
                    pChild->Win = win - chip;
                    pChild->Credit += chip;
                    if (ChildKeyBET0(pChild) || ChildKeyBET1(pChild) || ChildKeyBET2(pChild) || ChildKeyBET3(pChild) ||
                        ChildKeyBET4(pChild))
                        pChild->Flag |= QUICK_WIN;
                    BuzzerDeal(pChild);
                }
                addtime = time + 2;
            }

            // 处理闪烁
            if (time - prev_flash_time >= OS_TICKS_PER_SEC / 2)
            {
                prev_flash_time += OS_TICKS_PER_SEC / 2;
                flash ^= 1;
                for (i = 0; i < MAX_FENJI; i++)
                {
                    FlashBetLED(&Child[i], flash);
                    FlashWinLamp(&Child[i], flash);
                }
            }
            do
            {
                BackProcess();
                CoinKeyProc();
            } while (OSTimeGet(&err) <= time);
        }
#ifdef __MAIN_DEBUG__
        uartprintf("__STEP5__\n");
#endif
        // 快速得分
        for (win_wait = OSTimeGet(&err) + 1 * OS_TICKS_PER_SEC;;)
        {
            uint32_t flag = 0;
            for (i = 0, time = OSTimeGet(&err); i < MAX_FENJI; i++)
            {
                uint32_t chip;
                int32_t win = (int32_t)((pChild = &Child[i])->Win);
                if (win <= 0)
                    continue;
                flag = 1;
                chip = GetBecomeChip(win);
                pChild->Win = win - chip;
                pChild->Credit += chip;
                DispWIN(pChild);
                DispCREDIT(pChild);
                BuzzerDeal(pChild);
            }
            // 处理闪烁
            if (time - prev_flash_time >= OS_TICKS_PER_SEC / 2)
            {
                prev_flash_time += OS_TICKS_PER_SEC / 2;
                flash ^= 1;
                for (i = 0; i < MAX_FENJI; i++)
                {
                    FlashBetLED(&Child[i], flash);
                    FlashWinLamp(&Child[i], flash);
                }
            }
            do
            {
                BackProcess();
                CoinKeyProc();
            } while (OSTimeGet(&err) <= time);
            if (flag == 0 && win_wait < OSTimeGet(&err))
                break;
        }
    }
    SetMastercmd(MasterCmdpoint, PC_STOPWIN);
    SetChildcmd(ChildCmdpoint, PC_STOPWIN);
    for (i = 0; i < MAX_FENJI; i++)
	{
		FlashBetLED(&Child[i], 1);
		FlashWinLamp(&Child[i], flash);
    }
    ProcMem++;
}
//------------------------
// 游戏结束1
//------------------------
void GameEnd(void)
{
    #ifdef  __USE_JIXIN__
    MachineCloseQiu();
    #else
    #warning "收球操作"
    #endif
    ProcMem++;
}
//--------------------------
// 游戏结束2
//--------------------------
void GameEnd2(void)
{
    SetMastercmd(MasterCmdpoint, PC_GAMEEND);
    SetChildcmd(ChildCmdpoint, PC_GAMEEND);
    GameCount++;
    if (GameCount >= TOTAL_JUSHU)
    {
        GameCount = TOTAL_JUSHU ;
        ProcMem = 0;
    }
    else
    {
        ProcMem = 1;
    }
}
//-----------------------------------------------------------------------------
// 设置球位
//-----------------------------------------------------------------------------
void GameSetBall(void)
{
	uint8_t swtqiuflag = 0;
	uint8_t qtext;
	QiuState = 1;
	while (1)
	{
		BackProcess();
		CoinKeyProc();
		qtext = QiuTest;
		if (qtext == 1)
		{
			MachineSheQiu(ChuQiuHao);
			QiuTest = 0;
		}
		swtqiuflag = SwtQiuFlag;
		if (swtqiuflag == 0)
			break;
	}
	Gamestation &= (~MACHERR07);
	MachineSaveQiu();
	QiuState = 0;
	//ProcMem = 0;
    ProcMem = OldProcMem;
}
