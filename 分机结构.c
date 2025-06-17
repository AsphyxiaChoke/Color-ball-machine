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
    PC_GAMEEND      // 游戏结束
};

#define BET_NUMBER 5
// 分机数据结构：
typedef struct ChildCmdType
{
    uint8_t procmd;                     // 游戏进程
    uint8_t gametime;                   // 游戏时间
    uint16_t totalgame;                 // 游戏总轮数
    uint16_t gamecount;                 // 游戏总局数
    uint32_t linebonus;                 // 游戏随机彩金
    uint8_t result;                     // 游戏结果
    uint8_t luck;                       // 高4位特殊奖项，低4位幸运位
    uint16_t Conv;                      // 分机切换分值
    uint16_t Dec;                       // 限红 16位 ？？？可能要改
    uint32_t childcredit;               // 分机总分
    uint32_t childwin;                  // 分机赢分
    uint16_t hei_cnt;                   // 花色计数
    uint16_t hong_cnt;                  // 花色计数
    uint16_t mei_cnt;                   // 花色计数
    uint16_t fang_cnt;                  // 花色计数
    uint16_t wang_cnt;                  // 花色计数
    uint16_t childbet[BET_NUMBER];      // 本台分机每门押分
    uint32_t childTotalbet[BET_NUMBER]; // 所有分机每门总押分
    uint8_t childid;                    // 分机号
    uint32_t gamestion;                 // 游戏状态
    uint16_t checksum;                  // 校验和
} ChildCmdType;
// 分机发送历史记录和特殊奖项结构：
typedef struct ChildHistoryType
{
    uint8_t procmd;          // 游戏进程
    uint8_t historybuf[100]; // 历史记录或特殊奖项
    uint8_t childid;         // 分机号
    uint16_t checksum;       // 校验和
} ChildHistoryType;

//以上分机结构体数据放入分机发送数据结构的 Data[114];