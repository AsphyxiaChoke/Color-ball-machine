#include "MasterCmd.h"

uint8_t MasterReadResult()
{
	if (MingPai)
	{
		uint8_t result;
		MingPai = 2;
        result = ((GamePai & 0x0f) >= 0x0e) ? 4 : (GamePai & 0x30) >> 4;
		Luck = (result<<4) + LUCK_MINGPAI;
		return GamePai&0x3f;
	}
	else
	{
		Luck = 0;
		return 0;
	}
}
void MasterRuncmd(uint8_t cmd)
{
	uint8_t i, *point;
	uint16_t checksum;
	MasterCmdType masterbuf;
	masterbuf.procmd = cmd;
	masterbuf.gametime = GameTime;
	masterbuf.totalgame = ChildTotalGame;
	masterbuf.gamecount = ChildGameCount;
	masterbuf.linebonus = LineBonus;
	masterbuf.result = MasterReadResult();				//这个要处理
	masterbuf.Luck = Luck;
	masterbuf.Conv = 0;
    masterbuf.Dec = GetDec(&DipSw);
    masterbuf.childcredit = 0;
    masterbuf.childwin = 0;

	masterbuf.hei_cnt = HeiCnt;
	masterbuf.hong_cnt = HongCnt;
	masterbuf.mei_cnt = CaoCnt;
	masterbuf.fang_cnt = FangCnt;
	masterbuf.wang_cnt = WangCnt;
	for (i = 0; i < BET_NUMBER; i++)
	{
		masterbuf.childbet[i] = 0;
		masterbuf.childTotalbet[i] = Child[0].Bet[i] + Child[1].Bet[i] + Child[2].Bet[i] + Child[3].Bet[i] +
									 Child[4].Bet[i] + Child[5].Bet[i] + Child[6].Bet[i] + Child[7].Bet[i] ;
	}
	masterbuf.childid = MAX_FENJI+1;
	masterbuf.gamestation = Gamestation;
	masterbuf.minbonus = MinBonus;
    masterbuf.midbonus = MidBonus;
    masterbuf.maxbonus = MaxBonus;
    masterbuf.childwin2 = Child[0].Win2+Child[1].Win2+Child[2].Win2+Child[3].Win2+
						  Child[4].Win2+Child[5].Win2+Child[6].Win2+Child[7].Win2;
    checksum = 0;
	for (i = 0, point = (uint8_t *)&masterbuf; i < sizeof(MasterCmdType) - 2; i++)
	{
		register uint8_t ch;
		ch = *point;
		checksum += ch;
		point++;
	}
	masterbuf.checksum = checksum;
    USART_SendDataString(1, (uint8_t*)&masterbuf, sizeof(MasterCmdType));
}
void MasterRunsult(uint8_t cmd)
{
	uint8_t i, *point;
	uint16_t checksum;
	MasterCmdType masterbuf;
	masterbuf.procmd = cmd;
	masterbuf.gametime = GameTime;
	masterbuf.totalgame = ChildTotalGame;
	masterbuf.gamecount = ChildGameCount;
	masterbuf.linebonus = LineBonus;
	masterbuf.result = GamePai;
	masterbuf.Luck = Luck;
	masterbuf.Conv = 0;
    masterbuf.Dec = GetDec(&DipSw);
    masterbuf.childcredit = 0;
    masterbuf.childwin = 0;

	masterbuf.hei_cnt = HeiCnt;
	masterbuf.hong_cnt = HongCnt;
	masterbuf.mei_cnt = CaoCnt;
	masterbuf.fang_cnt = FangCnt;
	masterbuf.wang_cnt = WangCnt;
	for (i = 0; i < BET_NUMBER; i++)
	{
		masterbuf.childbet[i] = 0;
		masterbuf.childTotalbet[i] = Child[0].Bet[i] + Child[1].Bet[i] + Child[2].Bet[i] + Child[3].Bet[i] +
									 Child[4].Bet[i] + Child[5].Bet[i] + Child[6].Bet[i] + Child[7].Bet[i] ;
	}
	masterbuf.childid = MAX_FENJI+1;
	masterbuf.gamestation = Gamestation;
	masterbuf.minbonus = MinBonus;
    masterbuf.midbonus = MidBonus;
    masterbuf.maxbonus = MaxBonus;
    masterbuf.childwin2 = Child[0].Win2+Child[1].Win2+Child[2].Win2+Child[3].Win2+
						  Child[4].Win2+Child[5].Win2+Child[6].Win2+Child[7].Win2;
	checksum = 0;
	for (i = 0, point = (uint8_t *)&masterbuf; i < sizeof(MasterCmdType) - 2; i++)
	{
		register uint8_t ch;
		ch = *point;
		checksum += ch;
		point++;
	}
	masterbuf.checksum = checksum;
	USART_SendDataString(1, (uint8_t *)&masterbuf, sizeof(MasterCmdType));
}

void MasterRunHistory(void)
{
	uint8_t i, *point;
	uint16_t checksum = 0;
	MasterHistoryType cmdbuf;
	cmdbuf.procmd = PC_HISTORY;
	memcpy(cmdbuf.historybuf, History, sizeof(cmdbuf.historybuf));
	for (i = 0, point = (uint8_t *)&cmdbuf; i < sizeof(MasterHistoryType) - 2; i++)
	{
		register uint8_t ch;
		ch = *point;
		checksum += ch;
		point++;
	}
	cmdbuf.checksum = checksum;
	USART_SendDataString(1, (uint8_t *)&cmdbuf, sizeof(MasterHistoryType));
}

void MasterRunLuckHistory(void)
{
	uint8_t i, *point;
	uint16_t checksum = 0;
	MasterHistoryType cmdbuf;
	cmdbuf.procmd = PC_LUCKHISTORY;
	memcpy(cmdbuf.historybuf, LuckHistory, sizeof(cmdbuf.historybuf));
	for (i = 0, point = (uint8_t *)&cmdbuf; i < sizeof(MasterHistoryType) - 2; i++)
	{
		register uint8_t ch;
		ch = *point;
		checksum += ch;
		point++;
	}
	cmdbuf.checksum = checksum;
	USART_SendDataString(1, (uint8_t *)&cmdbuf, sizeof(MasterHistoryType));
}
