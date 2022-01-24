/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
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
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

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
#define LED_1_Pin GPIO_PIN_13
#define LED_1_GPIO_Port GPIOC
#define B1_RST_Pin GPIO_PIN_0
#define B1_RST_GPIO_Port GPIOA
#define B2_RST_Pin GPIO_PIN_1
#define B2_RST_GPIO_Port GPIOA
#define ETH_RST_Pin GPIO_PIN_4
#define ETH_RST_GPIO_Port GPIOA
#define ETH_INT_Pin GPIO_PIN_0
#define ETH_INT_GPIO_Port GPIOB
#define ETH_CS_Pin GPIO_PIN_1
#define ETH_CS_GPIO_Port GPIOB
#define Flash_NSS_Pin GPIO_PIN_12
#define Flash_NSS_GPIO_Port GPIOB
#define BOOT_MODE_Pin GPIO_PIN_15
#define BOOT_MODE_GPIO_Port GPIOA
#define B1_DO_Pin GPIO_PIN_4
#define B1_DO_GPIO_Port GPIOB
#define B1_DC_Pin GPIO_PIN_5
#define B1_DC_GPIO_Port GPIOB
#define B2_DO_Pin GPIO_PIN_6
#define B2_DO_GPIO_Port GPIOB
#define B2_DC_Pin GPIO_PIN_7
#define B2_DC_GPIO_Port GPIOB
#define LED_2_Pin GPIO_PIN_8
#define LED_2_GPIO_Port GPIOB
#define LED_3_Pin GPIO_PIN_9
#define LED_3_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
