#ifndef __ARITH_H__
#define __ARITH_H__
#include "includes.h"

#define CHILD_BET0_LED (1UL<<0)
#define CHILD_BET1_LED (1UL<<1)
#define CHILD_BET2_LED (1UL<<2)
#define CHILD_BET3_LED (1UL<<3)
#define CHILD_BET4_LED (1UL<<4)

#define __Ues_Android_Key__
#ifndef  __Ues_Android_Key__
#define CHILD_MASK_BET0 (1UL << 0)
#define CHILD_MASK_BET1 (1UL << 1)
#define CHILD_MASK_BET2 (1UL << 2)
#define CHILD_MASK_BET3 (1UL << 3)
#define CHILD_MASK_BET4 (1UL << 4)
#else
#define CHILD_MASK_BET0 (1UL << 16)
#define CHILD_MASK_BET1 (1UL << 17)
#define CHILD_MASK_BET2 (1UL << 18)
#define CHILD_MASK_BET3 (1UL << 19)
#define CHILD_MASK_BET4 (1UL << 20)
#endif

#define CHILD_MASK_TUI (1UL << 8)      //退�?
#define CHILD_MASK_CONV (1UL << 9)     //切换
//#define CHILD_MASK_CAN (1UL << 10)      //取消
#define CHILD_MASK_HIDE (1UL << 11)     //隐藏
#define CHILD_MASK_KAI (1UL << 12)      //大开
#define CHILD_MASK_KAI2 (1UL << 13)        //小开
#define CHILD_MASK_XI (1UL << 14)       //小洗
#define CHILD_MASK_XI2 (1UL << 15)       //大洗

#define a_CHILD_MASK_CONV10 (1UL << 24)
#define a_CHILD_MASK_CONV50 (1UL << 25)
#define a_CHILD_MASK_CONV100 (1UL << 26)
#define a_CHILD_MASK_CONV500 (1UL << 27)
#define a_CHILD_MASK_CONV1000 (1UL << 28)
#define a_CHILD_MASK_XUIBET (1UL << 29) //安卓�?�?
#define CHILD_MASK_CAN (1UL << 30)    //安卓取消

#ifdef __LOCK_EN__
#define CHILD_MASK_SUO (1UL << 5) //
#else
#define CHILD_MASK_SUO 0
#endif


int ChildKeyScan(ChildType *pChild, uint32_t mask);

__inline int ChildKeyBET0(ChildType *pChild) { return ChildKeyScan(pChild, CHILD_MASK_BET0); }
__inline int ChildKeyBET1(ChildType *pChild) { return ChildKeyScan(pChild, CHILD_MASK_BET1); }
__inline int ChildKeyBET2(ChildType *pChild) { return ChildKeyScan(pChild, CHILD_MASK_BET2); }
__inline int ChildKeyBET3(ChildType *pChild) { return ChildKeyScan(pChild, CHILD_MASK_BET3); }
__inline int ChildKeyBET4(ChildType *pChild) { return ChildKeyScan(pChild, CHILD_MASK_BET4); }
__inline int ChildKeyBET5(ChildType *pChild) { return 0; }
__inline int ChildKeyBET6(ChildType *pChild) { return 0; }
__inline int ChildKeyBET7(ChildType *pChild) { return 0; }

__inline int ChildKeyCONV(ChildType *pChild) { return ChildKeyScan(pChild, CHILD_MASK_CONV); } //�?�?
__inline int ChildKeyXI(ChildType *pChild) { return ChildKeyScan(pChild, CHILD_MASK_XI); }
__inline int ChildKeyXI2(ChildType *pChild) { return ChildKeyScan(pChild, CHILD_MASK_XI2); }
__inline int ChildKeyREP(ChildType *pChild) { return 0; }
__inline int ChildKeyKAI(ChildType *pChild) { return ChildKeyScan(pChild, CHILD_MASK_KAI); }
__inline int ChildKeyKAI2(ChildType *pChild)  { return ChildKeyScan(pChild,CHILD_MASK_KAI2); }

__inline int ChildKeyCan(ChildType *pChild) { return ChildKeyScan(pChild, CHILD_MASK_CAN); }

__inline int ChildKeyHIDE(ChildType *pChild) { return 0; }
__inline int ChildKeyTUI(ChildType *pChild) { return ChildKeyScan(pChild, CHILD_MASK_TUI); }
__inline int ChildKeyHPSW(ChildType *pChild) { return 0; }

__inline int ChildKeyCONV10(ChildType *pChild) { return ChildKeyScan(pChild, a_CHILD_MASK_CONV10); }
__inline int ChildKeyCONV50(ChildType *pChild) { return ChildKeyScan(pChild, a_CHILD_MASK_CONV50); }
__inline int ChildKeyCONV100(ChildType *pChild) { return ChildKeyScan(pChild, a_CHILD_MASK_CONV100); }
__inline int ChildKeyCONV500(ChildType *pChild) { return ChildKeyScan(pChild, a_CHILD_MASK_CONV500); }
__inline int ChildKeyCONV1000(ChildType *pChild) { return ChildKeyScan(pChild, a_CHILD_MASK_CONV1000); }

void SetGameTime(uint32_t time);
void BuzzerDeal(ChildType *pChild);
void Buzzer(ChildType *pChild);
void DispCREDIT(ChildType *pChild);
void DispWIN(ChildType *pChild);
void DispBET(ChildType *pChild, uint32_t index);
void DispBetLED(ChildType *pChild, uint32_t credit);
void FlashWin(uint32_t flash);
void FlashBet(uint32_t index, uint32_t flash);
void FlashBetLED(ChildType *pChild, uint32_t flash);
void FlashWinLamp(ChildType *pChild, uint32_t flash);
void FlashLuckBetLED(ChildType *pChild, uint8_t flash);

void CloseSSR(ChildType *pChild);
void Scene(void);

uint32_t WriteChildAccu(ChildType *pChild, uint32_t index, uint32_t value);
void SetTuiBiNum(ChildType *pChild);
uint32_t ChildMaxBET(ChildType *pChild, uint32_t index);
uint32_t ChildCancel(ChildType *pChild);
void CoinKeyProc1(ChildType *pChild);
void BetKeyProc1(ChildType *pChild);
#endif
