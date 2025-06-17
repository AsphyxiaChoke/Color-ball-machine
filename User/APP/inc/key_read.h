#ifndef __KEY_READ_H__
#define __KEY_READ_H__
#include "includes.h"
//-------------------------------------------------------
//主机按键共有12个
//
//-------------------------------------------------------
#define cMaskK0         (0)
#define cMaskK1         (1)
#define cMaskK2         (2)
#define cMaskK3         (3)
#define cMaskK4         (4)
#define cMaskK5         (5)
#define cMaskK6         (6)
#define cMaskK7         (7)

#define cMaskX0         (8)
#define cMaskX1         (9)
#define cMaskX2         (10)
#define cMaskX3         (11)
#define cMaskX4         (12)
#define cMaskX5         (13)
#define cMaskX6         (14)
#define cMaskX7         (15)

#define MaskK0          (1UL<<cMaskK0)
#define MaskK1          (1UL<<cMaskK1)
#define MaskK2          (1UL<<cMaskK2)
#define MaskK3          (1UL<<cMaskK3)
#define MaskK4          (1UL<<cMaskK4)
#define MaskK5          (1UL<<cMaskK5)
#define MaskK6          (1UL<<cMaskK6)
#define MaskK7          (1UL<<cMaskK7)

#define MaskX0          (1UL<<cMaskX0)
#define MaskX1          (1UL<<cMaskX1)
#define MaskX2          (1UL<<cMaskX2)
#define MaskX3          (1UL<<cMaskX3)
#define MaskX4          (1UL<<cMaskX4) 
#define MaskX5          (1UL<<cMaskX5) 
#define MaskX6          (1UL<<cMaskX6) 
#define MaskX7          (1UL<<cMaskX7) 

#define HaveKeyK0()     KEY_SCAN(MaskK0)
#define HaveKeyK1()     KEY_SCAN(MaskK1)
#define HaveKeyK2()     KEY_SCAN(MaskK2)
#define HaveKeyK3()     KEY_SCAN(MaskK3)
#define HaveKeyK4()     KEY_SCAN(MaskK4)
#define HaveKeyK5()     KEY_SCAN(MaskK5)
#define HaveKeyK6()     KEY_SCAN(MaskK6)
#define HaveKeyK7()     KEY_SCAN(MaskK7)

#define HaveKeyX0()     KEY_SCAN(MaskX0)
#define HaveKeyX1()     KEY_SCAN(MaskX1)
#define HaveKeyX2()     KEY_SCAN(MaskX2)
#define HaveKeyX3()     KEY_SCAN(MaskX3)
#define HaveKeyX4()     KEY_SCAN(MaskX4)
#define HaveKeyX5()     KEY_SCAN(MaskX5)
#define HaveKeyX6()     KEY_SCAN(MaskX6)
#define HaveKeyX7()     KEY_SCAN(MaskX7)

#define HaveKeyOPEN()   HaveKeyK0()

typedef void* (*pKeyTaskFunc)(uint8_t *pKeyCount, uint32_t mask, uint32_t *rtv);
extern volatile uint32_t KeyPress;
extern volatile uint8_t  KeyCount[32];

extern pKeyTaskFunc KeyTaskTbl[MAX_FENJI][32];
extern const pKeyTaskFunc InitKeyTaskTbl[32];

extern void* KeyTaskFunc1(uint8_t *pKeyCount,uint32_t mask, uint32_t *rtv);
extern void* KeyTaskWait1(uint8_t *pKeyCount,uint32_t mask, uint32_t *rtv);
extern void* KeyTaskFunc2(uint8_t *pKeyCount,uint32_t mask, uint32_t *rtv);
extern void* KeyTaskWait2(uint8_t *pKeyCount,uint32_t mask, uint32_t *rtv);
extern void* KeyTaskWait0(uint8_t *pKeyCount,uint32_t mask, uint32_t *rtv);

__inline uint32_t ChildReadKey(ChildInputType *pChildInput)
{
  return ((uint32_t)(pChildInput->Key1) << 0) |
         ((uint32_t)(pChildInput->Key3) << 8) |
         ((uint32_t)(pChildInput->Android_Key1) << 16)|
         ((uint32_t)(pChildInput->Android_Key2) << 24);
}
int KEY_SCAN(uint32_t mask);
void Master_key_proc(void);
void Child_key_proc(uint8_t x);
#endif
