#ifndef __DIPSW_H__
#define __DIPSW_H__
#include "includes.h"

uint8_t GetBaseBei(DipSwType *dipsw);
uint32_t GetNoteIn(DipSwType *dipsw);
uint32_t GetNoteOUt(DipSwType *dipsw);
uint32_t GetCoinIn(DipSwType *dipsw);
uint32_t GetCoinOut(DipSwType *dipsw);

uint32_t GetMinbet(DipSwType *dipsw);
uint32_t GetMaxbet(DipSwType *dipsw);

uint32_t GetDec(DipSwType *dipsw);
uint32_t GetRate(DipSwType * dipsw);
uint8_t GetPlaying(DipSwType *dipsw);
uint32_t GetGameTime(DipSwType *dipsw);

uint32_t GetChildOver(DipSwType *dipsw);
uint32_t GetGameOver(DipSwType *dipsw);

uint32_t GetMinBonus(DipSwType *dipsw);
uint32_t GetMaxBonus(DipSwType *dipsw);

uint16_t GetPlayFun(DipSwType *dipsw);

uint8_t GetLuckIndex(DipSwType *dipsw);

uint32_t GetMinJifen(DipSwType *dipsw);
uint32_t GetJifenRate(DipSwType *dipsw);

uint32_t GetLineMinLitBonus(DipSwType *dipsw);
uint32_t GetLineMaxLitBonus(DipSwType *dipsw);

uint32_t GetLineMinMidBonus(DipSwType *dipsw);
uint32_t GetLineMaxMidBonus(DipSwType *dipsw);

uint32_t GetLineMinBigBonus(DipSwType *dipsw);
uint32_t GetLineMaxBigBonus(DipSwType *dipsw);

extern const uint32_t g_NewMul[4][5];

#endif
