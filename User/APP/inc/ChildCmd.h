#ifndef __CHILDCMD_H__
#define __CHILDCMD_H__
#include "includes.h"

#pragma pack(1)
typedef struct ChildCmdType
{
    uint8_t procmd;                     // 游戏流程--1
    uint8_t gametime;                   // 游戏时间--2
    uint16_t totalgame;                 // 游戏轮数--3，4
    uint16_t gamecount;                 // 游戏局数--5，6
    uint32_t linebonus;                 // 游戏随机彩金--7，8，9，10  
    uint8_t result;                     // 游戏结果--11
    uint8_t Luck;                       // 游戏幸运奖--12
    uint16_t Conv;                      // 切换--13，14
    uint32_t Dec;                       // 限红--15，16，17，18
    uint16_t hei_cnt;                   // 结果计数1--19，20
    uint16_t hong_cnt;                  // 结果计数2--21，22
    uint16_t mei_cnt;                   // 结果计数3--23，24 
    uint16_t fang_cnt;                  // 结果计数4--25，26
    uint16_t wang_cnt;                  // 结果计数5--27，28
    uint32_t childcredit;               // 分机总分--29，30，31，32
    uint32_t childwin;                  // 分机赢分--33，34，35，36
    uint16_t childbet[BET_NUMBER];      // 5*2=10--37，... ,46
    uint32_t childTotalbet[BET_NUMBER]; // 5*4=20--47, ... ,66
    uint8_t childid;                    // 分机号--67
    uint32_t gamestation;               // 游戏状态--68,69,70,71
    uint32_t minbonus;                 // 游戏随机彩金--72,73,74,75
    uint32_t midbonus;                 // 游戏随机彩金--76，77，78，79
    uint32_t maxbonus;                 // 游戏随机彩金--80，81，82，83
    uint32_t childwin2;                 //分机二次中奖，赢分--84，85，86，87
    uint16_t checksum;                  // 校验和--88,89
}  ChildCmdType;
typedef struct ChildHistoryType
{
    uint8_t procmd;          // 游戏流程--1
    uint8_t historybuf[100]; // 历史记录或幸运奖项--2, ... ,101
    uint16_t checksum;       // 校验和--102,103
} ChildHistoryType;
#pragma pack()

typedef struct
{
    uint8_t CmdBuf[4];
    uint8_t Cmdstart;
    uint8_t Cmdend;
    uint8_t Cmd;
}ChildCmdTYPE;

extern ChildCmdTYPE *ChildCmdpoint;
void ReadChildcmd(ChildCmdTYPE *cmdpoint);
void SendChild(ChildType *pChild, ChildCmdTYPE *cmdpoint, uint8_t childid);
void SetChildcmd(ChildCmdTYPE *cmdpoint, uint8_t cmd);

#endif
