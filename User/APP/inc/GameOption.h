#ifndef __GAMEOPTION_H__
#define __GAMEOPTION_H__
#include "includes.h"

void SetGameResult(uint8_t* ptr, uint32_t game_count, uint8_t val);
uint8_t GetGameResult(void* buf, uint32_t game_count);
uint8_t GetGameLuck(void);

uint32_t ClearBet(ChildType *pChild);
void ReturnBet(ChildType *pChild);


#endif
