#include "arith.h"

//-------------------------------------------------
// 分机按键灯处理
//-------------------------------------------------
void SetGameTime(uint32_t time)
{
	GameTime = time;
}
void DispCREDIT(ChildType *pChild) {}
void DispWIN(ChildType *pChild) {}
void DispBET(ChildType *pChild, uint32_t index) {}
void DispConv(ChildType *pChild) {}
void FlashWin(uint32_t flash) { ; }
void FlashBet(uint32_t index, uint32_t flash) { ; }
void FlashLuckBet(ChildType *pChild, uint8_t flash) { ; }
void FlashWinLamp(ChildType *pChild, uint32_t flash) { ; }

void BuzzerDeal(ChildType *pChild) { ; }
void Buzzer(ChildType *pChild) { ; }
void DispBei(ChildType *pChild, char flash) {}

void DispBetLED(ChildType *pChild, uint32_t credit) // 改动押分灯位置
{
	if (credit == 0)
	{
		// 3到7是按键灯控制位
		pChild->KeyLED &= (~(CHILD_BET0_LED | CHILD_BET1_LED | CHILD_BET2_LED | CHILD_BET3_LED | CHILD_BET4_LED));
	}
	else
	{ // 开灯
		if (ChildMaxBET(pChild, 0))
			pChild->KeyLED |= CHILD_BET0_LED;
		else
			pChild->KeyLED &= (~CHILD_BET0_LED);
		if (ChildMaxBET(pChild, 1))
			pChild->KeyLED |= CHILD_BET1_LED;
		else
			pChild->KeyLED &= (~CHILD_BET1_LED);
		if (ChildMaxBET(pChild, 2))
			pChild->KeyLED |= CHILD_BET2_LED;
		else
			pChild->KeyLED &= (~CHILD_BET2_LED);
		if (ChildMaxBET(pChild, 3))
			pChild->KeyLED |= CHILD_BET3_LED;
		else
			pChild->KeyLED &= (~CHILD_BET3_LED);
		if (ChildMaxBET(pChild, 4))
			pChild->KeyLED |= CHILD_BET4_LED;
		else
			pChild->KeyLED &= (~CHILD_BET4_LED);
	}
	pChild->pChildDisp->Count[0] = (uint8_t)(pChild->KeyLED | pChild->Count & TUI_FLAG); // 押分灯
	pChild->pChildDisp->Count[1] = (uint8_t)(pChild->KeyLED >> 8);
	pChild->pChildDisp->Count[2] = (uint8_t)(pChild->KeyLED >> 16);
}

void FlashBetLED(ChildType *pChild, uint32_t flash)
{
	if (flash)
	{
		pChild->KeyLED &= (~(CHILD_BET0_LED | CHILD_BET1_LED | CHILD_BET2_LED | CHILD_BET3_LED | CHILD_BET4_LED));
	}
	else
	{
		uint8_t pai = GamePai;
		switch (pai & 0x30)
		{
		case 0x00:
			if (((pai & 0x0f) == 0x0f) || ((pai & 0x0f) == 0x0e))
			{
				pChild->KeyLED |= CHILD_BET4_LED;
			}
			else
			{
				pChild->KeyLED |= CHILD_BET0_LED;
			}
			break;
		case 0x10:
			pChild->KeyLED |= CHILD_BET1_LED;
			break;
		case 0x20:
			pChild->KeyLED |= CHILD_BET2_LED;
			break;
		case 0x30:
			pChild->KeyLED |= CHILD_BET3_LED;
			break;
		}
	}
	pChild->pChildDisp->Count[0] = (uint8_t)(pChild->KeyLED ); // 押分灯
	pChild->pChildDisp->Count[1] = (uint8_t)(pChild->KeyLED >> 8);
	pChild->pChildDisp->Count[2] = (uint8_t)(pChild->KeyLED >> 16);
}

