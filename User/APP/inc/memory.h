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
// 分机标志
//------------------------------------------------------------
#define COIN_TIME_SPEED 100 // 退币间隔
#define CHILDFLG 0x1a       // 分机标志

#define XI_FLAG (1UL << 0)
#define HIDE_FLAG (1UL << 1)
#define QUICK_WIN (1UL << 2)
#define COMPUTER (1UL << 3)
#define QUICK_BONUS (1UL << 4)
#define WINLAMP_FLAG (1UL << 6)
#define LOCK_FLAG (1UL << 8) /*Child[0].Flag 标志*/
#define TUI_FLAG (1UL << 9)
#define SAVE_WIN (1UL << 10)

#define SSR_OPEN (0x10) // 分机退币马达开关位
//-------------------------------------------------------------
// 游戏功能标志位定义
//-------------------------------------------------------------
#define BONUS_FLG (1UL << 0)    // 彩金功能
#define FANBEI_FLG (1UL << 1)   // 加倍功能
#define MINGPAI_FLG (1UL << 2)  // 明牌功能
#define JIFEN_FLG (1UL << 3)    // 积分功能
#define LUCKBIT_FLG (1UL << 4)  // 幸运位功能
#define ALLWIN_FLG (1UL << 5)   // 全中功能
#define SONGDENG_FLG (1UL << 6) // 送灯功能
#define PLAYING_FLG (1UL << 7)  // 演示功能
#define TOUCAI_FLG (1UL << 8)    // 退币功能
//-------------------------------------------------------------
// II2C save data flg
//-------------------------------------------------------------
#define S_DIP (1UL << 0)         // 游戏设置
#define S_ACCOUTPASS (1UL << 1)  // 查账密码
#define S_CONFIGPASS (1UL << 2)  // 参数密码
#define S_SYSTEMPASS (1UL << 3)  // 系统密码
#define S_FUTORYTIME (1UL << 4)  // 出厂时间
#define S_PROGRAMVEL (1UL << 5)  // 游戏版本
#define S_MACHINELINE (1UL << 6) // 线号
#define S_MACHINEID (1UL << 7)   // 台号
#define S_QIUWEI (1UL << 8)      // 球位
//---------------------------------------------------
// 机器运行错误
//---------------------------------------------------
#define MACHERR01 (1UL << 0)    // 顶杆故障
#define MACHERR02 (1UL << 1)    // 帽子故障
#define MACHERR03 (1UL << 2)    // 翻球故障
#define MACHERR04 (1UL << 3)    // 落球故障
#define MACHERR05 (1UL << 4)    // 中心盘故障
#define MACHERR06 (1UL << 5)    // 机芯未联接
#define MACHERR07 (1UL << 6)    // 球位故障
#define MACHERR08 (1UL << 7)    // 门控故障
#define MACHERR09 (1UL<<8) //




#pragma pack(1)
typedef struct sw_struct
{
    uint16_t NoteIn;          // 开分大小
    uint16_t NoteOut;         // 下分大小
    uint16_t CoinIn;          // 投币比例
    uint16_t CoinOut;         // 退币比例
    uint16_t Minbet;          // 最小押分
    uint16_t Maxbet;          // 最大押分
    uint32_t DecIndex;        // 限红选择
    uint8_t Playing;          // 无分演示
    uint8_t RateIndex;        // 牌路选择
    uint8_t TimeIndex;        // 游戏时间
    uint32_t OverIndex;       // 破台分数
    uint32_t GameOver;        // 爆机分数
    uint32_t MinBonus;        // 最小彩金
    uint32_t MaxBonus;        // 最大彩金
    uint8_t CoLtd;            // 公司台标志
    uint16_t FuctoryFunEn;    // 公司设置游戏功能选择
    uint16_t PlayFunEn;       // 游戏幸运功能选择
    uint8_t LuckTimes;        // 幸运次数
    uint8_t LuckIndex;        // 幸运奖比例  累计到幸运奖池中的比例
    uint32_t MinJifen;        // 最小积分
    uint32_t Jifenrate;       // 积分累加比例
    uint32_t Toucairate;     // 头彩比例
    uint32_t Maxlinelitbonus; // 最大连线小彩金
    uint32_t Minlinelitbonus; // 最小连线小彩金
    uint32_t Maxlinemidbonus; // 最大连线中彩金
    uint32_t Minlinemidbonus; // 最小连线中彩金
    uint32_t Maxlinebigbonus; // 最大连线大彩金
    uint32_t Minlinebigbonus; // 最小连线大彩金
} DipSwType;

