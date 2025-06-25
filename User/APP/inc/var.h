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
// 游戏参数
//------------------------------------------------------
extern uint32_t __24C04_FLAG;     // ii2c save flg
extern DipSwType DipSw;           // 游戏设置
extern DipSwType SetDipSw;        // 游戏临时设置
extern uint32_t Machine_line;     // 机台线号
extern uint32_t Machine_id;       // 机台台号
extern char FutoryTime[10];       // 出厂日期
extern char ProgramVel[10];       // 程序版本
extern int64_t Prevpayoff;        // 前期盈利
extern int64_t CurrPayoff;        // 现在盈利
extern int32_t AccoutPassWord;    // 查账密码
extern int32_t ConfigPassWord;    // 参数密码
extern int32_t SystemPassWord;    // 系统密码
extern int32_t FuncPassWord;      // 功能密码
extern int32_t RecordPassWord;    // 记录密码
extern uint8_t Qiuwei[10];        // 机器球位
extern LuckJiangType LuckJiang;   // 幸运奖记录
extern LuckJiangType RecordLuckJiang; // 幸运奖记录
extern GameRecordType GameRecord; // 游戏记录
//------------------------------------------------------
// 游戏参数重设使用
//------------------------------------------------------
extern char SetFutoryTime[10];
extern char SetProgramVel[10];
extern int32_t SetAccoutPassWord;
extern int32_t SetConfigPassWord;
extern int32_t SetSystemPassWord;
//-----------------------------------------------------
// 设置球位使用变量
//-----------------------------------------------------
extern uint8_t SetQiuwei[10]; // 设置球位
extern uint8_t ChuQiuHao;     // 亮出球位
extern uint8_t QiuTest;       // 亮球操作QiuText
extern uint8_t QiuState;
extern uint8_t SwtQiuFlag;
//---------------------------------------------------------
// 系统时间相关变量
//---------------------------------------------------------
extern TimeType SysTime; // 系统时间
extern TimeType SetTime; // 设置时间
//------------------------------------------------------
// 游戏变量
//------------------------------------------------------
extern uint8_t GameTime;
extern uint16_t TotalGame;
extern uint16_t GameCount;
extern uint16_t DispGameCount;  // 显示游戏局数
extern uint16_t ChildTotalGame; // 玩家显示轮数
extern uint16_t ChildGameCount; // 玩家显示局数
extern uint32_t LineBonus;
extern uint32_t MaxBonus; // 头顶大彩金
extern uint32_t MidBonus; // 头顶中彩金
extern uint32_t MinBonus; // 头顶小彩金
extern uint8_t GamePai;
extern uint8_t MingPai; // 明牌状态标志
extern uint8_t Luck;
extern uint8_t Luck_CS;
extern uint8_t LuckBuf[TOTAL_JUSHU]; // 游戏幸运奖BUF
extern uint8_t LuckFunBuf[10];       // 游戏幸运奖类目
extern LuckJiangType LuckJiang;

extern uint32_t DingFen[MAX_FENJI];
extern uint8_t New_Bill[TOTAL_JUSHU * TOTAL_LUNSHU]; //
//------------------------------------------------------
// 游戏计数
//------------------------------------------------------
extern uint16_t HeiCnt;
extern uint16_t HongCnt;
extern uint16_t CaoCnt;
extern uint16_t FangCnt;
extern uint16_t WangCnt;
extern uint8_t History[TOTAL_JUSHU];
extern uint8_t LuckHistory[TOTAL_JUSHU];
//------------------------------------------------------
// 分机变量
//------------------------------------------------------
extern uint16_t ChildTotalGame;
extern uint16_t ChildGameCount;
extern ChildType Child[MAX_FENJI];
extern uint32_t Sramflg;
//------------------------------------------------------
// 游戏状态变量
//------------------------------------------------------
extern uint8_t ProcMem;    // 游戏步骤
extern uint8_t OldProcMem; // 游戏步骤
extern uint32_t Gamestation;

//------------------------------------------------------
// 特殊功能变量
//------------------------------------------------------
extern uint8_t g_AiWaterSw;       // 自动放水
extern uint8_t g_AiDingfenSw;     // 自动顶分
extern uint8_t g_AiKeyLudanSw;    // 自动改路单
extern uint8_t g_AiTongjiLudanSw; // 自动统计路单

extern uint8_t g_Random_Tab_Count;
extern uint8_t g_Random_Count;
extern uint8_t g_Random_Add;
extern uint8_t g_Random_KeyRecode;
extern uint8_t g_XiPai4_Count;
extern uint8_t g_ChaPai_Count;
extern uint8_t g_ChaPai_Pai;
extern uint8_t g_ChaPai_Max;

extern uint8_t g_SavePaiBet;      // 保存游戏结果上的押分。
extern uint8_t g_SavePaiBet0;     //
extern uint8_t g_DingFen1_KG;     // 顶分开关，正常是打开的。
extern uint8_t g_DingFen1_FX;     // 顶分被发现的标志，
extern uint8_t g_DingFen1_Random; // 顶分停多少局后再开始启动。
extern uint8_t g_DingFen1_Recode; // 记录顶分
#endif
