#include "MasterCmd.h"

uint8_t MasterReadResult()
{
	if (Luck & 0x06)
	{
		return GamePai;
	}
	return 0;
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
	masterbuf.result1 = MasterReadResult();				//这个要处理
	masterbuf.Luck = Luck;
	masterbuf.Conv = GetDec(&DipSw);
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
	masterbuf.gamestation = Gamestation;
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
	masterbuf.result1 = GamePai;
	masterbuf.Luck = Luck;
	masterbuf.Conv = GetDec(&DipSw);
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
	masterbuf.gamestation = Gamestation;
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
