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
    PC_GAMEEND      // ��Ϸ����
};

#define BET_NUMBER 5
// �ֻ����ݽṹ��
typedef struct ChildCmdType
{
    uint8_t procmd;                     // ��Ϸ����
    uint8_t gametime;                   // ��Ϸʱ��
    uint16_t totalgame;                 // ��Ϸ������
    uint16_t gamecount;                 // ��Ϸ�ܾ���
    uint32_t linebonus;                 // ��Ϸ����ʽ�
    uint8_t result;                     // ��Ϸ���
    uint8_t luck;                       // ��4λ���⽱���4λ����λ
    uint16_t Conv;                      // �ֻ��л���ֵ
    uint16_t Dec;                       // �޺� 16λ ����������Ҫ��
    uint32_t childcredit;               // �ֻ��ܷ�
    uint32_t childwin;                  // �ֻ�Ӯ��
    uint16_t hei_cnt;                   // ��ɫ����
    uint16_t hong_cnt;                  // ��ɫ����
    uint16_t mei_cnt;                   // ��ɫ����
    uint16_t fang_cnt;                  // ��ɫ����
    uint16_t wang_cnt;                  // ��ɫ����
    uint16_t childbet[BET_NUMBER];      // ��̨�ֻ�ÿ��Ѻ��
    uint32_t childTotalbet[BET_NUMBER]; // ���зֻ�ÿ����Ѻ��
    uint8_t childid;                    // �ֻ���
    uint32_t gamestion;                 // ��Ϸ״̬
    uint16_t checksum;                  // У���
} ChildCmdType;
// �ֻ�������ʷ��¼�����⽱��ṹ��
typedef struct ChildHistoryType
{
    uint8_t procmd;          // ��Ϸ����
    uint8_t historybuf[100]; // ��ʷ��¼�����⽱��
    uint8_t childid;         // �ֻ���
    uint16_t checksum;       // У���
} ChildHistoryType;

//���Ϸֻ��ṹ�����ݷ���ֻ��������ݽṹ�� Data[114];