#ifndef __GAMESET_H__
#define __GAMESET_H__
#include "includes.h"

enum
{
    INIT_WAIT = 0, // 待机画面
    MAIN_MUNE,     // 主菜单
    AUDIT_LOOK,    // 查账
    AUDIT_INPUT,   // 报账
    CONFIG_CRT1,   // 参数调整
    CONFIG_CRT2,   // 系统设置
    CONFIG_CRT3,   // 功能设置
    SET_QIU,       // 设置球位
    INPUT_PASS,    // 输入密码
    SETBONUS,      // 设置头彩金
    OPENDOOR
};

typedef struct
{
    uint8_t GameIndex;    // 主菜单索引
    uint8_t Oldpage;      // 上一页
    uint8_t Oldctr;       // 上一控件索引
    uint8_t page;         // 从HMI接收的当前页码
    uint8_t ctr;          // 从HMI接收到的按键值
    char data[30];        // 从HMI接收到的文本数据
    uint32_t password;    // 转化数值或密码
    uint32_t xuleihao;    // 序列号
    uint32_t gamestation; // 游戏状态
    uint32_t line_num;    // 设置线号
    uint32_t machine_id;  // 设置机号
    uint8_t SetVolume;    // 设置音量
    uint8_t save_flg;
    uint8_t rx_ok_flg;
    uint8_t delay;
    uint8_t dim;
} HANDTYPE;

struct GameSetRx
{
    char RxBuf[50];
    uint8_t Res;
    uint8_t RxCnt;
    uint8_t RxendCnt;
    uint8_t Rx_ok_flg;
    char TxBuf[50];
    uint8_t TxCnt;
    uint8_t TxLen;
    uint8_t Tx_busy_flg;
};

void HAL_UART_RxGameSet(UART_HandleTypeDef *huart);
extern struct GameSetRx *Gamesetrx;
void Init_UartSet(void);
void GameSetProc(HANDTYPE *handpoint);
#endif
