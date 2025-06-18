#include "var.h"

//------------------------------------------------------
// 游戏参数
//------------------------------------------------------
uint32_t __24C04_FLAG;  // ii2c save flg
DipSwType DipSw;        // 游戏设置
DipSwType SetDipSw;     // 游戏临时设置
uint32_t Machine_line;  // 机台线号
uint32_t Machine_id;    // 机台台号
char FutoryTime[10];    // 出厂日期
char ProgramVel[10];    // 程序版本
int64_t Prevpayoff;     // 前期盈利
int64_t CurrPayoff;     // 现在盈利
int32_t AccoutPassWord; // 查账密码
int32_t ConfigPassWord; // 参数密码
int32_t SystemPassWord; // 系统密码
int32_t FuncPassWord;   // 功能密码
uint8_t Qiuwei[10];     // 机器球位
//------------------------------------------------------
// 游戏参数重设使用
//------------------------------------------------------
char SetFutoryTime[10];
char SetProgramVel[10];
int32_t SetAccoutPassWord;
int32_t SetConfigPassWord;
int32_t SetSystemPassWord;
//-----------------------------------------------------
// 设置球位使用变量
//-----------------------------------------------------
uint8_t SetQiuwei[10]; //
uint8_t ChuQiuHao;     // 亮出球位
uint8_t QiuTest;       // 亮球操作
uint8_t QiuState;
uint8_t SwtQiuFlag;
//---------------------------------------------------------
// 系统时间相关变量
//---------------------------------------------------------
TimeType SysTime; // 系统时间
TimeType SetTime; // 设置时间
//------------------------------------------------------
// 游戏变量
//------------------------------------------------------
uint8_t GameTime;                             // 游戏时间
uint16_t TotalGame;                           // 游戏轮数
uint16_t GameCount;                           // 游戏局数
uint16_t DispGameCount;                       // 显示游戏局数
uint16_t ChildTotalGame;                      // 玩家显示轮数
uint16_t ChildGameCount;                      // 玩家显示局数
uint32_t LineBonus;                           // 随机彩金
uint8_t GamePai;                              // 游戏结果
uint8_t Luck;                                 // 游戏幸运奖
uint8_t YK_Luck;
uint32_t DingFen[MAX_FENJI];                  //
uint8_t New_Bill[TOTAL_JUSHU * TOTAL_LUNSHU]; //
//------------------------------------------------------
//  游戏计数
//------------------------------------------------------
uint16_t HeiCnt;
uint16_t HongCnt;
uint16_t CaoCnt;
uint16_t FangCnt;
uint16_t WangCnt;
uint8_t History[TOTAL_JUSHU];                   //
uint8_t LuckHistory[TOTAL_JUSHU];
//------------------------------------------------------
// 分机变量
//------------------------------------------------------
uint16_t ChildTotalGame;
uint16_t ChildGameCount;
ChildType Child[MAX_FENJI]  BKPSRAM;
uint32_t Sramflg BKPSRAM;
//------------------------------------------------------
// 游戏状态变量
//------------------------------------------------------
uint8_t ProcMem;      // 游戏步骤
uint8_t OldProcMem;
uint32_t Gamestation; // 游戏状态

//------------------------------------------------------
// 特殊功能变量
//------------------------------------------------------
uint8_t g_AiWaterSw = 0;       // 自动放水
uint8_t g_AiDingfenSw = 0;     // 自动顶分
uint8_t g_AiKeyLudanSw = 0;    // 自动改路单
uint8_t g_AiTongjiLudanSw = 0; // 自动统计路单

uint8_t g_Random_Tab_Count = 0;
uint8_t g_Random_Count = 0;
uint8_t g_Random_Add = 0;
uint8_t g_Random_KeyRecode = 0;
uint8_t g_XiPai4_Count = 0;
uint8_t g_ChaPai_Count = 0;
uint8_t g_ChaPai_Pai = 0;
uint8_t g_ChaPai_Max = 0;

uint8_t g_SavePaiBet = 0;      // 保存游戏结果上的押分。
uint8_t g_SavePaiBet0 = 0;     //
uint8_t g_DingFen1_KG = 1;     // 顶分开关，正常是打开的。
uint8_t g_DingFen1_FX = 0;     // 顶分被发现的标志，
uint8_t g_DingFen1_Random = 0; // 顶分停多少局后再开始启动。
uint8_t g_DingFen1_Recode = 0; // 记录顶分