void FlashLuckBetLED(ChildType *pChild, uint8_t flash)
{
	if (flash)
	{
		pChild->KeyLED &= (~(CHILD_BET0_LED | CHILD_BET1_LED | CHILD_BET2_LED | CHILD_BET3_LED | CHILD_BET4_LED));
	}
	else
	{
		pChild->KeyLED |= (CHILD_BET0_LED | CHILD_BET1_LED | CHILD_BET2_LED | CHILD_BET3_LED | CHILD_BET4_LED);
	}
	pChild->pChildDisp->Count[0] = (uint8_t)(pChild->KeyLED | pChild->Count & TUI_FLAG); // 押分灯
	pChild->pChildDisp->Count[1] = (uint8_t)(pChild->KeyLED >> 8);
	pChild->pChildDisp->Count[2] = (uint8_t)(pChild->KeyLED >> 16);
}
//---------------------------------------------------------
// 投退 上下分处理
//---------------------------------------------------------
uint32_t GetConv(ChildType *pChild)
{
	if (pChild->Conv < 10)
		return 1;
	if (pChild->Conv < 100)
		return 10;
	return 100;
}

int ChildIsTui(ChildType *pChild)
{
	return (pChild->Count & TUI_FLAG) ? 1 : 0;
}
uint32_t GetSSR(ChildType *pChild) // 得到现在退币个数
{
	uint32_t coin_out_tmp = pChild->CoinOutTmp;
	uint32_t tmp = coin_out_tmp - pChild->CoinOutTmp2;
	pChild->CoinOutTmp2 = coin_out_tmp;
	if (pChild->TuiBiNum > tmp)
	{
		pChild->TuiBiNum -= tmp;
	}
	else
	{
		pChild->TuiBiNum = 0;
	}
	if ((pChild->DispCount & SSR_OPEN) == 0)
	{
		pChild->Flag &= ~TUI_FLAG;
	}
	return tmp;
}
__inline void OpenXI(ChildType *pChild)
{
	pChild->Flag |= XI_FLAG;
}
__inline void CloseXI(ChildType *pChild)
{
	pChild->Flag &= ~XI_FLAG;
}
__inline int ChildIsXI(ChildType *pChild)
{
	return (pChild->Flag & XI_FLAG) ? 1 : 0;
}

int ChildKeyScan(ChildType *pChild, uint32_t mask)
{
	mask = pChild->ChildKeyPress & mask;
	if ( mask != 0)
	{
		CPU_SR cpu_sr;
		CPU_CRITICAL_ENTER();
		pChild->ChildKeyPress &= ~mask;
		CPU_CRITICAL_EXIT();
		return 1;
	}
	return 0;
}


int ChildKeyXiOrTui(ChildType *pChild)
{
	if (pChild->Flag & TUI_FLAG)
		return 1;
	if (pChild->Flag & XI_FLAG)
		return 1;
	return 0;
}

uint32_t ChildKeyTOU(ChildType *pChild)
{
	uint32_t coin_in_tmp = pChild->CoinInTmp;
	uint32_t tmp = (coin_in_tmp - pChild->CoinInTmp2);
	if (tmp)
		pChild->CoinInTmp2 = coin_in_tmp;
	return tmp;
}

void OpenSSR(ChildType *pChild)
{
	if ((pChild->DispCount & SSR_OPEN) == 0)
	{
		pChild->Flag |= TUI_FLAG;
		pChild->TuiBiNum = 0;
		SetTuiBiNum(pChild);
		pChild->CoinOutTime = 15 * COIN_TIME_SPEED;
		pChild->DispCount |= SSR_OPEN; // 打开SSR
		pChild->pChildDisp->Count[0] = pChild->DispCount;
	}
}

void CloseSSR(ChildType *pChild)
{
	pChild->pChildDisp->Count[0] = (pChild->KeyLED | (pChild->Flag &= (~TUI_FLAG))); // SSR
}
//----------------------------------------
// 配置输出
//----------------------------------------
void Scene(void)
{
	Fpga_wreg->Send_crt = 0x01;
}

