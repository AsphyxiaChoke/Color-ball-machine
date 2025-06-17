#include "ChildCmd.h"

ChildCmdTYPE *ChildCmdpoint;

uint8_t ChildReadResult()
{
    if (Luck & 0x06)
    {
        return GamePai & 0x3f;
    }
    return 0;
}

void ChildRuncmd(ChildType *pChild, uint8_t cmd, uint8_t childid)
{
    uint8_t i, j, *point;
    uint16_t checksum = 0;
    ChildCmdType cmdbuf;
    ChildDispType *pChildDisp;
    pChild->Packcnt++;
    cmdbuf.procmd = cmd;
    cmdbuf.gametime = GameTime;
    cmdbuf.totalgame = ChildTotalGame;
    cmdbuf.gamecount = ChildGameCount;
    cmdbuf.linebonus = LineBonus;
    cmdbuf.result = ChildReadResult();
    cmdbuf.Luck = Luck;
    cmdbuf.Conv = pChild->Conv;
    cmdbuf.Dec = GetDec(&DipSw);
    cmdbuf.childcredit = pChild->Credit;
    cmdbuf.childwin = pChild->Win;
    cmdbuf.hei_cnt = HeiCnt;
    cmdbuf.hong_cnt = HongCnt;
    cmdbuf.mei_cnt = CaoCnt;
    cmdbuf.fang_cnt = FangCnt;
    cmdbuf.wang_cnt = WangCnt;
    for (i = 0; i < BET_NUMBER; i++)
    {
        cmdbuf.childbet[i] = pChild->Bet[i];
        cmdbuf.childTotalbet[i] = 0;
        for (j = 0; j < MAX_FENJI; j++)
        {
            cmdbuf.childTotalbet[i] += Child[j].Bet[i];
        }
    }
    cmdbuf.childid = childid;
    cmdbuf.gamestation = Gamestation;
    for (i = 0, point = (uint8_t *)&cmdbuf; i < sizeof(ChildCmdType) - 2; i++)
    {
        register uint8_t ch;
        ch = *point;
        checksum += ch;
        point++;
    }
    cmdbuf.checksum = checksum;
    pChildDisp = pChild->pChildDisp;
    pChildDisp->PageStart = pChild->Packcnt;
    pChildDisp->Cmd = 1;
    pChildDisp->Len = sizeof(ChildCmdType);
    memcpy(pChildDisp->Data, (void *)&cmdbuf, sizeof(ChildCmdType));
    pChildDisp->Pageend = pChild->Packcnt;
}

void ChildRunresult(ChildType *pChild, uint8_t cmd, uint8_t childid)
{
    uint8_t i, j, *point;
    uint16_t checksum = 0;
    ChildCmdType cmdbuf;
    ChildDispType *pChildDisp;
    pChild->Packcnt++;
    cmdbuf.procmd = cmd;
    cmdbuf.gametime = GameTime;
    cmdbuf.totalgame = ChildTotalGame;
    cmdbuf.gamecount = ChildGameCount;
    cmdbuf.linebonus = LineBonus;
    cmdbuf.result = GamePai;
    cmdbuf.Luck = Luck;
    cmdbuf.Conv = pChild->Conv;
    cmdbuf.Dec = GetDec(&DipSw);
    cmdbuf.childcredit = pChild->Credit;
    cmdbuf.childwin = pChild->Win;
    cmdbuf.hei_cnt = HeiCnt;
    cmdbuf.hong_cnt = HongCnt;
    cmdbuf.mei_cnt = CaoCnt;
    cmdbuf.fang_cnt = FangCnt;
    cmdbuf.wang_cnt = WangCnt;
    for (i = 0; i < BET_NUMBER; i++)
    {
        cmdbuf.childbet[i] = pChild->Bet[i];
        cmdbuf.childTotalbet[i] = 0;
        for (j = 0; j < MAX_FENJI; j++)
        {
            cmdbuf.childTotalbet[i] += Child[j].Bet[i];
        }
    }
    cmdbuf.childid = childid;
    cmdbuf.gamestation = Gamestation;
    for (i = 0, point = (uint8_t *)&cmdbuf; i < sizeof(ChildCmdType) - 2; i++)
    {
        register uint8_t ch;
        ch = *point;
        checksum += ch;
        point++;
    }
    cmdbuf.checksum = checksum;
    pChildDisp = pChild->pChildDisp;
    pChildDisp->PageStart = pChild->Packcnt;
    pChildDisp->Cmd = 1;
    pChildDisp->Len = sizeof(ChildCmdType);
    memcpy(pChildDisp->Data, (void *)&cmdbuf, sizeof(ChildCmdType));
    pChildDisp->Pageend = pChild->Packcnt;
}

