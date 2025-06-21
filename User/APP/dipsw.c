#include "dipsw.h"

const uint32_t g_NewMul[4][5]=
{
    {380,380,400,400,2000},
    {390,390,400,400,2000},
    {395,395,400,400,2000},
    {400,400,400,400,2000}
};

//uint8_t GetBaseBei(DipSwType *dipsw) { return dipsw->BaseBeiIndex; }

uint32_t GetNoteIn(DipSwType *dipsw) { return dipsw->NoteIn; }
uint32_t GetNoteOUt(DipSwType *dipsw) { return dipsw->NoteOut; }
uint32_t GetCoinIn(DipSwType *dipsw) { return dipsw->CoinIn; }
uint32_t GetCoinOut(DipSwType *dipsw) { return dipsw->CoinOut; }

uint32_t GetMinbet(DipSwType *dipsw) { return dipsw->Minbet; }
uint32_t GetMaxbet(DipSwType *dipsw) { return dipsw->Maxbet; }

uint32_t GetDec(DipSwType *dipsw) { return dipsw->DecIndex; }
uint32_t GetRate(DipSwType * dipsw){return dipsw->RateIndex;}
uint8_t GetPlaying(DipSwType *dipsw) { return dipsw->Playing; }
uint32_t GetGameTime(DipSwType *dipsw) { return dipsw->TimeIndex; }

uint32_t GetChildOver(DipSwType *dipsw) { return dipsw->OverIndex; }
uint32_t GetGameOver(DipSwType *dipsw) { return dipsw->GameOver; }

uint32_t GetMinBonus(DipSwType *dipsw){return dipsw->MinBonus;}
uint32_t GetMaxBonus(DipSwType *dipsw) { return dipsw->MaxBonus; }

uint16_t GetPlayFun(DipSwType *dipsw) { return dipsw->PlayFunEn; }

uint16_t GetLuckTimes(DipSwType *dipsw) { return dipsw->LuckTimes; }
uint16_t GetLuckIndex(DipSwType *dipsw) { return dipsw->LuckIndex; }

uint32_t GetMinJifen(DipSwType *dipsw) { return dipsw->MinJifen; }
uint32_t GetJifenRate(DipSwType *dipsw) { return dipsw->Jifenrate; }

uint32_t GetTouCaiRate(DipSwType *dipsw) { return dipsw->Toucairate; }

uint32_t GetLineMinLitBonus(DipSwType *dipsw) { return dipsw->Minlinelitbonus; }
uint32_t GetLineMaxLitBonus(DipSwType *dipsw) { return dipsw->Maxlinelitbonus; }

uint32_t GetLineMinMidBonus(DipSwType *dipsw) { return dipsw->Minlinemidbonus; }
uint32_t GetLineMaxMidBonus(DipSwType *dipsw) { return dipsw->Maxlinemidbonus; }

uint32_t GetLineMinBigBonus(DipSwType *dipsw) { return dipsw->Minlinebigbonus; }
uint32_t GetLineMaxBigBonus(DipSwType *dipsw) { return dipsw->Maxlinebigbonus; }