uint32_t WriteChildAccu(ChildType *pChild, uint32_t index, uint32_t value)
{
	int i;
	uint32_t ee_addr = 0;
	for (i = 0; i < MAX_FENJI; i++)
		if (pChild == &Child[i])
			break;
	ee_addr = index + i * 4;
	return WriteEEPROM(ee_addr,(uint8_t*)&value, sizeof(uint32_t));
}
//----------------------------------------
// 分机处理
//----------------------------------------
void SetTuiBiNum(ChildType *pChild)
{
	if (pChild->Credit >= GetCoinOut(&DipSw) && GetCoinOut(&DipSw) > 0 && pChild->TuiBiNum == 0)
	{
		pChild->TuiBiNum = pChild->Credit / GetCoinOut(&DipSw);
		if (pChild->TuiBiNum > 100)
			pChild->TuiBiNum = 100;
		pChild->pChildDisp->TuiBiNumLow = pChild->TuiBiNum & 0xff;
		pChild->pChildDisp->TuiBiNumHigh = (pChild->TuiBiNum >> 8) & 0xff;
		pChild->pChildDisp->TuiBiCnt1++;
		pChild->pChildDisp->TuiBiCnt2 = pChild->pChildDisp->TuiBiCnt1;
		pChild->CoinOutTmp2 = pChild->CoinOutTmp;
	}
}
#warning "这个MachineIsHalt要重写"
int64_t MachineIsHalt(void)	//这个
{
	return 1;
}
#define MECHINE_FLAG ((MachineIsHalt() > 0))// && (!(gsmFlag & GSM_POWER))) // 加个短信当机