void ChildRunHistory(ChildType *pChild)
{
    uint8_t i, *point;
    uint16_t checksum = 0;
    ChildHistoryType cmdbuf;
    ChildDispType *pChildDisp;
    pChild->Packcnt++;
    cmdbuf.procmd = PC_HISTORY;
    memcpy(cmdbuf.historybuf, History, sizeof(cmdbuf.historybuf));
    for (i = 0, point = (uint8_t *)&cmdbuf; i < sizeof(ChildHistoryType) - 2; i++)
    {
        register uint8_t ch;
        ch = *point;
        checksum += ch;
        point++;
    }
    cmdbuf.checksum = checksum;
    pChildDisp = pChild->pChildDisp;
    pChildDisp->PageStart = pChild->Packcnt;
    pChildDisp->Cmd = 1;
    pChildDisp->Len = sizeof(ChildHistoryType);
    memcpy(pChildDisp->Data, (void *)&cmdbuf, sizeof(ChildHistoryType));
    pChildDisp->Pageend = pChild->Packcnt;
}

void ChildRunLuckHistory(ChildType *pChild)
{
    uint8_t i, *point;
    uint16_t checksum = 0;
    ChildHistoryType cmdbuf;
    ChildDispType *pChildDisp;
    pChild->Packcnt++;
    cmdbuf.procmd = PC_LUCKHISTORY;
    memcpy(cmdbuf.historybuf, LuckHistory, sizeof(cmdbuf.historybuf));
    for (i = 0, point = (uint8_t *)&cmdbuf; i < sizeof(ChildHistoryType) - 2; i++)
    {
        register uint8_t ch;
        ch = *point;
        checksum += ch;
        point++;
    }
    cmdbuf.checksum = checksum;
    pChildDisp = pChild->pChildDisp;
    pChildDisp->PageStart = pChild->Packcnt;
    pChildDisp->Cmd = 1;
    pChildDisp->Len = sizeof(ChildHistoryType);
    memcpy(pChildDisp->Data, (void *)&cmdbuf, sizeof(ChildHistoryType));
    pChildDisp->Pageend = pChild->Packcnt;
}

void ReadChildcmd(ChildCmdTYPE *cmdpoint)
{
    if (cmdpoint->Cmdstart != cmdpoint->Cmdend)
    {
        cmdpoint->Cmd = cmdpoint->CmdBuf[cmdpoint->Cmdend++];
        cmdpoint->Cmdend &= 0x03;
    }
}

void SendChild(ChildType *pChild, ChildCmdTYPE *cmdpoint, uint8_t childid)
{
    switch (cmdpoint->Cmd)
    {
    case PC_RESTART:
    case PC_GAMESTART:
    case PC_STARTBET:
    case PC_GAMETIME:
    case PC_STOPBET:
        ChildRuncmd(pChild, cmdpoint->Cmd, childid);
        break;
    case PC_RESULT:
    case PC_STARTWIN:
    case PC_STARTBONUS:
    case PC_STOPBONUS:
    case PC_STOPWIN:
    case PC_GAMEEND:
        ChildRunresult(pChild, cmdpoint->Cmd, childid);
        break;
    case PC_HISTORY:
        ChildRunHistory(pChild);
        break;
    case PC_LUCKHISTORY:
        ChildRunLuckHistory(pChild);
        break;
    }
}

//-------------------------------------------------
// ÉèÖÃÒª·¢ËÍÃüÁî
//-------------------------------------------------
void SetChildcmd(ChildCmdTYPE *cmdpoint, uint8_t cmd)
{
	uint32_t time;
	OS_ERR err;
	cmdpoint->CmdBuf[cmdpoint->Cmdstart++] = cmd;
	cmdpoint->Cmdstart &= 0x03;
	for (time = OSTimeGet(&err) + 4; time > OSTimeGet(&err);)
	{
		BackProcess();
		CoinKeyProc();
	}
}
