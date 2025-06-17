#ifndef ZHUANQIUJI_H_
#define ZHUANQIUJI_H_

#define __ZHUANGQIANJI__

#define MACHINERST    	0		//��λ
#define MACHINEXIQIU    6//1		//��ϴ��
#define MACHINEKAIQIU   7//2		//�Ͽ���ֻ�������������
#define MACHINESHOUQIU  8//3		//����
#define MACHINECHECK    4		//��ѯ
#define MACHINESHEQIU   5		//������λ
#define MAC_XI_QIU		6		//�Զ���̬ϴ��
#define MAC_KAI_QIU		7		//�Զ���̬����
#define MAC_SHOU_QIU	8		//�Զ���̬����

#define MAC_SAVE_QIU	9		// 9 0xea ���ö�̬��λ....
#define MAC_CAP_QIU		10		// 10 0xeb ƥ�䵱ǰ��λ


// #define SWERR1		0x01
// #define SWERR2		0x02
// #define SWERR3		0x04
// #define SWERR4		0x08
// #define SWERR5		0x10
// #define SWERR6		0x20	//ͨ�Ų���

uint8_t ZhungQiuJiAsk(uint8_t * state);
uint8_t SendCmdZhuanQIuJI(uint8_t cmd,uint8_t sheqiu);
void Init_JiXinUartSet(void);

void MachineReSet(void);//��λ
uint8_t MachineOutQiu(void);//ϴ��
uint8_t MachineOpenQiu(void);//����1
void MachineCloseQiu(void);//����
void MachineOpenQiu2(void);//����2 
uint8_t MachineSheQiu(uint8_t ball);//������λ
void MachineSaveQiu(void);//������λ
uint8_t MachineCapQiu(void);//�Ƚ���λ

// uint8_t MachineAutoOutQiu(void);
// uint8_t MachineAutoOpenQiu(void);
// void MachineAutoCloseQiu(void);
// uint8_t MachineAutoSheQiu(uint8_t ball);
// void MachineAutoSaveQiu(void);
// uint8_t MachineCheckSaveQiu(void);
#endif

