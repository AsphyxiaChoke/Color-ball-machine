#include "GameOption.h"

void SetGameResult(uint8_t* ptr, uint32_t game_count, uint8_t val)
{
    ptr[game_count]=val;
}

uint8_t GetGameResult(void* buf, uint32_t game_count)
{
    uint8_t *ptr=(uint8_t*)buf;
    uint8_t pai=ptr[game_count];
    if ((pai&0x0F)==0x00 || (pai&0xf0)>0x30)
    {
        pai=(__random()&0x37)+(__random()&0x03)+2;
        pai&=0x3F;
        SetGameResult(ptr,game_count,pai);
    }
    return pai;
}
uint8_t GetGameLuck(void)
{
    uint8_t luck = __random()%11;
    uint8_t result = 0;
    uint16_t luck_index = GetLuckIndex(&DipSw);
    //最大出特殊奖的概率为15/128
    //if ((luck_index&0x000F) > (__random()&0x007f) || YK_Luck)
    {
        /*if(YK_Luck)
        {
            luck = YK_Luck;
        }
        else
        {
            luck = __random()&0x07 + __random()&0x03;
        }*/
        switch (luck)
        {
        case 0x01://彩金
            break;
        case 0x02://明牌
            result = ((GamePai & 0x0f) >= 0x0e) ? 4 : (GamePai & 0x30) >> 4;
            luck = luck + (result + 1)<<4;
            break;
        case 0x03://幸运位，1~6
            luck = luck + ((__random() % MAX_FENJI) + 1)<<4;
            break;
        case 0x04://加倍，1~3
            luck = luck + ((__random() % 3) + 1)<<4;
            break;
        case 0x05://全中
            break;
        case 0x06://送一门
            result = ((GamePai & 0x0f) >= 0x0e) ? 4 : (GamePai & 0x30) >> 4;
            for(;;)
            {
                luck = __random() % 5;
                if(luck != result)
                {
                    luck = 0x06 + luck<<4;
                    break;
                }
            }
            break;
        case 0x07://大彩金
            break;
        case 0x08://中彩金
            break;
        case 0x09://小彩金
            break;
        case 0x0a://返积分
            break;
        default:
            break;
        }
    }
    return luck;
}

uint32_t ClearBet(ChildType *pChild)
{
    uint32_t bet,i;
    for(i=0,bet=0;i<BET_NUMBER;i++)
    {
        bet+=pChild->Bet[i];
    }
    if(bet!=0)
    {
        for(i=0;i<BET_NUMBER;i++)
        {
            pChild->OldBet[i]=pChild->Bet[i];
            pChild->Bet[i]=0;
        }
    }
    return bet;
}

void ReturnBet(ChildType *pChild)
{
    uint32_t bet=0, win=0, credit=pChild->Credit,i;
    if(pChild->Flag&COMPUTER)
    {
        for(i=0;i<BET_NUMBER;i++)
        {
    	    pChild->Bet[0]=0;
        }
        win=pChild->Win;
        pChild->Win=0;
    }
    else
    {
    	bet=ClearBet(pChild);
    }
    if (bet!=0 || win!=0)
    {
        pChild->Credit=credit+bet+win;
        WriteChildAccu(pChild, eeCredit,pChild->Credit);
    }
}

