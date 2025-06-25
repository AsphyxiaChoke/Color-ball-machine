#include "Gameflow.h"
//---------------------------
// 数据保存
//---------------------------
void BackProcess(void)
{
    // WriteEEPROM(eeCurrPayoff, (uint8_t *)&CurrPayoff, sizeof(CurrPayoff));
    if (__24C04_FLAG & S_DIP) // 游戏设置
    {
        __24C04_FLAG &= (~S_DIP);
        WriteEEPROM(eeDipSw, (uint8_t *)&SetDipSw, sizeof(DipSw));
    }
    if (__24C04_FLAG & S_ACCOUTPASS) // 查账密码
    {
        __24C04_FLAG &= (~S_ACCOUTPASS);
        WriteEEPROM(eeAccoutPassWord, (uint8_t *)&SetAccoutPassWord, sizeof(AccoutPassWord));
    }
    if (__24C04_FLAG & S_CONFIGPASS) // 参数密码
    {
        __24C04_FLAG &= (~S_CONFIGPASS);
        WriteEEPROM(eeConfigPassWord, (uint8_t *)&SetConfigPassWord, sizeof(ConfigPassWord));
    }
    if (__24C04_FLAG & S_SYSTEMPASS) // 系统密码
    {
        __24C04_FLAG &= (~S_SYSTEMPASS);
        WriteEEPROM(eeSystemPassWord, (uint8_t *)&SetSystemPassWord, sizeof(SystemPassWord));
    }
    if (__24C04_FLAG & S_FUTORYTIME) // 出厂时间
    {
        __24C04_FLAG &= (~S_FUTORYTIME);
        WriteEEPROM(eeFutoryTime, (uint8_t *)SetFutoryTime, sizeof(FutoryTime));
    }
    if (__24C04_FLAG & S_PROGRAMVEL) // 游戏版本
    {
        __24C04_FLAG &= (~S_PROGRAMVEL);
        WriteEEPROM(eeProgramVel, (uint8_t *)SetProgramVel, sizeof(ProgramVel));
    }
    if (__24C04_FLAG & S_MACHINELINE) // 线号
    {
        __24C04_FLAG &= (~S_MACHINELINE);
        WriteEEPROM(eeMachine_line, (uint8_t *)&Machine_line, sizeof(Machine_line));
    }
    if (__24C04_FLAG & S_MACHINEID) // 台号
    {
        __24C04_FLAG &= (~S_MACHINEID);
        WriteEEPROM(eeMachine_id, (uint8_t *)&Machine_id, sizeof(Machine_id));
    }
    if (__24C04_FLAG & S_QIUWEI) // 球位
    {
        __24C04_FLAG &= (~S_QIUWEI);
        WriteEEPROM(eeQiuwei, Qiuwei, sizeof(Qiuwei));
    }
    if (__24C04_FLAG & S_GAMERECORD) // 游戏记录保存
    {
        __24C04_FLAG &= (~S_GAMERECORD);
        WriteEEPROM(eeGameRecord, (uint8_t *)&GameRecord, sizeof(GameRecord));
    }
    if(__24C04_FLAG & S_LUCKJIANG) // 奖池和幸运奖记录保存
    {
        __24C04_FLAG &= (~S_LUCKJIANG);
        WriteEEPROM(eeLuckJiang, (uint8_t *)&LuckJiang, sizeof(LuckJiang));
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
#ifdef __USE_JIXIN__
    if (SwtQiuFlag)
    {
        OldProcMem = ProcMem;
        ProcMem = 7;
        return;
    }
#endif
    for (i = 0; i < MAX_FENJI; i++)
    {
        ReturnBet(&Child[i]);
        Child[i].Flag &= ~(QUICK_WIN | QUICK_BONUS | COMPUTER | WINLAMP_FLAG);
    }
    memset(DingFen, 0, sizeof(DingFen));

    MinBonus = 100000; // 测试用
    MidBonus = 200000;
    MaxBonus = 500000;
    SetGameLuckBuf(LuckBuf);

    SetMastercmd(MasterCmdpoint, PC_RESTART);
    SetChildcmd(ChildCmdpoint, PC_RESTART);
    TotalGame++;
    if (TotalGame >= TOTAL_LUNSHU)
    {
        TotalGame = 0;
    }
    GameCount = 0;
    Luck = Luck_CS = 0;
    MingPai = 0;
    GamePai = 0;
    GameTime = 0;
    {
        memset(History, 0, sizeof(History));
        memset(LuckHistory, 0, sizeof(LuckHistory));
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
    SetMastercmd(MasterCmdpoint, PC_LUCKHISTORY);
    SetChildcmd(ChildCmdpoint, PC_LUCKHISTORY);
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
#ifdef __USE_JIXIN__
    if (SwtQiuFlag)
    {
        OldProcMem = ProcMem;
        ProcMem = 7;
        return;
    }
#endif
    for (i = 0; i < MAX_FENJI; i++)
    {
        ReturnBet(&Child[i]);
        Child[i].Flag &= ~(QUICK_WIN | QUICK_BONUS | COMPUTER | COMPUTER_WIN2 | WINLAMP_FLAG);
        ChildKeyTUI(&Child[i]);
        ChildKeyREP(&Child[i]);
        ChildKeyBET0(&Child[i]);
        ChildKeyBET1(&Child[i]);
        ChildKeyBET2(&Child[i]);
        ChildKeyBET3(&Child[i]);
        ChildKeyBET4(&Child[i]);
    }

    memset(DingFen, 0, sizeof(DingFen));
    ReadEEPROM(eeDipSw, (uint8_t *)&DipSw, sizeof(DipSw));
    GameTime = GetGameTime(&DipSw);
    GamePai = GetGameResult(PlayPaiBuf, GameCount);

    ChildGameCount = GameCount;
    SetMastercmd(MasterCmdpoint, PC_GAMESTART);
    SetChildcmd(ChildCmdpoint, PC_GAMESTART);

#ifdef __USE_JIXIN__
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
            LineBonus = __random() % 100000;
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

#ifdef __USE_JIXIN__
    MachineOpenQiu();
#else
#warning "添加开球"
#endif

    SetMastercmd(MasterCmdpoint, PC_RESULT);
    SetChildcmd(ChildCmdpoint, PC_RESULT);
    // for (time = OSTimeGet(&err) + OS_TICKS_PER_SEC * 9; time > OSTimeGet(&err);)
    for (time = OSTimeGet(&err) + OS_TICKS_PER_SEC * 1; time > OSTimeGet(&err);)
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
    uint8_t pai, result, flash, i, luck_cesi;
    uint32_t win_wait, time, addtime, prev_flash_time;
    ChildType *pChild;

    uint32_t all_win, all_bet;
    // 第一次中奖，开奖结果
    pai = GetGameResult(PlayPaiBuf, GameCount);
    result = ((pai & 0x0f) >= 0x0e) ? 4 : (pai & 0x30) >> 4;
    SetMastercmd(MasterCmdpoint, PC_STARTWIN);
    SetChildcmd(ChildCmdpoint, PC_STARTWIN);
    for (i = 0; i < MAX_FENJI; i++)
    { // 计算分机中奖账目
        pChild = &Child[i];
        if (!(pChild->Flag & COMPUTER))
        {
            all_win = (pChild->Bet[result] * (uint32_t)g_NewMul[0][result] + 99) / 100;
            all_bet = pChild->Bet[0] + pChild->Bet[1] + pChild->Bet[2] + pChild->Bet[3] + pChild->Bet[4];
            if (result == 4)
                all_win += pChild->Bet[0] + pChild->Bet[1] + pChild->Bet[2] + pChild->Bet[3];

            pChild->OldBet[0] = pChild->Bet[0];
            pChild->OldBet[1] = pChild->Bet[1];
            pChild->OldBet[2] = pChild->Bet[2];
            pChild->OldBet[3] = pChild->Bet[3];
            pChild->OldBet[4] = pChild->Bet[4];
            pChild->Win = all_win;
            pChild->AllBet8 += (uint64_t)all_bet;
            pChild->AllWin8 += (uint64_t)all_win;

            pChild->JiFenBet += all_bet; // 有退币，玩家积分清零。
            pChild->JiFenWin += all_win; // 有退币，玩家积分清零。

            pChild->Flag |= COMPUTER; // 这个分机账目计算完成。
            if (all_win)
                pChild->Flag |= WINLAMP_FLAG;
        }
    }
    time = prev_flash_time = OSTimeGet(&err);
    for (win_wait = OSTimeGet(&err) + 5 * OS_TICKS_PER_SEC, flash = 0; win_wait > time;)
    { // 等待玩家查看自己的中奖得分。10秒时间
        time = OSTimeGet(&err);
        BackProcess();
        CoinKeyProc();
        if (time - prev_flash_time >= OS_TICKS_PER_SEC / 2)
        {
            prev_flash_time += OS_TICKS_PER_SEC / 2;
            flash ^= 1;
            for (i = 0; i < MAX_FENJI; i++)
            {
                FlashBet(result, flash);
                FlashWin(flash);
                FlashBetLED(&Child[i], flash);
                FlashWinLamp(&Child[i], flash);
            }
        }
    }
    // 计算二次中奖
    Luck = 0;
    LuckJiang.Luck_AllWin2 = 0;
    Luck_CS = GetGameLuck(LuckBuf, GameCount);
    if (!MingPai && (Luck_CS & 0x0f)) // 如果本局没有明牌，看看要不中其他特殊奖。
    {
        LuckJiang.Luck_AllWin2 = GetGameLuckWin2(Luck_CS);
        if ((Luck_CS & 0x0f)) // 有特殊奖，真的可以中特殊奖啦。???????????????
        {
            if (Luck_CS == LUCK_MINGPAI)
            {
                MingPai = 1; // 计算出可以出明牌，但是本局不能直接显示。
                Luck = 0;
                for (i = 0; i < MAX_FENJI; i++)
                { // 计算分机二次中奖账目
                    pChild = &Child[i];
                    if (!(pChild->Flag & COMPUTER_WIN2))
                    {
                        pChild->Win2 = 0;
                        pChild->Flag |= COMPUTER_WIN2; // 这个分机账目计算完成。
                    }
                }
                LuckJiang.JiangJinChi_Out += LuckJiang.Luck_AllWin2;
                LuckJiang.JiangJinWin[LUCK_MINGPAI] += LuckJiang.Luck_AllWin2;
                LuckJiang.JiangJinCnt[LUCK_MINGPAI]++;
            }
            else
            {
                for (i = 0; i < MAX_FENJI; i++)
                { // 计算分机二次中奖账目
                    pChild = &Child[i];
                    if (!(pChild->Flag & COMPUTER_WIN2))
                    {
                        pChild->Win += pChild->Win2;
                        pChild->AllWin8 += (uint64_t)pChild->Win2;
                        pChild->JiFenWin += pChild->Win2; // 有退币，玩家积分清零。
                        pChild->Flag |= COMPUTER_WIN2;    // 这个分机账目计算完成。
                    }
                }
                if (Luck_CS == LUCK_JIFEN)
                {
                    pChild = &Child[Luck_CS >> 4];
                    pChild->JiFen = 0;
                    pChild->JiFenBet = 0;
                    pChild->JiFenWin = 0;
                }
                LuckJiang.JiangJinChi_Out += LuckJiang.Luck_AllWin2;
                LuckJiang.JiangJinWin[Luck_CS & 0x0f] += LuckJiang.Luck_AllWin2;
                LuckJiang.JiangJinCnt[Luck_CS & 0x0f]++;
                // 播放特殊奖的特效，
                Luck = Luck_CS;
                SetMastercmd(MasterCmdpoint, PC_STARTBONUS);
                SetChildcmd(ChildCmdpoint, PC_STARTBONUS);
                time = prev_flash_time = OSTimeGet(&err);
                for (win_wait = OSTimeGet(&err) + 10 * OS_TICKS_PER_SEC, flash = 0; win_wait > time;)
                { // 等待玩家查看自己的中奖得分。10秒时间
                    time = OSTimeGet(&err);
                    BackProcess();
                    CoinKeyProc();
                    if (time - prev_flash_time >= OS_TICKS_PER_SEC / 2)
                    {
                        prev_flash_time += OS_TICKS_PER_SEC / 2;
                        flash ^= 1;
                        for (i = 0; i < MAX_FENJI; i++)
                        {
                            FlashBet(result, flash);
                            FlashWin(flash);
                            FlashBetLED(&Child[i], flash);
                            FlashWinLamp(&Child[i], flash);
                        }
                    }
                }
                // 停止播放特殊奖的特效，
                SetMastercmd(MasterCmdpoint, PC_STOPBONUS);
                SetChildcmd(ChildCmdpoint, PC_STOPBONUS);
            }
        }
        else // 有特殊奖，但是还不能出。
        {
            Luck = 0;
            for (i = 0; i < MAX_FENJI; i++)
            { // 计算分机二次中奖账目
                pChild = &Child[i];
                if (!(pChild->Flag & COMPUTER_WIN2))
                {
                    pChild->Win2 = 0;
                    pChild->Flag |= COMPUTER_WIN2; // 这个分机账目计算完成。
                }
            }
        }
    }
    for (i = 0; i < MAX_FENJI; i++)
    { // 清除按键，允许按键启动快速过分。
        pChild = &Child[i];
        ChildKeyTUI(pChild);
        ChildKeyREP(pChild);
        ChildKeyBET0(pChild);
        ChildKeyBET1(pChild);
        ChildKeyBET2(pChild);
        ChildKeyBET3(pChild);
        ChildKeyBET4(pChild);
        flash = 1; // 清除闪烁
        FlashBet(result, flash);
        FlashWin(flash);
        FlashBetLED(&Child[i], flash);
        FlashWinLamp(&Child[i], flash);
    }
    for (win_wait = OSTimeGet(&err) + 5 * OS_TICKS_PER_SEC; (time = OSTimeGet(&err)) < win_wait;)
    { // 慢速过分，5秒时间。
        if (addtime <= time)
        {
            for (i = 0; i < MAX_FENJI; i++)
            {
                uint32_t chip;
                pChild = &Child[i];
                int32_t win = (int32_t)(pChild->Win);
                if (win <= 0)
                {
                    continue;
                }
                chip = ((pChild->Flag & QUICK_WIN) == 0) ? ((win <= 10) ? win : 10) : GetBecomeChip(win);
                pChild->Win = win - chip;
                pChild->Credit += chip;
                if (ChildKeyBET0(pChild) || ChildKeyBET1(pChild) || ChildKeyBET2(pChild) || ChildKeyBET3(pChild) || ChildKeyBET4(pChild))
                {
                    pChild->Flag |= QUICK_WIN;
                }
            }
            addtime = time + 1; // 5*20ms=0.1秒
        }
    }
    for (i = 0; i < MAX_FENJI; i++)
    { // 5秒后，账目没有过完的，直接过完。
        pChild = &Child[i];
        if (pChild->Win > 0)
        {
            pChild->Credit += pChild->Win;
            pChild->Win = 0;
        }
        pChild->Win2 = 0;
        if (pChild->JiFenBet > pChild->JiFenWin)
        {
            pChild->JiFen = (pChild->JiFenBet - pChild->JiFenWin) / 10; // 目前是积分是输分的10%
        }
        else
        {
            pChild->JiFen = 0;
        }

        ClearBet(&Child[i]); // 清除押分。
    }
    MinBonus += __random() % 10; // 测试用
    MidBonus += __random() % 20;
    MaxBonus += __random() % 30;
    // 中奖结束，更新历史记录。
    SetMastercmd(MasterCmdpoint, PC_STOPWIN);
    SetChildcmd(ChildCmdpoint, PC_STOPWIN);

    ProcMem++;
}
//------------------------
// 游戏结束1
//------------------------
void GameEnd(void)
{
    uint8_t pai;
#ifdef __USE_JIXIN__
    MachineCloseQiu();
#else
#warning "收球操作"
#endif

    pai = ((GamePai & 0x0f) >= 0x0e) ? 4 : (GamePai & 0x30) >> 4;
    switch (pai)
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
    case 0x03:
        FangCnt++;
        break;
    case 0x04:
        WangCnt++;
        break;
    default:
        break;
    }
    History[GameCount] = GamePai;
    if (MingPai == 1)
    {
        LuckHistory[GameCount] = 0;
    }
    else if (MingPai == 2)
    {
        LuckHistory[GameCount] = (pai << 4) + LUCK_MINGPAI;
        MingPai = 0;
    }
    else
    {
        if (Luck)
        {
            LuckHistory[GameCount] = Luck;
        }
        else
        {
            LuckHistory[GameCount] = 0;
        }
    }

    SetMastercmd(MasterCmdpoint, PC_HISTORY);
    SetChildcmd(ChildCmdpoint, PC_HISTORY);
    SetMastercmd(MasterCmdpoint, PC_LUCKHISTORY);
    SetChildcmd(ChildCmdpoint, PC_LUCKHISTORY);
    GamePai = 0;
    Luck = 0;

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
        GameCount = TOTAL_JUSHU;
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
    // ProcMem = 0;
    ProcMem = OldProcMem;
}