// 分机发送数据结构总长128字节：
typedef struct ChildDispType
{
    uint8_t reserve;      // 备用
    uint8_t PageStart;    // 包计数
    uint8_t NewTuiFlag;   // 分机认证标志
    uint8_t TuiBiCnt1;    // 初值为0主机每处理一次退币此值加1
    uint8_t TuiBiNumLow;  // 本次需要退币数量的低8位
    uint8_t TuiBiNumHigh; // 本次需要退币数量的高8位
    uint8_t TuiBiCnt2;    // 和TuiBiCnt1值一样
    uint8_t CheckSum;     // 以上六值的效验和
    uint8_t PcConectCnt;  // PC连接计数器
    uint8_t Count[3];     // 中奖灯 8*3=24 bit灯
    uint8_t Cmd;          // 发送的数据命令
    uint8_t Len;          // 数据长度
    uint8_t Data[113];    // 数据存放起始位置
    uint8_t Pageend;      // 包计数
} ChildDispType;

// 分机接收数据结构
typedef struct ChildInputType
{
    uint8_t KeyCount; // 分机硬件提供的键盘计数器

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

// 游戏进程定义
enum GAME_PRO
{
    PC_RESTART = 1, // 游戏重新开始
    PC_GAMESTART,   // 每局游戏开始
    PC_STARTBET,    // 开始押分
    PC_GAMETIME,    // 游戏时间
    PC_STOPBET,     // 停止押分
    PC_RESULT,      // 游戏结果
    PC_STARTBONUS,  // 开始特殊奖项中奖开始
    PC_STOPBONUS,   // 结束特殊奖项中奖结束
    PC_STARTWIN,    // 开始中奖得分
    PC_STOPWIN,     // 结束中奖
    PC_GAMEEND,     // 游戏结束

    PC_HISTORY = 20, // 历史记录
    PC_LUCKHISTORY   // 历史记录奖项
};

typedef struct ChildType
{
    uint32_t Credit;             // 4
    uint32_t Win;                // 4
    uint32_t Win2;                // 二次中特殊奖的一个赢分
    uint16_t Bet[BET_NUMBER];    // 当前的压分 //10
    uint16_t OldBet[BET_NUMBER]; // 10
    uint16_t Conv;               // 押分转换		//2
    uint32_t CoinIn;             // 投币总进		//4
    uint32_t CoinOut;            // 退币总出		//4
    uint32_t KeyIn;              // 开分总开		//4
    uint32_t KeyOut;             // 洗分总洗		//4

    uint32_t PlayCnt;
    uint32_t WinCnt;

    uint32_t Count; // 退币处理标志	//4

    uint32_t Flag; // 处理一次性数据标志	//4

    uint32_t KeyLED;

    ChildDispType *pChildDisp;   // 4
    ChildInputType *pChildInput; // 4
    uint8_t ChildKeyCount[32];   // 按键计数		//32
    uint32_t ChildKeyPress;      // 按键值		//4

    uint8_t KeyCount; // 对应到pChildInput的KeyCount(分机硬件提供的键盘计数器);//1

    uint32_t CoinInTmp;   // 投币使用,表示现在投了多少个币,主程序只能读不能写	//4
    uint32_t CoinInTmp2;  // 投币使用,给主程序使用							//4
    uint32_t CoinOutTmp;  // 退币使用,表示现在退了多少个币,主程序只能读不能写	//4
    uint32_t CoinOutTmp2; // 退币使用,给主程序使用							//4
    int64_t CoinOutTime;  // 退币中断使用,电机时间							//8
    uint8_t TuiBiNum;     // 退币要退个数
    int64_t Bonus;        // 这台分机显示的彩金								//8
    int64_t WinBonus;     // 这台分机能中多少彩金								//8
    uint32_t ResultWin;   // 计算出的赢分										//4

    uint64_t AllBet8; // 总押												//8
    uint64_t AllWin8; // 总中												//8

    uint8_t TouInitCount; // 投币初始化计数器
    uint8_t TuiInitCount; // 退币初始化计数器
    uint8_t TouOldCount;  // 先前投币计数
    uint8_t TuiOldCount;  // 先前退币计数
    uint8_t TuiProcCount; // 退币处理计数
    uint8_t TuiProcFlag;

    uint8_t CoinKeyCount;     // 防止开机时误投币
    uint8_t InitCoinOut;      // 投退初始化
    uint8_t AgainInitCoinOut; // 投退初始化
    uint8_t DispCount;        // 对应到pChildDisp的Count(码表);
    uint8_t PcConectCnt;      // 通信计数											//1

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
