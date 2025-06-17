#ifndef ZHUANQIUJI_H_
#define ZHUANQIUJI_H_

#define __ZHUANGQIANJI__

#define MACHINERST    	0		//复位
#define MACHINEXIQIU    6//1		//新洗球
#define MACHINEKAIQIU   7//2		//老开球只用于正常开球的
#define MACHINESHOUQIU  8//3		//收球
#define MACHINECHECK    4		//查询
#define MACHINESHEQIU   5		//设置球位
#define MAC_XI_QIU		6		//自动动态洗球
#define MAC_KAI_QIU		7		//自动动态开球
#define MAC_SHOU_QIU	8		//自动动态收球

#define MAC_SAVE_QIU	9		// 9 0xea 设置动态球位....
#define MAC_CAP_QIU		10		// 10 0xeb 匹配当前球位


// #define SWERR1		0x01
// #define SWERR2		0x02
// #define SWERR3		0x04
// #define SWERR4		0x08
// #define SWERR5		0x10
// #define SWERR6		0x20	//通信不上

uint8_t ZhungQiuJiAsk(uint8_t * state);
uint8_t SendCmdZhuanQIuJI(uint8_t cmd,uint8_t sheqiu);
void Init_JiXinUartSet(void);

void MachineReSet(void);//复位
uint8_t MachineOutQiu(void);//洗球
uint8_t MachineOpenQiu(void);//开球1
void MachineCloseQiu(void);//收球
void MachineOpenQiu2(void);//开球2 
uint8_t MachineSheQiu(uint8_t ball);//设置球位
void MachineSaveQiu(void);//保存球位
uint8_t MachineCapQiu(void);//比较球位

// uint8_t MachineAutoOutQiu(void);
// uint8_t MachineAutoOpenQiu(void);
// void MachineAutoCloseQiu(void);
// uint8_t MachineAutoSheQiu(uint8_t ball);
// void MachineAutoSaveQiu(void);
// uint8_t MachineCheckSaveQiu(void);
#endif

