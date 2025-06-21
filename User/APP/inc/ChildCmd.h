#ifndef __CHILDCMD_H__
#define __CHILDCMD_H__
#include "includes.h"

#pragma pack(1)
typedef struct ChildCmdType
{
    uint8_t procmd;                     // ��Ϸ����--1
    uint8_t gametime;                   // ��Ϸʱ��--2
    uint16_t totalgame;                 // ��Ϸ����--3��4
    uint16_t gamecount;                 // ��Ϸ����--5��6
    uint32_t linebonus;                 // ��Ϸ����ʽ�--7��8��9��10  
    uint8_t result;                     // ��Ϸ���--11
    uint8_t Luck;                       // ��Ϸ���˽�--12
    uint16_t Conv;                      // �л�--13��14
    uint32_t Dec;                       // �޺�--15��16��17��18
    uint16_t hei_cnt;                   // �������1--19��20
    uint16_t hong_cnt;                  // �������2--21��22
    uint16_t mei_cnt;                   // �������3--23��24 
    uint16_t fang_cnt;                  // �������4--25��26
    uint16_t wang_cnt;                  // �������5--27��28
    uint32_t childcredit;               // �ֻ��ܷ�--29��30��31��32
    uint32_t childwin;                  // �ֻ�Ӯ��--33��34��35��36
    uint16_t childbet[BET_NUMBER];      // 5*2=10--37��... ,46
    uint32_t childTotalbet[BET_NUMBER]; // 5*4=20--47, ... ,66
    uint8_t childid;                    // �ֻ���--67
    uint32_t gamestation;               // ��Ϸ״̬--68,69,70,71
    uint32_t minbonus;                 // ��Ϸ����ʽ�--72,73,74,75
    uint32_t midbonus;                 // ��Ϸ����ʽ�--76��77��78��79
    uint32_t maxbonus;                 // ��Ϸ����ʽ�--80��81��82��83
    uint32_t childwin2;                 //�ֻ������н���Ӯ��--84��85��86��87
    uint16_t checksum;                  // У���--88,89
}  ChildCmdType;
typedef struct ChildHistoryType
{
    uint8_t procmd;          // ��Ϸ����--1
    uint8_t historybuf[100]; // ��ʷ��¼�����˽���--2, ... ,101
    uint16_t checksum;       // У���--102,103
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
