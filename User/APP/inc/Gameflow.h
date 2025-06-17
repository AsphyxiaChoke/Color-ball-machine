#ifndef __GAMEFLOW_H__
#define __GAMEFLOW_H__
#include "includes.h"

#ifndef __weak
#define __weak __attribute__((weak))
#endif


void BackProcess(void);
void CoinKeyProc(void);
void ReStart(void);
void Start(void);
void WaitBET(void);
void StartRun(void);
void WinProc(void);
void GameEnd(void);
void GameEnd2(void);
void GameSetBall(void);

#endif
