#ifndef __GAMESET_H__
#define __GAMESET_H__
#include "includes.h"

enum
{
    INIT_WAIT = 0, // ��������
    MAIN_MUNE,     // ���˵�
    AUDIT_LOOK,    // ����
    AUDIT_INPUT,   // ����
    CONFIG_CRT1,   // ��������
    CONFIG_CRT2,   // ϵͳ����
    SET_QIU,       // ������λ
    INPUT_PASS,    // ��������
    SETBONUS,      // ����ͷ�ʽ�
    OPENDOOR
};

typedef struct
{
    uint8_t GameIndex;    // ���˵�����
    uint8_t Oldpage;      // ��һҳ
    uint8_t Oldctr;       // ��һ�ؼ�����
    uint8_t page;         // ��HMI���յĵ�ǰҳ��
    uint8_t ctr;          // ��HMI���յ��İ���ֵ
    char data[30];        // ��HMI���յ����ı�����
    uint32_t password;    // ת����ֵ������
    uint32_t xuleihao;    // ���к�
    uint32_t gamestation; // ��Ϸ״̬
    uint32_t line_num;    // �����ߺ�
    uint32_t machine_id;  // ���û���
    uint8_t SetVolume;    // ��������
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
