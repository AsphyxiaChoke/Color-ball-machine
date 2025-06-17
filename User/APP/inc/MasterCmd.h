#ifndef __MASTER_CMD_H__
#define __MASTER_CMD_H__
#include "includes.h"

#pragma pack(1)
typedef struct MasterCmdType
{
    uint8_t procmd;                     // 游戏流程
    uint8_t gametime;                   // 游戏时间
    uint16_t totalgame;                 // 游戏轮数
    uint16_t gamecount;                 // 游戏局数
    uint32_t linebonus;                 // 游戏随机彩金
    uint8_t result1;                    // 游戏结果
    uint8_t Luck;                       // 游戏幸运奖
    uint16_t Conv;                      // 限红
    uint16_t hei_cnt;                   // 结果计数1
    uint16_t hong_cnt;                  // 结果计数2
    uint16_t mei_cnt;                   // 结果计数3
    uint16_t fang_cnt;                  // 结果计数4
    uint16_t wang_cnt;                  // 结果计数5
    uint16_t childbet[BET_NUMBER];      // 10
    uint32_t childTotalbet[BET_NUMBER]; //
    uint32_t gamestation;               // 游戏状态
    uint16_t checksum;                  // 校验和
} MasterCmdType;

typedef struct MasterHistoryType
{
    uint8_t procmd;          // 游戏流程
    uint8_t historybuf[100]; // 历史记录或幸运奖项
    uint16_t checksum;       // 校验和
} MasterHistoryType;
#pragma pack()

void MasterRuncmd(uint8_t cmd);
void MasterRunsult(uint8_t cmd);
void MasterRunHistory(void);
void MasterRunLuckHistory(void);
/*


*/

#endif
