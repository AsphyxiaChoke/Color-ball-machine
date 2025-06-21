#include "key_read.h"
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include "stm32h7xx.h"

volatile uint32_t KeyPress;
volatile uint8_t KeyCount[32];

const uint32_t MainKeyIndex[] =
    {
        cMaskK0, cMaskK1, cMaskK2, cMaskK3, cMaskK4, cMaskK5, cMaskK6, cMaskK7,
        cMaskX0, cMaskX1, cMaskX2, cMaskX3, cMaskX4, cMaskX5};

pKeyTaskFunc MainKeyTaskTbl[] =
    {
        KeyTaskFunc2, KeyTaskFunc2, KeyTaskFunc2, KeyTaskFunc2,
        KeyTaskFunc2, KeyTaskFunc2, KeyTaskWait2, KeyTaskWait2,
        KeyTaskFunc2, KeyTaskFunc2, KeyTaskFunc2, KeyTaskFunc2,
        KeyTaskFunc2, KeyTaskFunc2};
//-------------------------------------------------------------------------------
// 分机按键
//-------------------------------------------------------------------------------
pKeyTaskFunc KeyTaskTbl[MAX_FENJI][32];
//const pKeyTaskFunc InitKeyTaskTbl[32] =
//    {KeyTaskFunc1, KeyTaskFunc1, KeyTaskFunc1, KeyTaskFunc1, KeyTaskFunc1, KeyTaskWait2, KeyTaskWait2, KeyTaskWait2,
//     KeyTaskWait2, KeyTaskWait2, KeyTaskWait2, KeyTaskWait2, KeyTaskWait2, KeyTaskWait2, KeyTaskWait2, KeyTaskWait2,
//     KeyTaskWait2, KeyTaskWait2, KeyTaskWait2, KeyTaskWait2, KeyTaskWait2, KeyTaskWait2, KeyTaskWait2, KeyTaskWait2,
//     KeyTaskFunc2, KeyTaskFunc2, KeyTaskFunc2, KeyTaskFunc2, KeyTaskFunc1, KeyTaskFunc1, KeyTaskFunc1, KeyTaskFunc1};
const pKeyTaskFunc InitKeyTaskTbl[32] =
    {KeyTaskFunc1, KeyTaskFunc1, KeyTaskFunc1, KeyTaskFunc1, KeyTaskFunc1, KeyTaskWait2, KeyTaskWait2, KeyTaskWait2,
     KeyTaskFunc2, KeyTaskFunc2, KeyTaskFunc2, KeyTaskFunc2, KeyTaskFunc2, KeyTaskFunc2, KeyTaskFunc2, KeyTaskFunc2,
     KeyTaskFunc2, KeyTaskFunc2, KeyTaskFunc2, KeyTaskFunc2, KeyTaskFunc1, KeyTaskFunc1, KeyTaskFunc1, KeyTaskFunc1,
     KeyTaskWait2, KeyTaskWait2, KeyTaskWait2, KeyTaskWait2, KeyTaskWait2, KeyTaskWait2, KeyTaskWait2, KeyTaskWait2};

void *KeyTaskFunc1(uint8_t *pKeyCount, uint32_t mask, uint32_t *rtv)
{ // 按下就起作用,多次起作用
    if (mask)
    {
        register uint8_t cnt = (*pKeyCount += 1);
        if ((cnt & 0x0F) == 2)
            *rtv |= mask;
        if (cnt >= 0x12)
            *pKeyCount = 0x10;
    }
    else
    {
        *pKeyCount = 0;
    }
    return &KeyTaskFunc1;
}

void *KeyTaskWait1(uint8_t *pKeyCount, uint32_t mask, uint32_t *rtv)
{ // 等待按键无效
    if (!mask)
        if ((*pKeyCount += 1) >= 5)
        {
            *pKeyCount = 0;
            return KeyTaskFunc1;
        }
    return &KeyTaskWait1;
}

void *KeyTaskFunc2(uint8_t *pKeyCount, uint32_t mask, uint32_t *rtv)
{ // 按下就起作用,只有一次起作用
    if (mask)
    {
        register uint8_t cnt = (*pKeyCount += 1);
        if (cnt >= 0x02 && cnt < 0x30)
        {
            *rtv |= mask;
            *pKeyCount = 0x31;
        }
        if (cnt >= 0x32)
            *pKeyCount = 0x31;
    }
    else
    {
        *pKeyCount = 0;
    }
    return &KeyTaskFunc2;
}
void *KeyTaskWait2(uint8_t *pKeyCount, uint32_t mask, uint32_t *rtv)
{ // 等待按键无效
    if (!mask)
        if ((*pKeyCount += 1) >= 5)
        {
            *pKeyCount = 0;
            return KeyTaskFunc2;
        }
    return &KeyTaskWait2;
}

void *KeyTaskWait0(uint8_t *pKeyCount, uint32_t mask, uint32_t *rtv)
{ // 等待按键无效
    return &KeyTaskWait0;
}

int KEY_SCAN(uint32_t mask)
{
    CPU_SR cpu_sr;
    mask = KeyPress & mask;
    if (mask)
    {
        CPU_CRITICAL_ENTER();
        KeyPress &= ~mask;
        CPU_CRITICAL_EXIT();
        return 1;
    }
    return 0;
}

uint32_t ReadKey(void)
{
    uint32_t key;
    key = GPIOC->IDR & 0x0f;
    key |= (GPIOA->IDR & 0x03) << 4;
    key |= ((GPIOA->IDR & 0xf0) >> 4) << 6;
    key |= ((GPIOC->IDR & 0x30) >> 4) << 10;
    return key;
}

void Master_key_proc(void)
{
    uint32_t key_map, tmp, mask;
    key_map = ReadKey();
    for (mask = tmp = 0; mask < sizeof(MainKeyIndex) / sizeof(uint32_t); mask++)
    {
        uint8_t index = MainKeyIndex[mask];
        MainKeyTaskTbl[index] = (pKeyTaskFunc)MainKeyTaskTbl[index]((uint8_t *)(&KeyCount[index]), key_map & (1UL << index), &tmp);
    }
    KeyPress |= tmp;
}

void Child_key_proc(uint8_t x)
{
    register uint8_t *pKeyCount;
    uint32_t key_map, tmp, mask;
    if (Child[x].pChildInput->Version1 != 0x49 || Child[x].pChildInput->Version2 != 0x50)
    {
        memset(Child[x].ChildKeyCount, 0, sizeof(Child[x].ChildKeyCount));
        for (mask = 0; mask < 32; mask++)
            KeyTaskTbl[x][mask] = InitKeyTaskTbl[mask];
        Child[x].ChildKeyPress = 0;
    }
    else if (Child[x].pChildInput->KeyCount != Child[x].KeyCount)
    {
        Child[x].KeyCount = Child[x].pChildInput->KeyCount;
        key_map = ChildReadKey(Child[x].pChildInput);
        
        for (pKeyCount = (uint8_t *)&Child[x].ChildKeyCount[mask = tmp = 0]; mask < 32; mask++, pKeyCount++)
            KeyTaskTbl[x][mask] = (pKeyTaskFunc)KeyTaskTbl[x][mask](pKeyCount, key_map & (1UL << mask), &tmp);
        Child[x].ChildKeyPress |= tmp;
			if(tmp)
        {
            uartprintf("tt%08x\n",tmp);
        }
    }
    else
    {
        memcpy(&KeyTaskTbl[x][0], InitKeyTaskTbl, sizeof(InitKeyTaskTbl));
        Child[x].ChildKeyPress = 0;
    }
}
