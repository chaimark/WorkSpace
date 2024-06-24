/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.h
 * @brief          : Header for main.c file.
 *                   This file contains the common defines of the application.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2019 FMSH.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by FM under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "fm33lc0xx_ll_rcc.h"
#include "fm33lc0xx_ll_lptim.h"
#include "fm33lc0xx_ll_lpuart.h"
#include "fm33lc0xx_ll_gpio.h"
#include "fm33lc0xx_ll_uart.h"
#include "fm33lc0xx_ll_vref.h"
#include "fm33lc0xx_ll_iwdt.h"
#include "fm33lc0xx_ll_wwdt.h"
#include "fm33lc0xx_ll_pmu.h"
#include "fm33lc0xx_ll_flash.h"
#include "fm33lc0xx_ll_svd.h"
#include "fm33lc0xx_ll_aes.h"
#include "fm33lc0xx_ll_rmu.h"
#include "fm33lc0xx_ll_rng.h"
#include "fm33lc0xx_ll_opa.h"
#include "fm33lc0xx_ll_comp.h"
#include "fm33lc0xx_ll_hdiv.h"
#include "fm33lc0xx_ll_i2c.h"
#include "fm33lc0xx_ll_spi.h"
#include "fm33lc0xx_ll_u7816.h"
#include "fm33lc0xx_ll_bstim.h"
#include "fm33lc0xx_ll_gptim.h"
#include "fm33lc0xx_ll_atim.h"
#include "fm33lc0xx_ll_crc.h"
#include "fm33lc0xx_ll_dma.h"
#include "fm33lc0xx_ll_rtc.h"
#include "fm33lc0xx_ll_lcd.h"
#include "WifiTask.h"
#include "mf_config.h"
#include "Uart.h"
#include "User_Init.h"
  // #include "w600_wifi.h"

#if defined(USE_FULL_ASSERT)
#include "fm33_assert.h"
#endif /* USE_FULL_ASSERT */

  /* Private includes ----------------------------------------------------------*/
  /* USER CODE BEGIN Includes */

  /* USER CODE END Includes */

  /* Exported types ------------------------------------------------------------*/
  /* USER CODE BEGIN ET */

  /* USER CODE END ET */

  /* Exported constants --------------------------------------------------------*/
  /* USER CODE BEGIN EC */

  /* USER CODE END EC */

  /* Exported macro ------------------------------------------------------------*/
  /* USER CODE BEGIN EM */

  /* USER CODE END EM */

  /* Exported functions prototypes ---------------------------------------------*/
  void Error_Handler(void);

  /* USER CODE BEGIN EFP */

  /* USER CODE END EFP */

  /* Private defines -----------------------------------------------------------*/

  /* USER CODE BEGIN Private defines */

  /* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

extern void copyDataForUART(void);
extern void IWDT_Clr(void);
extern void BSTIM_Init(uint16_t ms);
extern void wifi_task(void);
extern void wifi_timer_clean(void);
extern void wifi_timer_add(void);

// extern void Sys_Enter_Standby(void);
// extern void Radio_SetSleep( void );
// extern void Radio_Wakeup( void );
// extern void Radio_SetStandby( void );

#include "JsonApp.h"
// HTTP_Ser 的接口转换定义
#define WIFI_IDFlag WIFI_GatewayFlag
#define HTTPBuff_p Send_Task.WifiSendAcceptBuff.Name._char
#define HTTPBuff_NowLen Http_AccepSendBuff_Len
#define HTTPBuff_MaxLen WIFI_BUFF_MAXLEN
#define CatString catString
#define CopyString copyString

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT FMSH *****END OF FILE****/
