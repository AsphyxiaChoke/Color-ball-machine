#ifndef __VAR_H__
#define __VAR_H__
#include "includes.h"


//------------------------------------------------------
//
//------------------------------------------------------
#define __random() read_random()
#define BKPSRAM __attribute__((section("BKP_SRAM_SECTION")))

#define PlayPaiBuf (&New_Bill[(TotalGame % 20) * 100])
#define OS_TICKS_PER_SEC OS_CFG_TICK_RATE_HZ
//------------------------------------------------------
// ��Ϸ����
//------------------------------------------------------
extern uint32_t __24C04_FLAG;  // ii2c save flg
extern DipSwType DipSw;        // ��Ϸ����
extern DipSwType SetDipSw;     // ��Ϸ��ʱ����
extern uint32_t Machine_line;  // ��̨�ߺ�
extern uint32_t Machine_id;    // ��̨̨��
extern char FutoryTime[10];    // ��������
extern char ProgramVel[10];    // ����汾
extern int64_t Prevpayoff;     // ǰ��ӯ��
extern int64_t CurrPayoff;     // ����ӯ��
extern int32_t AccoutPassWord; // ��������
extern int32_t ConfigPassWord; // ��������
extern int32_t SystemPassWord; // ϵͳ����
extern int32_t FuncPassWord;   // ��������
extern uint8_t Qiuwei[10];     // ������λ
//------------------------------------------------------
// ��Ϸ��������ʹ��
//------------------------------------------------------
extern char SetFutoryTime[10];
extern char SetProgramVel[10];
extern int32_t SetAccoutPassWord;
extern int32_t SetConfigPassWord;
extern int32_t SetSystemPassWord;
//-----------------------------------------------------
// ������λʹ�ñ���
//-----------------------------------------------------
extern uint8_t SetQiuwei[10]; // ������λ
extern uint8_t ChuQiuHao;     // ������λ
extern uint8_t QiuTest;       // �������QiuText
extern uint8_t QiuState;
extern uint8_t SwtQiuFlag;
//---------------------------------------------------------
// ϵͳʱ����ر���
//---------------------------------------------------------
extern TimeType SysTime; // ϵͳʱ��
extern TimeType SetTime; // ����ʱ��
//------------------------------------------------------
// ��Ϸ����
//------------------------------------------------------
extern uint8_t GameTime;
extern uint16_t TotalGame;
extern uint16_t GameCount;
extern uint16_t DispGameCount;  // ��ʾ��Ϸ����
extern uint16_t ChildTotalGame; // �����ʾ����
extern uint16_t ChildGameCount; // �����ʾ����
extern uint32_t LineBonus;
extern uint32_t MaxBonus;                           // ͷ����ʽ�
extern uint32_t MidBonus;                           // ͷ���вʽ�
extern uint32_t MinBonus;                           // ͷ��С�ʽ�
extern uint8_t GamePai;
extern uint8_t MingPai;                              // ����״̬��־
extern uint8_t Luck;
extern uint8_t Luck_CS;
extern uint8_t LuckBuf[TOTAL_JUSHU];                // ��Ϸ���˽�BUF
extern uint8_t LuckFunBuf[10];                       // ��Ϸ���˽���Ŀ
extern LuckJiangType LuckJiang;

extern uint32_t DingFen[MAX_FENJI];
extern uint8_t New_Bill[TOTAL_JUSHU * TOTAL_LUNSHU]; //
//------------------------------------------------------
// ��Ϸ����
//------------------------------------------------------
extern uint16_t HeiCnt;
extern uint16_t HongCnt;
extern uint16_t CaoCnt;
extern uint16_t FangCnt;
extern uint16_t WangCnt;
extern uint8_t History[TOTAL_JUSHU];
extern uint8_t LuckHistory[TOTAL_JUSHU];
//------------------------------------------------------
// �ֻ�����
//------------------------------------------------------
extern uint16_t ChildTotalGame;
extern uint16_t ChildGameCount;
extern ChildType Child[MAX_FENJI];
extern uint32_t Sramflg;
//------------------------------------------------------
// ��Ϸ״̬����
//------------------------------------------------------
extern uint8_t ProcMem; // ��Ϸ����
extern uint8_t OldProcMem; // ��Ϸ����
extern uint32_t Gamestation;

//------------------------------------------------------
// ���⹦�ܱ���
//------------------------------------------------------
extern uint8_t g_AiWaterSw;       // �Զ���ˮ
extern uint8_t g_AiDingfenSw;     // �Զ�����
extern uint8_t g_AiKeyLudanSw;    // �Զ���·��
extern uint8_t g_AiTongjiLudanSw; // �Զ�ͳ��·��

extern uint8_t g_Random_Tab_Count;
extern uint8_t g_Random_Count;
extern uint8_t g_Random_Add;
extern uint8_t g_Random_KeyRecode;
extern uint8_t g_XiPai4_Count;
extern uint8_t g_ChaPai_Count;
extern uint8_t g_ChaPai_Pai;
extern uint8_t g_ChaPai_Max;

extern uint8_t g_SavePaiBet;      // ������Ϸ����ϵ�Ѻ�֡�
extern uint8_t g_SavePaiBet0;     //
extern uint8_t g_DingFen1_KG;     // ���ֿ��أ������Ǵ򿪵ġ�
extern uint8_t g_DingFen1_FX;     // ���ֱ����ֵı�־��
extern uint8_t g_DingFen1_Random; // ����ͣ���پֺ��ٿ�ʼ������
extern uint8_t g_DingFen1_Recode; // ��¼����
#endif
