#include "GameOption.h"

void SetGameResult(uint8_t *ptr, uint16_t game_count, uint8_t val)
{
    ptr[game_count] = val;
}

uint8_t GetGameResult(void *buf, uint16_t game_count)
{
    uint8_t *ptr = (uint8_t *)buf;
    uint8_t pai = ptr[game_count];
    if ((pai & 0x0F) == 0x00 || (pai & 0xf0) > 0x30)
    {
        //pai = (__random() & 0x37) + (__random() & 0x03) + 2;
        pai = (__random() & 0x30) + (__random() % 0x0d) + 1;
        if((__random()&0x1f)==0)
        {
            pai = (__random()&1) ? 0x0e : 0x0f;
        }
        pai &= 0x3F;
        SetGameResult(ptr, game_count, pai);
    }
    return pai;
}
void SetGameLuck(uint8_t *ptr, uint16_t game_count, uint8_t val)
{
    ptr[game_count] = val;
}
//设置100局的特殊�?�，�?100�?位置，随机放入几�?特殊奖项
void SetGameLuckBuf(uint8_t *buf)
{
    uint8_t *ptr = (uint8_t *)buf;
    uint8_t luck_cnt = GetLuckIndex(&DipSw);
    uint16_t dipsw_luck_fun = GetPlayFun(&DipSw);
    uint8_t luck_fun_cnt = 0,i;
    dipsw_luck_fun |= 0xff;//调试用�?�为每�?��?�项都是打开的。？？？？？？？？？�?
    if(dipsw_luck_fun & BONUS_FLG)
    {
        LuckFunBuf[luck_fun_cnt++] = LUCK_BONUS;//1
    }
    if(dipsw_luck_fun & MINGPAI_FLG)
    {
        LuckFunBuf[luck_fun_cnt++] = LUCK_MINGPAI;//2
    }
    if(dipsw_luck_fun & LUCKBIT_FLG)
    {
        LuckFunBuf[luck_fun_cnt++] = LUCK_LUCKBIT;//3
    }
    if(dipsw_luck_fun & FANBEI_FLG)
    {
        LuckFunBuf[luck_fun_cnt++] = LUCK_FANBEI;//4
    }
    if(dipsw_luck_fun & ALLWIN_FLG)
    {
        LuckFunBuf[luck_fun_cnt++] = LUCK_ALLWIN;//5
    }
    if(dipsw_luck_fun & SONGDENG_FLG)
    {
        LuckFunBuf[luck_fun_cnt++] = LUCK_SONGDENG;//6
    }
    if(dipsw_luck_fun & TOUBONUS_FLG)
    {
        LuckFunBuf[luck_fun_cnt++] = LUCK_TOUBONUS;//7
    }
    if(dipsw_luck_fun & JIFEN_FLG)
    {
        LuckFunBuf[luck_fun_cnt++] = LUCK_JIFEN;//8
    }
    for(i=0;i<TOTAL_JUSHU;i++)
    {
        ptr[i] = 0;
    }
    //luck_cnt = (luck_cnt > 4) ? __random()%5 : __random()%luck_cnt;//(0~4)
    luck_cnt = 99;//__random()%5;
    for(i=0;i<luck_cnt;i++)//随机的位�?，放入打开的随机�?�项�?
    {
        //ptr[__random()%100] = LuckFunBuf[__random()%luck_fun_cnt];
        
        if(i%luck_fun_cnt)
        {
            ptr[i] = LuckFunBuf[i%luck_fun_cnt];
        }
        else
        {
            ptr[i] = LUCK_LUCKBIT;
        }
    }
    if((ptr[0]&0x0f)==0x02)
    {
        ptr[0] = 0;//保证�?一局不提示明牌�?
    }
}
uint8_t GetGameLuck(void *buf, uint16_t game_count)
{
    uint8_t *ptr = (uint8_t *)buf;
    uint8_t luck = ptr[game_count];
    uint8_t result = 0,pai;

    luck = ptr[game_count];

    if(luck & 0x0f) 
    {//计算要出
        switch (luck)
        {
        case 0x01: // 彩金
            break;
        case 0x02: // 明牌
            if(game_count == (TOTAL_JUSHU-1))
            {
                luck = 0;//最后一局不需要去计算明牌
            }
            break;
        case 0x03: // 幸运位，0~7
            luck = ((__random()%MAX_FENJI) << 4) +3;
            break;
        case 0x04: // 加倍，1~3
            luck = (((__random() % 3) + 1) << 4) +4;
            break;
        case 0x05: // 全中
            break;
        case 0x06: // 送一�?�?0~4
            result = ((GamePai & 0x0f) >= 0x0e) ? 4 : (GamePai & 0x30) >> 4;
            for (;;)
            {
                pai = __random() % 5;
                if (pai != result)
                {
                    luck = (pai << 4)+6;
                    break;
                }
            }
            break;
        case 0x07: // 大彩金，1~3
            result = ((GamePai & 0x0f) >= 0x0e) ? 4 : (GamePai & 0x30) >> 4;
            switch(result)
            {
                case 0:
                case 1:
                    luck = 0x17;// 头顶小彩金，出黑�?/红�?�才出�?
                    break;
                case 2:
                case 3:
                    luck = 0x27;// 头顶�?彩金，出草花/方块才出�?
                    break;
                case 4:
                    luck = 0x37;// 头顶大彩金，出王才出�?
                    break;
            }
            break;
        case 0x08: // 返积�?
            luck = 8;
            break;
        default:
            luck = 0;
            break;
        }
    }else
    {
        luck = 0;
    }
    return luck;
}

