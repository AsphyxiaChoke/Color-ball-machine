#ifndef __MOMORY_H__
#define __MOMORY_H__
#include "includes.h"

#define __DEBUG__

#ifdef  __DEBUG__
#define _XTMSG uartprintf
#endif

#define MAX_FENJI 8
#define BET_NUMBER 5
#define TOTAL_LUNSHU 20
#define TOTAL_JUSHU 100
//------------------------------------------------------------
// �ֻ���־
//------------------------------------------------------------
#define COIN_TIME_SPEED 100 // �˱Ҽ��
#define CHILDFLG 0x1a       // �ֻ���־

#define XI_FLAG (1UL << 0)
#define HIDE_FLAG (1UL << 1)
#define QUICK_WIN (1UL << 2)
#define COMPUTER (1UL << 3)
#define QUICK_BONUS (1UL << 4)
#define WINLAMP_FLAG (1UL << 6)
#define LOCK_FLAG (1UL << 8) /*Child[0].Flag ��־*/
#define TUI_FLAG (1UL << 9)
#define SAVE_WIN (1UL << 10)

#define SSR_OPEN (0x10) // �ֻ��˱���￪��λ
//-------------------------------------------------------------
// ��Ϸ���ܱ�־λ����
//-------------------------------------------------------------
#define BONUS_FLG (1UL << 0)    // �ʽ���
#define FANBEI_FLG (1UL << 1)   // �ӱ�����
#define MINGPAI_FLG (1UL << 2)  // ���ƹ���
#define JIFEN_FLG (1UL << 3)    // ���ֹ���
#define LUCKBIT_FLG (1UL << 4)  // ����λ����
#define ALLWIN_FLG (1UL << 5)   // ȫ�й���
#define SONGDENG_FLG (1UL << 6) // �͵ƹ���
#define PLAYING_FLG (1UL << 7)  // ��ʾ����
#define TOUCAI_FLG (1UL << 8)    // �˱ҹ���
//-------------------------------------------------------------
// II2C save data flg
//-------------------------------------------------------------
#define S_DIP (1UL << 0)         // ��Ϸ����
#define S_ACCOUTPASS (1UL << 1)  // ��������
#define S_CONFIGPASS (1UL << 2)  // ��������
#define S_SYSTEMPASS (1UL << 3)  // ϵͳ����
#define S_FUTORYTIME (1UL << 4)  // ����ʱ��
#define S_PROGRAMVEL (1UL << 5)  // ��Ϸ�汾
#define S_MACHINELINE (1UL << 6) // �ߺ�
#define S_MACHINEID (1UL << 7)   // ̨��
#define S_QIUWEI (1UL << 8)      // ��λ
//---------------------------------------------------
// �������д���
//---------------------------------------------------
#define MACHERR01 (1UL << 0)    // ���˹���
#define MACHERR02 (1UL << 1)    // ñ�ӹ���
#define MACHERR03 (1UL << 2)    // �������
#define MACHERR04 (1UL << 3)    // �������
#define MACHERR05 (1UL << 4)    // �����̹���
#define MACHERR06 (1UL << 5)    // ��оδ����
#define MACHERR07 (1UL << 6)    // ��λ����
#define MACHERR08 (1UL << 7)    // �ſع���
#define MACHERR09 (1UL<<8) //




#pragma pack(1)
typedef struct sw_struct
{
    uint16_t NoteIn;          // ���ִ�С
    uint16_t NoteOut;         // �·ִ�С
    uint16_t CoinIn;          // Ͷ�ұ���
    uint16_t CoinOut;         // �˱ұ���
    uint16_t Minbet;          // ��СѺ��
    uint16_t Maxbet;          // ���Ѻ��
    uint32_t DecIndex;        // �޺�ѡ��
    uint8_t Playing;          // �޷���ʾ
    uint8_t RateIndex;        // ��·ѡ��
    uint8_t TimeIndex;        // ��Ϸʱ��
    uint32_t OverIndex;       // ��̨����
    uint32_t GameOver;        // ��������
    uint32_t MinBonus;        // ��С�ʽ�
    uint32_t MaxBonus;        // ���ʽ�
    uint8_t CoLtd;            // ��˾̨��־
    uint16_t FuctoryFunEn;    // ��˾������Ϸ����ѡ��
    uint16_t PlayFunEn;       // ��Ϸ���˹���ѡ��
    uint8_t LuckTimes;        // ���˴���
    uint8_t LuckIndex;        // ���˽�����  �ۼƵ����˽����еı���
    uint32_t MinJifen;        // ��С����
    uint32_t Jifenrate;       // �����ۼӱ���
    uint32_t Toucairate;     // ͷ�ʱ���
    uint32_t Maxlinelitbonus; // �������С�ʽ�
    uint32_t Minlinelitbonus; // ��С����С�ʽ�
    uint32_t Maxlinemidbonus; // ��������вʽ�
    uint32_t Minlinemidbonus; // ��С�����вʽ�
    uint32_t Maxlinebigbonus; // ������ߴ�ʽ�
    uint32_t Minlinebigbonus; // ��С���ߴ�ʽ�
} DipSwType;

