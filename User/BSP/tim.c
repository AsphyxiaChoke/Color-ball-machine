/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    tim.c
 * @brief   This file provides code for the configuration
 *          of the TIM instances.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "tim.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

TIM_HandleTypeDef htim2;

#define COINERRORVALUE 10

/* TIM2 init function */
void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 19999;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 499;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *tim_baseHandle)
{

  if (tim_baseHandle->Instance == TIM2)
  {
    /* USER CODE BEGIN TIM2_MspInit 0 */

    /* USER CODE END TIM2_MspInit 0 */
    /* TIM2 clock enable */
    __HAL_RCC_TIM2_CLK_ENABLE();

    /* TIM2 interrupt Init */
    HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM2_IRQn);
    /* USER CODE BEGIN TIM2_MspInit 1 */
    if (HAL_TIM_Base_Start_IT(&htim2) != HAL_OK)
    {
      Error_Handler();
    }
    /* USER CODE END TIM2_MspInit 1 */
  }
}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef *tim_baseHandle)
{

  if (tim_baseHandle->Instance == TIM2)
  {
    /* USER CODE BEGIN TIM2_MspDeInit 0 */

    /* USER CODE END TIM2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM2_CLK_DISABLE();

    /* TIM2 interrupt Deinit */
    HAL_NVIC_DisableIRQ(TIM2_IRQn);
    /* USER CODE BEGIN TIM2_MspDeInit 1 */

    /* USER CODE END TIM2_MspDeInit 1 */
  }
}
// 定时器2中断处理函数
void TIM2_IRQHandler(void)
{
  OSIntEnter();
  HAL_TIM_IRQHandler(&htim2);
  OSIntExit();
}

/* USER CODE BEGIN 1 */
#define TOU_INIT_COUNT (150)
uint32_t ChildReadCoinTou(ChildType *pChild)
{
  uint8_t Temp1, Temp2;
  uint32_t TouCoin = 0;
#if MAX_FENJI == 16
  if (pChild->ChildNum & 0x01)
  {
    Temp1 = pChild->pChildInput->Tou2Num1;
    Temp2 = pChild->pChildInput->Tou2Num2;
  }
  else
  {
    Temp1 = pChild->pChildInput->TouNum1;
    Temp2 = pChild->pChildInput->TouNum2;
  }
#else
  Temp1 = pChild->pChildInput->TouNum1;
  Temp2 = pChild->pChildInput->TouNum2;
#endif
  if (Temp1 == Temp2) // 防止开机抖动投退币
  {
    if (pChild->TouInitCount < TOU_INIT_COUNT)
    {
      // if(pChild->TouOldCount==0 && Temp1==pChild->TouOldCount)      //初始化阶段不处理投币
      {
        pChild->TouInitCount++;
      }
      pChild->TouOldCount = Temp1;
    }
    else if (Temp1 != pChild->TouOldCount)
    {
      //		    _XTMSG("i %d,%d\n",pChild->TouOldCount,Temp1);
      TouCoin = (Temp1 >= pChild->TouOldCount) ? (Temp1 - pChild->TouOldCount) : (Temp1 + 256 - pChild->TouOldCount);
      pChild->TouOldCount = Temp1;
    }
  }
  return TouCoin;
}

uint32_t ChildReadCoinTui(ChildType *pChild)
{
  uint8_t Temp1, Temp2;
  uint32_t TuiCoin = 0;
#if MAX_FENJI == 16
  if (pChild->ChildNum & 0x01)
  {
    Temp1 = pChild->pChildInput->Tui2Num1;
    Temp2 = pChild->pChildInput->Tui2Num2;
  }
  else
  {
    Temp1 = pChild->pChildInput->TuiNum1;
    Temp2 = pChild->pChildInput->TuiNum2;
  }
#else
  Temp1 = pChild->pChildInput->TuiNum1;
  Temp2 = pChild->pChildInput->TuiNum2;
#endif
  if (Temp1 == Temp2) // 防止开机抖动投退币
  {
    if (Temp1 != pChild->TuiOldCount && pChild->TouInitCount >= TOU_INIT_COUNT)
    {
      TuiCoin = (Temp1 >= pChild->TuiOldCount) ? (Temp1 - pChild->TuiOldCount) : (Temp1 + 256 - pChild->TuiOldCount);
      pChild->TuiOldCount = Temp1;
    }
  }
  return TuiCoin;
}

void CHILD_COIN_OUT(ChildType *pChild, uint8_t x)
{
  uint32_t coin = GetCoinOut(&DipSw);
  uint8_t count8, sum = 0;
  uint32_t close = 0;
  uint32_t FenJiCoin = 0;
  pChild->PcConectCnt++;
	pChild->pChildDisp->PcConectCnt = pChild->PcConectCnt;
	pChild->pChildDisp->NewTuiFlag = CHILDFLG;
  count8 = pChild->pChildInput->KeyCount;
  sum = pChild->pChildInput->TouNum1 +
        pChild->pChildInput->TouNum2 +
        pChild->pChildInput->TuiNum1 +
        pChild->pChildInput->TuiNum2;
  if ((count8 != pChild->CoinKeyCount) &&
      pChild->pChildInput->Version1 == 0x49 &&
      pChild->pChildInput->Version2 == 0x50 &&
      pChild->pChildInput->Version3 == 0x99 &&
      pChild->pChildInput->Version4 == 0x88 &&
      pChild->pChildInput->CheckSum == sum)
  {
    pChild->CoinKeyCount = count8;
    // 处理投币
    FenJiCoin = ChildReadCoinTou(pChild);
    pChild->CoinInTmp += FenJiCoin;
    // 处理退币
    FenJiCoin = ChildReadCoinTui(pChild);
    if (pChild->DispCount & SSR_OPEN) // 开马达才计退币数
    {
      pChild->TuiProcCount++; // 继续退币
      if (FenJiCoin > 0)
      {
        pChild->CoinOutTmp += FenJiCoin;
        // if (pChild->TuiBiNum>=(pChild->CoinOutTmp-pChild->CoinOutTmp2+1))
        if (pChild->Credit >= coin * (pChild->CoinOutTmp - pChild->CoinOutTmp2 + 1))
        {
          pChild->CoinOutTime = 15 * COIN_TIME_SPEED;
        }
        else
        {
          close = 1;
        }
      }
    }
  }
  if (pChild->DispCount & SSR_OPEN)
  {
    if (pChild->CoinOutTime <= 0)
    {
      close = 1;
    }
    else
    {
      --pChild->CoinOutTime;
    }
    if (close)
      CloseSSR(pChild);
  }

}
// 定时器2中断回调函数
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM2)
  {
    register uint8_t i;
    for (i = 0; i < MAX_FENJI; i++)
    {
      CHILD_COIN_OUT(&Child[i], i);
    }
  }
}

/* USER CODE END 1 */
