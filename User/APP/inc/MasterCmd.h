#ifndef __MASTER_CMD_H__
#define __MASTER_CMD_H__
#include "includes.h"

#pragma pack(1)
typedef struct MasterCmdType
{
    uint8_t procmd;                     // ��Ϸ����
    uint8_t gametime;                   // ��Ϸʱ��
    uint16_t totalgame;                 // ��Ϸ����
    uint16_t gamecount;                 // ��Ϸ����
    uint32_t linebonus;                 // ��Ϸ����ʽ�
    uint8_t result1;                    // ��Ϸ���
    uint8_t Luck;                       // ��Ϸ���˽�
    uint16_t Conv;                      // �޺�
    uint16_t hei_cnt;                   // �������1
    uint16_t hong_cnt;                  // �������2
    uint16_t mei_cnt;                   // �������3
    uint16_t fang_cnt;                  // �������4
    uint16_t wang_cnt;                  // �������5
    uint16_t childbet[BET_NUMBER];      // 10
    uint32_t childTotalbet[BET_NUMBER]; //
    uint32_t gamestation;               // ��Ϸ״̬
    uint16_t checksum;                  // У���
} MasterCmdType;

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
