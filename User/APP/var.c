#include "var.h"

//------------------------------------------------------
// ��Ϸ����
//------------------------------------------------------
uint32_t __24C04_FLAG;  // ii2c save flg
DipSwType DipSw;        // ��Ϸ����
DipSwType SetDipSw;     // ��Ϸ��ʱ����
uint32_t Machine_line;  // ��̨�ߺ�
uint32_t Machine_id;    // ��̨̨��
char FutoryTime[10];    // ��������
char ProgramVel[10];    // ����汾
int64_t Prevpayoff;     // ǰ��ӯ��
int64_t CurrPayoff;     // ����ӯ��
int32_t AccoutPassWord; // ��������
int32_t ConfigPassWord; // ��������
int32_t SystemPassWord; // ϵͳ����
int32_t FuncPassWord;   // ��������
uint8_t Qiuwei[10];     // ������λ
//------------------------------------------------------
// ��Ϸ��������ʹ��
//------------------------------------------------------
char SetFutoryTime[10];
char SetProgramVel[10];
int32_t SetAccoutPassWord;
int32_t SetConfigPassWord;
int32_t SetSystemPassWord;
//-----------------------------------------------------
// ������λʹ�ñ���
//-----------------------------------------------------
uint8_t SetQiuwei[10]; //
uint8_t ChuQiuHao;     // ������λ
uint8_t QiuTest;       // �������
uint8_t QiuState;
uint8_t SwtQiuFlag;
//---------------------------------------------------------
// ϵͳʱ����ر���
//---------------------------------------------------------
TimeType SysTime; // ϵͳʱ��
TimeType SetTime; // ����ʱ��
//------------------------------------------------------
// ��Ϸ����
//------------------------------------------------------
uint8_t GameTime;                             // ��Ϸʱ��
uint16_t TotalGame;                           // ��Ϸ����
uint16_t GameCount;                           // ��Ϸ����
uint16_t DispGameCount;                       // ��ʾ��Ϸ����
uint16_t ChildTotalGame;                      // �����ʾ����
uint16_t ChildGameCount;                      // �����ʾ����
uint32_t LineBonus;                           // ����ʽ�
uint32_t MaxBonus;                           // ͷ����ʽ�
uint32_t MidBonus;                           // ͷ���вʽ�
uint32_t MinBonus;                           // ͷ��С�ʽ�
uint8_t GamePai;                              // ��Ϸ���
uint8_t MingPai;                              // ����״̬��־
uint8_t Luck;                                 // ��Ϸ���˽�
uint8_t Luck_CS;
uint8_t LuckBuf[TOTAL_JUSHU];                // ��Ϸ���˽�BUF
uint8_t LuckFunBuf[10];                       // ��Ϸ���˽���Ŀ
LuckJiangType LuckJiang;                   // ���˽�������

uint32_t DingFen[MAX_FENJI];                  //
uint8_t New_Bill[TOTAL_JUSHU * TOTAL_LUNSHU]; //
//------------------------------------------------------
//  ��Ϸ����
//------------------------------------------------------
uint16_t HeiCnt;
uint16_t HongCnt;
uint16_t CaoCnt;
uint16_t FangCnt;
uint16_t WangCnt;
uint8_t History[TOTAL_JUSHU];                   //
uint8_t LuckHistory[TOTAL_JUSHU];
//------------------------------------------------------
// �ֻ�����
//------------------------------------------------------
uint16_t ChildTotalGame;
uint16_t ChildGameCount;
ChildType Child[MAX_FENJI]  BKPSRAM;
uint32_t Sramflg BKPSRAM;
//------------------------------------------------------
// ��Ϸ״̬����
//------------------------------------------------------
uint8_t ProcMem;      // ��Ϸ����
uint8_t OldProcMem;
uint32_t Gamestation; // ��Ϸ״̬

//------------------------------------------------------
// ���⹦�ܱ���
//------------------------------------------------------
uint8_t g_AiWaterSw = 0;       // �Զ���ˮ
uint8_t g_AiDingfenSw = 0;     // �Զ�����
uint8_t g_AiKeyLudanSw = 0;    // �Զ���·��
uint8_t g_AiTongjiLudanSw = 0; // �Զ�ͳ��·��

uint8_t g_Random_Tab_Count = 0;
uint8_t g_Random_Count = 0;
uint8_t g_Random_Add = 0;
uint8_t g_Random_KeyRecode = 0;
uint8_t g_XiPai4_Count = 0;
uint8_t g_ChaPai_Count = 0;
uint8_t g_ChaPai_Pai = 0;
uint8_t g_ChaPai_Max = 0;

uint8_t g_SavePaiBet = 0;      // ������Ϸ����ϵ�Ѻ�֡�
uint8_t g_SavePaiBet0 = 0;     //
uint8_t g_DingFen1_KG = 1;     // ���ֿ��أ������Ǵ򿪵ġ�
uint8_t g_DingFen1_FX = 0;     // ���ֱ����ֵı�־��
uint8_t g_DingFen1_Random = 0; // ����ͣ���پֺ��ٿ�ʼ������
uint8_t g_DingFen1_Recode = 0; // ��¼����
