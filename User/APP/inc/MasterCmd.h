#ifndef __MASTER_CMD_H__
#define __MASTER_CMD_H__
#include "includes.h"

#pragma pack(1)
typedef struct MasterCmdType
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
} MasterCmdType;
/*typedef struct MasterCmdType
{
    uint8_t procmd;                     // ��Ϸ����--1
    uint8_t gametime;                   // ��Ϸʱ��--2
    uint16_t totalgame;                 // ��Ϸ����--3,4
    uint16_t gamecount;                 // ��Ϸ����--5,6
    uint32_t linebonus;                 // ��Ϸ����ʽ�--7,8,9,10
    uint8_t result1;                    // ��Ϸ���--11
    uint8_t Luck;                       // ��Ϸ���˽�--12
    uint16_t Conv;                      // �޺�--13,14
    uint16_t hei_cnt;                   // �������1--15,16
    uint16_t hong_cnt;                  // �������2--17,18
    uint16_t mei_cnt;                   // �������3--19,20
    uint16_t fang_cnt;                  // �������4--21,22
    uint16_t wang_cnt;                  // �������5--23,24
    uint16_t childbet[BET_NUMBER];      // 10--25...34
    uint32_t childTotalbet[BET_NUMBER]; // 20--35...54
    uint32_t gamestation;               // ��Ϸ״̬--55,56,57,58
    uint16_t checksum;                  // У���--59,60
} MasterCmdType;*/

typedef struct MasterHistoryType
{
    uint8_t procmd;          // ��Ϸ����
    uint8_t historybuf[100]; // ��ʷ��¼�����˽���
    uint16_t checksum;       // У���
} MasterHistoryType;
#pragma pack()

void MasterRuncmd(uint8_t cmd);
void MasterRunsult(uint8_t cmd);
void MasterRunHistory(void);
void MasterRunLuckHistory(void);
/*


*/

#endif