uint32_t ClearBet(ChildType *pChild)
{
    uint32_t bet, i;
    for (i = 0, bet = 0; i < BET_NUMBER; i++)
    {
        bet += pChild->Bet[i];
    }
    if (bet != 0)
    {
        for (i = 0; i < BET_NUMBER; i++)
        {
            pChild->OldBet[i] = pChild->Bet[i];
            pChild->Bet[i] = 0;
        }
    }
    return bet;
}

void ReturnBet(ChildType *pChild)
{
    uint32_t bet = 0, win = 0, credit = pChild->Credit, i;
    if (pChild->Flag & COMPUTER)
    {
        for (i = 0; i < BET_NUMBER; i++)
        {
            pChild->Bet[0] = 0;
        }
        win = pChild->Win;
        pChild->Win = 0;
    }
    else
    {
        bet = ClearBet(pChild);
    }
    if (bet != 0 || win != 0)
    {
        pChild->Credit = credit + bet + win;
        WriteChildAccu(pChild, eeCredit, pChild->Credit);
    }
}

uint32_t LuckLineBonus_Win2(uint8_t luck)
{ // 按结果上的押分比例分配彩金。Luck=0x01;
    uint8_t result,i;
    uint32_t win2 = 0, result_allbet, dec_bet,result_maxbet;
    ChildType *pAllChild;
    dec_bet = GetDec(&DipSw);
    result = ((GamePai & 0x0f) >= 0x0e) ? 4 : (GamePai & 0x30) >> 4;
    result_maxbet = (dec_bet*100)/(g_NewMul[0][result]-100);
    
    for (i = 0; i < MAX_FENJI; i++)
    {
        pAllChild = &Child[i];
        result_allbet += pAllChild->Bet[result];
    }
    if (result_allbet < result_maxbet)
    {
        result_allbet = result_maxbet;
    }
    for (i = 0; i < MAX_FENJI; i++)
    {
        pAllChild = &Child[i];
        pAllChild->Win2 = (pAllChild->Bet[result] * LineBonus) / result_allbet;
        win2 += pAllChild->Win2;
    }
    return win2;
}

uint32_t LuckMingPai_Win2(uint8_t luck)
{ // 计算下一局出明牌，送出去的分数。Luck=0x02;
    uint8_t i, pai, result;
    uint32_t win2 = 0;
    ChildType *pAllChild;

    pai = GetGameResult(PlayPaiBuf, GameCount + 1);
    result = ((pai & 0x0f) >= 0x0e) ? 4 : (pai & 0x30) >> 4;

    for (i = 0; i < MAX_FENJI; i++)
    {
        pAllChild = &Child[i];
        pAllChild->Win2= (pAllChild->OldBet[result] * g_NewMul[0][result]) / 100;
        win2 += pAllChild->Win2;
    }
    return win2;
}

uint32_t LuckLuckBit_Win2(uint8_t luck)
{// 幸运位。Luck=0x03;
    uint8_t result,j,fenji;
    uint32_t win2 = 0;
    ChildType *pAllChild;

    result = ((GamePai & 0x0f) >= 0x0e) ? 4 : (GamePai & 0x30) >> 4;
    fenji = luck>>4;
    // for(i=0;i<MAX_FENJI;i++)
    {
        pAllChild = &Child[fenji];
        pAllChild->Win2 = 0;
        for (j = 0; j < BET_NUMBER; j++)
        {
            if (j != result)
            {
                pAllChild->Win2 += (pAllChild->Bet[j] * g_NewMul[0][j])/100;
            }
        }
        win2 = pAllChild->Win2;
    }
    return win2;
}

uint32_t LuckFanBei_Win2(uint8_t luck)
{// 加倍。Luck=0x04;
    uint8_t result,i,fanbei;
    uint32_t win2 = 0;
    ChildType *pAllChild;

    result = ((GamePai & 0x0f) >= 0x0e) ? 4 : (GamePai & 0x30) >> 4;
    fanbei = luck>>4;
    for (i = 0; i < MAX_FENJI; i++)
    {
        pAllChild = &Child[i];
        pAllChild->Win2 = pAllChild->Bet[result] * fanbei;
        win2 += pAllChild->Win2;
    }
    return win2;
}
uint32_t LuckQuanZhong_Win2(uint8_t luck)
{ // 全中。Luck=0x05;
    uint8_t result,i, j;
    uint32_t win2 = 0;
    ChildType *pAllChild;

    result = ((GamePai & 0x0f) >= 0x0e) ? 4 : (GamePai & 0x30) >> 4;

    for (i = 0; i < MAX_FENJI; i++)
    {
        pAllChild = &Child[i];
        pAllChild->Win2 = 0;
        for (j = 0; j < BET_NUMBER; j++)
        {
            if (j != result)
            {
                pAllChild->Win2 += (pAllChild->Bet[j] * g_NewMul[0][j])/100;
            }
        }
        win2 += pAllChild->Win2;
    }
    return win2;
}

