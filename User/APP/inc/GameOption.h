#ifndef __GAMEOPTION_H__
#define __GAMEOPTION_H__
#include "includes.h"

void SetGameResult(uint8_t* ptr, uint16_t game_count, uint8_t val);
uint8_t GetGameResult(void* buf, uint16_t game_count);
uint8_t GetGameLuck(void *buf, uint16_t game_count);
void SetGameLuckBuf(uint8_t *buf);
void SetGameLuck(uint8_t *ptr, uint16_t game_count, uint8_t val);
uint32_t GetGameLuckWin2(uint8_t luck);
uint32_t ClearBet(ChildType *pChild);
void ReturnBet(ChildType *pChild);


#endif