uint8_t  IsOver(uint32_t v) { return ((v)>=GetChildOver(&DipSw))?1:0; }
void CoinKeyProc1(ChildType *pChild)
{
	uint32_t num =GetSSR(pChild);
	if (num != 0) // 退币SSR
	{
		uint32_t total = num * GetCoinOut(&DipSw);
		pChild->Credit = (pChild->Credit <= total) ? 0 : (pChild->Credit - total);
		CurrPayoff -= total;
		pChild->CoinOut += total;
		pChild->JiFen = 0;//有退币，玩家积分清零。
		pChild->JiFenBet = 0;//有退币，玩家积分清零。
		pChild->JiFenWin = 0;//有退币，玩家积分清零。
		SetTuiBiNum(pChild);
		WriteChildAccu(pChild, eeCredit, pChild->Credit);
		WriteEEPROM(eeCurrPayoff, (uint8_t*)&CurrPayoff, sizeof(CurrPayoff));
		WriteChildAccu(pChild, eeCOIN_OUT, pChild->CoinOut);
	}
	// 分机关闭不准按键
	// if ((pChild->DispCount & 0xe0) != 0xA0) ///????????
	// 	return;
	{
#ifdef __LOCK_KAIXI__
		CoinKeyCreditOut(lpChild, pcount);
#else

		if (ChildKeyXI(pChild))
		{
			uint32_t note;
			if ((int32_t)pChild->Credit >= 100)
				note = 100;
			else if ((int32_t)pChild->Credit >= 10)
				note = 10;
			else if ((int32_t)pChild->Credit >= 1)
				note = 1;
			else
				note = 0;
			if (pChild->Credit >= note && note != 0)
			{
				pChild->JiFen = 0;//有下分，玩家积分清零。
				pChild->JiFenBet = 0;//有退币，玩家积分清零。
				pChild->JiFenWin = 0;//有退币，玩家积分清零。
				
				pChild->Credit -= note;
				CurrPayoff -= note;
				pChild->KeyOut += note;
				WriteChildAccu(pChild, eeKEY_OUT, pChild->KeyOut);
				WriteChildAccu(pChild, eeCredit, pChild->Credit);
				WriteEEPROM(eeCurrPayoff, (uint8_t*)&CurrPayoff, sizeof(CurrPayoff));
				OpenXI(pChild);
			}
			else
			{
				CloseXI(pChild);
				ChildKeyBET0(pChild);
				ChildKeyBET1(pChild);
				ChildKeyBET2(pChild);
				ChildKeyBET3(pChild);
				ChildKeyBET4(pChild);
				ChildKeyKAI(pChild);
				ChildKeyKAI2(pChild);
				ChildKeyXI(pChild);
			}
		}
		else
		{
	    	CloseXI(pChild);
		}	
#endif
	}
	

	if ((num = ChildKeyTOU(pChild)) != 0 && MECHINE_FLAG) // 投币
	{
		uint32_t coin = num * GetCoinIn(&DipSw);

		pChild->Credit += coin;
		CurrPayoff += coin;
		pChild->CoinIn += coin;
		WriteChildAccu(pChild, eeCredit, pChild->Credit);
		WriteEEPROM(eeCurrPayoff, (uint8_t*)&CurrPayoff, sizeof(CurrPayoff));
		WriteChildAccu(pChild, eeCOIN_IN, pChild->CoinIn);
		return;
	}

	/*if (ChildKeyCONV(pChild)) // 转换键
	{
		uint32_t conv = GetConv(pChild);
		if (conv < 10)
			pChild->Conv = 10; // 1
		else if (conv >= 100)
			pChild->Conv = 5; // 10
		else
			pChild->Conv = 100;
		return;
	}*/
	if (ChildKeyCONV10(pChild)) // 转换键
	{
		pChild->Conv = 10; // 1
		return;
	}
	if (ChildKeyCONV50(pChild)) // 转换键
	{
		pChild->Conv = 50; // 1
		return;
	}
	if (ChildKeyCONV100(pChild)) // 转换键
	{
		pChild->Conv = 100; // 1
		return;
	}
	if (ChildKeyCONV500(pChild)) // 转换键
	{
		pChild->Conv = 500; // 1
		return;
	}
	if (ChildKeyCONV1000(pChild)) // 转换键
	{
		pChild->Conv = 1000; // 1
		return;
	}
	// 如果正在退币或洗分,立刻返回
	if (ChildKeyXiOrTui(pChild))
		return;

	if (ChildKeyTUI(pChild)) // 退币
	{
		if (pChild->Credit >= (uint32_t)GetCoinOut(&DipSw) && GetCoinOut(&DipSw) > 0)
		{
			OpenSSR(pChild);
			return; // 这里必须返回,防止开分
		}
	}
	if (ChildKeyKAI(pChild)) // 开分
	{
		if (!IsOver(pChild->Credit + pChild->Credit) && MECHINE_FLAG)
		{
			uint32_t note = GetNoteIn(&DipSw);
			pChild->Credit += note;
			CurrPayoff += note;
			pChild->KeyIn += note;

			WriteChildAccu(pChild, eeCredit, pChild->Credit);
			WriteEEPROM(eeCurrPayoff, (uint8_t*)&CurrPayoff, sizeof(CurrPayoff));
			WriteChildAccu(pChild, eeKEY_IN, pChild->KeyIn);
		}
	}

	if (ChildKeyKAI2(pChild)) // 小开分
	{
		if (!IsOver(pChild->Credit + pChild->Credit) && MECHINE_FLAG)
		{
			uint32_t note;

			if (pChild->Conv < 10)
				note = 10;
			else if (pChild->Conv < 100)
				note = 100;
			else
				note = 1000;

			pChild->Credit += note;
			CurrPayoff += note;
			pChild->KeyIn += note;
			WriteChildAccu(pChild, eeCredit, pChild->Credit);
			WriteEEPROM(eeCurrPayoff, (uint8_t*)&CurrPayoff, sizeof(CurrPayoff));
			WriteChildAccu(pChild, eeKEY_IN, pChild->KeyIn);
		}
	}
}