// �ֻ��������ݽṹ�ܳ�128�ֽڣ�
typedef struct ChildDispType
{
    uint8_t reserve;      // ����
    uint8_t PageStart;    // ������
    uint8_t NewTuiFlag;   // �ֻ���֤��־
    uint8_t TuiBiCnt1;    // ��ֵΪ0����ÿ����һ���˱Ҵ�ֵ��1
    uint8_t TuiBiNumLow;  // ������Ҫ�˱������ĵ�8λ
    uint8_t TuiBiNumHigh; // ������Ҫ�˱������ĸ�8λ
    uint8_t TuiBiCnt2;    // ��TuiBiCnt1ֵһ��
    uint8_t CheckSum;     // ������ֵ��Ч���
    uint8_t PcConectCnt;  // PC���Ӽ�����
    uint8_t Count[3];     // �н��� 8*3=24 bit��
    uint8_t Cmd;          // ���͵���������
    uint8_t Len;          // ���ݳ���
    uint8_t Data[113];    // ���ݴ����ʼλ��
    uint8_t Pageend;      // ������
} ChildDispType;

// �ֻ��������ݽṹ
typedef struct ChildInputType
{
    uint8_t KeyCount; // �ֻ�Ӳ���ṩ�ļ��̼�����

    uint8_t TouNum1;
    uint8_t TuiNum1;
    uint8_t TouNum2;
    uint8_t TuiNum2;
    uint8_t CheckSum;

    uint8_t Key1;
    uint8_t Key2;
    uint8_t Key3;
    uint8_t Android_Key1;
    uint8_t Android_Key2;
    uint8_t Android_Key3;
    uint8_t Version1;
    uint8_t Version2;
    uint8_t Version3;
    uint8_t Version4;
} ChildInputType;

// ��Ϸ���̶���
enum GAME_PRO
{
    PC_RESTART = 1, // ��Ϸ���¿�ʼ
    PC_GAMESTART,   // ÿ����Ϸ��ʼ
    PC_STARTBET,    // ��ʼѺ��
    PC_GAMETIME,    // ��Ϸʱ��
    PC_STOPBET,     // ֹͣѺ��
    PC_RESULT,      // ��Ϸ���
    PC_STARTBONUS,  // ��ʼ���⽱���н���ʼ
    PC_STOPBONUS,   // �������⽱���н�����
    PC_STARTWIN,    // ��ʼ�н��÷�
    PC_STOPWIN,     // �����н�
    PC_GAMEEND,     // ��Ϸ����

    PC_HISTORY = 20, // ��ʷ��¼
    PC_LUCKHISTORY   // ��ʷ��¼����
};

typedef struct ChildType
{
    uint32_t Credit;             // 4
    uint32_t Win;                // 4
    uint32_t Win2;                // ���������⽱��һ��Ӯ��
    uint16_t Bet[BET_NUMBER];    // ��ǰ��ѹ�� //10
    uint16_t OldBet[BET_NUMBER]; // 10
    uint16_t Conv;               // Ѻ��ת��		//2
    uint32_t CoinIn;             // Ͷ���ܽ�		//4
    uint32_t CoinOut;            // �˱��ܳ�		//4
    uint32_t KeyIn;              // �����ܿ�		//4
    uint32_t KeyOut;             // ϴ����ϴ		//4

    uint32_t PlayCnt;
    uint32_t WinCnt;

    uint32_t Count; // �˱Ҵ����־	//4

    uint32_t Flag; // ����һ�������ݱ�־	//4

    uint32_t KeyLED;

    ChildDispType *pChildDisp;   // 4
    ChildInputType *pChildInput; // 4
    uint8_t ChildKeyCount[32];   // ��������		//32
    uint32_t ChildKeyPress;      // ����ֵ		//4

    uint8_t KeyCount; // ��Ӧ��pChildInput��KeyCount(�ֻ�Ӳ���ṩ�ļ��̼�����);//1

    uint32_t CoinInTmp;   // Ͷ��ʹ��,��ʾ����Ͷ�˶��ٸ���,������ֻ�ܶ�����д	//4
    uint32_t CoinInTmp2;  // Ͷ��ʹ��,��������ʹ��							//4
    uint32_t CoinOutTmp;  // �˱�ʹ��,��ʾ�������˶��ٸ���,������ֻ�ܶ�����д	//4
    uint32_t CoinOutTmp2; // �˱�ʹ��,��������ʹ��							//4
    int64_t CoinOutTime;  // �˱��ж�ʹ��,���ʱ��							//8
    uint8_t TuiBiNum;     // �˱�Ҫ�˸���
    int64_t Bonus;        // ��̨�ֻ���ʾ�Ĳʽ�								//8
    int64_t WinBonus;     // ��̨�ֻ����ж��ٲʽ�								//8
    uint32_t ResultWin;   // �������Ӯ��										//4

    uint64_t AllBet8; // ��Ѻ												//8
    uint64_t AllWin8; // ����												//8

    uint8_t TouInitCount; // Ͷ�ҳ�ʼ��������
    uint8_t TuiInitCount; // �˱ҳ�ʼ��������
    uint8_t TouOldCount;  // ��ǰͶ�Ҽ���
    uint8_t TuiOldCount;  // ��ǰ�˱Ҽ���
    uint8_t TuiProcCount; // �˱Ҵ������
    uint8_t TuiProcFlag;

    uint8_t CoinKeyCount;     // ��ֹ����ʱ��Ͷ��
    uint8_t InitCoinOut;      // Ͷ�˳�ʼ��
    uint8_t AgainInitCoinOut; // Ͷ�˳�ʼ��
    uint8_t DispCount;        // ��Ӧ��pChildDisp��Count(���);
    uint8_t PcConectCnt;      // ͨ�ż���											//1

    uint8_t Packcnt; // 1
} ChildType;

typedef struct
{
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t min;
    uint8_t sec;
    uint8_t mil;
} TimeType;

#pragma pack()
#endif