uint32_t LuckSongDeng_Win2(uint8_t luck)
{ // 送一�?，Luck=0x06;
    uint8_t i,songdeng;
    uint32_t win2 = 0;
    ChildType *pAllChild;

    //result = ((GamePai & 0x0f) >= 0x0e) ? 4 : (GamePai & 0x30) >> 4;
    songdeng = luck>>4;
    for (i = 0; i < MAX_FENJI; i++)
    {
        pAllChild = &Child[i];
        pAllChild->Win2 = (pAllChild->Bet[songdeng] * g_NewMul[0][songdeng])/100;
        win2 += pAllChild->Win2;
    }
    return win2;
}
uint32_t LuckTouBonus_Win2(uint8_t luck)
{//头顶彩金，Luck=0x07
    uint8_t result,i;
    uint32_t win2 = 0, result_allbet=0, dec_bet, toubonus,result_maxbet;
    ChildType *pAllChild;

    dec_bet = GetDec(&DipSw);
    result = ((GamePai & 0x0f) >= 0x0e) ? 4 : (GamePai & 0x30) >> 4;
    result_maxbet = (dec_bet*100)/(g_NewMul[0][result]-100);

    switch (luck&0x30)
    {
    case 0x10:
        toubonus = MinBonus;
        break;
    case 0x20:
        toubonus = MidBonus;
        break;
    case 0x30:
        toubonus = MaxBonus;
        break;
    default:
        toubonus = 0;
        break;
    }
    for (i = 0; i < MAX_FENJI; i++)
    {
        pAllChild = &Child[i];
        result_allbet += pAllChild->Bet[result];
    }
    if (result_allbet < result_maxbet)
    {
        result_allbet = result_maxbet;
    }
    for (i = 0; i < MAX_FENJI; i++)
    {
        pAllChild = &Child[i];
        pAllChild->Win2 = (pAllChild->Bet[result] * toubonus) / result_allbet;
        win2 += pAllChild->Win2;
    }
    return win2;
}
uint32_t LuckJiFen_Win2(uint8_t luck)
{ // �?分功能，Luck=0x08;
    uint8_t i,fenji;
    uint32_t win2 = 0,jifen[MAX_FENJI],max_jifen,dec_bet,allbet[MAX_FENJI];
    ChildType *pAllChild;
    dec_bet = GetDec(&DipSw);
    for (i = 0; i < MAX_FENJI; i++)
    {
        pAllChild = &Child[i];
        allbet[i] = pAllChild->Bet[0]+pAllChild->Bet[1]+pAllChild->Bet[2]+pAllChild->Bet[3]+pAllChild->Bet[4];
        if(allbet[i] >= (dec_bet/10))
        {
            jifen[i]=pAllChild->JiFen;
        }
        else
        {
            jifen[i]=0;
        }
    }
    for (max_jifen = 0,i = 0; i < MAX_FENJI; i++)
    {
        pAllChild = &Child[i];
        if(jifen[i] > max_jifen)
        {
            max_jifen = jifen[i];
            fenji=i;
        }
    }
    if(max_jifen)
    {
        pAllChild = &Child[fenji];
        win2 = pAllChild->Win2 = pAllChild->JiFen;
        Luck_CS = (fenji<<4)+8;//再�?�修改Luck
    }
    else
    {
        win2 = 0;
    }
    return win2;
}
uint32_t GetGameLuckWin2(uint8_t luck)
{
    uint32_t luck_win2;
    switch(luck & 0x0f)
    {
        case 0x01://连线彩金
            luck_win2 = LuckLineBonus_Win2(luck);
            break;
        case 0x02://明牌
            luck_win2 = LuckMingPai_Win2(luck);
            break;
        case 0x03://幸运�?
            luck_win2 = LuckLuckBit_Win2(luck);
            break;
        case 0x04://翻�?
            luck_win2 = LuckFanBei_Win2(luck);
            break;
        case 0x05://全中
            luck_win2 = LuckQuanZhong_Win2(luck);
            break;
        case 0x06://送一�?
            luck_win2 = LuckSongDeng_Win2(luck);
            break;
        case 0x07://头顶彩金
            luck_win2 = LuckTouBonus_Win2(luck);
            break;
        case 0x08://返积�?
            //luck_win2 = LuckLineBonus_Win2(luck);
            luck_win2 = 0;
            break;
        default://
            luck_win2 = 0;
            break;
    }
    return luck_win2;
}