uint32_t ChildCancel(ChildType *pChild)
{
	ChildType *pAllChild = Child;
	uint32_t all_bet[4], dec_bet;
	all_bet[0] = pAllChild[0].Bet[0] + pAllChild[1].Bet[0] + pAllChild[2].Bet[0] + pAllChild[3].Bet[0] +
				 pAllChild[4].Bet[0] + pAllChild[5].Bet[0] + pAllChild[6].Bet[0] + pAllChild[7].Bet[0] + DingFen[0];
	all_bet[1] = pAllChild[0].Bet[1] + pAllChild[1].Bet[1] + pAllChild[2].Bet[1] + pAllChild[3].Bet[1] +
				 pAllChild[4].Bet[1] + pAllChild[5].Bet[1] + pAllChild[6].Bet[1] + pAllChild[7].Bet[1] + DingFen[1];
	all_bet[2] = pAllChild[0].Bet[2] + pAllChild[1].Bet[2] + pAllChild[2].Bet[2] + pAllChild[3].Bet[2] +
				 pAllChild[4].Bet[2] + pAllChild[5].Bet[2] + pAllChild[6].Bet[2] + pAllChild[7].Bet[2] + DingFen[2];
	all_bet[3] = pAllChild[0].Bet[3] + pAllChild[1].Bet[3] + pAllChild[2].Bet[3] + pAllChild[3].Bet[3] +
				 pAllChild[4].Bet[3] + pAllChild[5].Bet[3] + pAllChild[6].Bet[3] + pAllChild[7].Bet[3] + DingFen[3];

	dec_bet = GetDec(&DipSw);

	all_bet[0] -= pChild->Bet[0];
	all_bet[1] -= pChild->Bet[1];
	all_bet[2] -= pChild->Bet[2];
	all_bet[3] -= pChild->Bet[3];
	if (all_bet[0] > (all_bet[1] + all_bet[2] + all_bet[3] + dec_bet) / 3)
		return 0;
	if (all_bet[1] > (all_bet[0] + all_bet[2] + all_bet[3] + dec_bet) / 3)
		return 0;
	if (all_bet[2] > (all_bet[0] + all_bet[1] + all_bet[3] + dec_bet) / 3)
		return 0;
	if (all_bet[3] > (all_bet[0] + all_bet[1] + all_bet[2] + dec_bet) / 3)
		return 0;

	pChild->Credit += pChild->Bet[0] + pChild->Bet[1] + pChild->Bet[2] + pChild->Bet[3] + pChild->Bet[4];
	pChild->Bet[0] = pChild->Bet[1] = pChild->Bet[2] = pChild->Bet[3] = pChild->Bet[4] = 0;
	return 1;
}
uint32_t ChildMaxBET(ChildType *pChild, uint32_t index)
{
	uint8_t i;
	uint32_t all_bet[5], max_bet, dec_bet, bet, total_bet;
	uint8_t pai = GetGameResult(PlayPaiBuf, GameCount);
	uint32_t bei = 300;
	max_bet = (9999 * 8);
	dec_bet = GetDec(&DipSw);
	// 如果顶分 all_bet[0] [1] [2] [3] 里加了顶分
	all_bet[0] = DingFen[0];
	all_bet[1] = DingFen[1];
	all_bet[2] = DingFen[2];
	all_bet[3] = DingFen[3];
	all_bet[4] = 0;
	for (i = 0; i < MAX_FENJI; i++)
	{
		all_bet[0] += Child[i].Bet[0];
		all_bet[1] += Child[i].Bet[1];
		all_bet[2] += Child[i].Bet[2];
		all_bet[3] += Child[i].Bet[3];
		all_bet[4] += Child[i].Bet[4];
	}
	switch (index) // 顶分不包括鬼的分
	{
	case 0:
		total_bet = ((all_bet[1] + all_bet[2] + all_bet[3] + dec_bet) * 100) / bei; // 最高可以押多少分
		bet = (all_bet[0] >= total_bet) ? 0 : total_bet - all_bet[0];				// 还能押多少
		if (bet >= max_bet)
			bet = 0;
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		if (bet == 0 && g_AiDingfenSw) // 说明本门已经押满。
		{
			if ((pai & 0x0f) != 0x00 &&
				(pai & 0x0f) <= 0x0D &&
				(pai & 0x30) == 0x00 &&
				pChild->Bet[0] > (dec_bet / 3))
			{
				if ((pChild->Bet[1] > pChild->Bet[0]) ||
					(pChild->Bet[2] > pChild->Bet[0]) ||
					(pChild->Bet[3] > pChild->Bet[0]))
				{
					if ((pChild->Bet[1] <= 100) ||
						(pChild->Bet[2] <= 100) ||
						(pChild->Bet[3] <= 100))
					{
						if (DingFen[index] > 0)
							g_DingFen1_FX = 1;
					}
				}
			}
		}
		// end+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		break;
	case 1:
		total_bet = ((all_bet[0] + all_bet[2] + all_bet[3] + dec_bet) * 100) / bei; // 最高可以押多少分
		bet = (all_bet[1] >= total_bet) ? 0 : total_bet - all_bet[1];				// 还能押多少
		if (bet >= max_bet)
			bet = 0;
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		if (bet == 0 && g_AiDingfenSw) // 说明本门已经押满。
		{
			if ((pai & 0x0f) != 0x00 &&
				(pai & 0x0f) <= 0x0D &&
				(pai & 0x30) == 0x10 &&
				pChild->Bet[1] > (dec_bet / 3))
			{
				if ((pChild->Bet[0] > pChild->Bet[1]) ||
					(pChild->Bet[2] > pChild->Bet[1]) ||
					(pChild->Bet[3] > pChild->Bet[1]))
				{
					if ((pChild->Bet[0] <= 100) ||
						(pChild->Bet[2] <= 100) ||
						(pChild->Bet[3] <= 100))
					{
						if (DingFen[index] > 0)
							g_DingFen1_FX = 1;
					}
				}
			}
		}
		// end+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		break;
	case 2:
		total_bet = ((all_bet[0] + all_bet[1] + all_bet[3] + dec_bet) * 100) / bei; // 最高可以押多少分
		bet = (all_bet[2] >= total_bet) ? 0 : total_bet - all_bet[2];				// 还能押多少
		if (bet >= max_bet)
			bet = 0;
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		if (bet == 0 && g_AiDingfenSw) // 说明本门已经押满。
		{
			if ((pai & 0x0f) != 0x00 &&
				(pai & 0x0f) <= 0x0D &&
				(pai & 0x30) == 0x20 &&
				pChild->Bet[2] > (dec_bet / 3))
			{
				if ((pChild->Bet[0] > pChild->Bet[2]) ||
					(pChild->Bet[1] > pChild->Bet[2]) ||
					(pChild->Bet[3] > pChild->Bet[2]))
				{
					if ((pChild->Bet[0] <= 100) ||
						(pChild->Bet[1] <= 100) ||
						(pChild->Bet[3] <= 100))
					{
						if (DingFen[index] > 0)
							g_DingFen1_FX = 1;
					}
				}
			}
		}
		// end+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		break;
	case 3:
		total_bet = ((all_bet[0] + all_bet[1] + all_bet[2] + dec_bet) * 100) / bei; // 最高可以押多少分
		bet = (all_bet[3] >= total_bet) ? 0 : total_bet - all_bet[3];				// 还能押多少
		if (bet >= max_bet)
			bet = 0;
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		if (bet == 0 && g_AiDingfenSw) // 说明本门已经押满。
		{
			if ((pai & 0x0f) != 0x00 &&
				(pai & 0x0f) <= 0x0D &&
				(pai & 0x30) == 0x30 &&
				pChild->Bet[3] > (dec_bet / 3))
			{
				if ((pChild->Bet[0] > pChild->Bet[3]) ||
					(pChild->Bet[1] > pChild->Bet[3]) ||
					(pChild->Bet[2] > pChild->Bet[3]))
				{
					if ((pChild->Bet[0] <= 100) ||
						(pChild->Bet[1] <= 100) ||
						(pChild->Bet[2] <= 100))
					{
						if (DingFen[index] > 0)
							g_DingFen1_FX = 1;
					}
				}
			}
		}
		// end+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		break;
	case 4:
		max_bet = ((dec_bet) * 100) / (g_NewMul[0][index]); // 最高可以押多少分
		bet = (all_bet[4] >= max_bet) ? 0 : max_bet - all_bet[4];
		break;
	default:
		break;
	}

#ifdef __MINGPAI_BET__
	// 只能押上一次的分数
	if (g_pSram->MingPaiFlag > 0)
	{
		uint32_t have_bet;
		pChild = &Child[ChildNum];
		if (g_pSram->MingPaiFlag == 2)
		{
			pai = ((pai & 0x0f) >= 0x0e) ? 4 : (pai & 0x30) >> 4;
			if (index != pai)
				return 0;

			if (g_OldBet[ChildNum][index] > 0 && bet > 0)
			{
				if (g_OldBet[ChildNum][index] > pChild->Bet[index])
				{
					max_bet = g_OldBet[ChildNum][index] - pChild->Bet[index];
					if (bet >= max_bet)
						bet = max_bet;
				}
				else
				{
					bet = 0;
				}
			}
			else
			{
				bet = 0;
			}
		}
		else if (g_pSram->MingPaiFlag == 1) // 押红黑
		{
			uint32_t have_bet;
			uint32_t old_bet;
			pai = ((pai & 0x0f) >= 0x0e) ? 4 : ((pai & 0x30) >> 4) & 0x01;
			if (index == 4 || pai == 4)
				return 0;
			if ((index & 0x01) != pai)
				return 0;

			if (pai > 0)
			{ // 红
				have_bet = pChild->Bet[1] + pChild->Bet[3];
				old_bet = g_OldBet[ChildNum][1] + g_OldBet[ChildNum][3];
			}
			else
			{ // 黑
				have_bet = pChild->Bet[0] + pChild->Bet[2];
				old_bet = g_OldBet[ChildNum][0] + g_OldBet[ChildNum][2];
			}
			if (old_bet > (dec_bet * 100 / bei))
				old_bet = dec_bet * 100 / bei;

			if (old_bet > 0 && bet > 0)
			{
				if (old_bet > have_bet)
				{
					max_bet = old_bet - have_bet;
					if (bet >= max_bet)
						bet = max_bet;
				}
				else
				{
					bet = 0;
				}
			}
			else
			{
				bet = 0;
			}
		}
	}
#endif
	return bet;
}
void BetKeyProc1(ChildType *pChild)
{
	if ((int32_t)pChild->Win > 0)
	{
		pChild->Credit += pChild->Win;
		pChild->Win = 0;
		return;
	}
	if (ChildKeyXiOrTui(pChild))
		return;
	if (ChildKeyCan(pChild))
	{
		ChildCancel(pChild);
	}
	{
		uint32_t tmp;
		uint16_t bet, old, max_bet;
		tmp = 0;
		if (ChildKeyBET0(pChild))
			goto bet_ok;
		tmp = 1;
		if (ChildKeyBET1(pChild))
			goto bet_ok;
		tmp = 2;
		if (ChildKeyBET2(pChild))
			goto bet_ok;
		tmp = 3;
		if (ChildKeyBET3(pChild))
			goto bet_ok;
		tmp = 4;
		if (ChildKeyBET4(pChild))
			goto bet_ok;
		return;
	bet_ok:
		bet = GetConv(pChild), max_bet = ChildMaxBET(pChild, tmp), old = pChild->Bet[tmp];
		// max_bet 是当前还能押多少分  old 是已经押了多少分
		if (old + bet <= GetMinbet(&DipSw))
			bet = GetMinbet(&DipSw) - old;
		if (bet > max_bet)
			bet = max_bet;
		if (pChild->Credit < bet)
			bet = pChild->Credit;
		if (bet + old > GetMaxbet(&DipSw))
			bet = (GetMaxbet(&DipSw) >= old) ? (GetMaxbet(&DipSw) - old) : 0;

		if (bet)
		{
			pChild->Credit -= bet;
			pChild->Bet[tmp] += bet;
		}
	}
}
