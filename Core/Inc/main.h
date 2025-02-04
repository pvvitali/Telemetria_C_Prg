/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#include "stm32f4xx_hal.h"

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
#define WIFI_ON_OFF_Pin GPIO_PIN_3
#define WIFI_ON_OFF_GPIO_Port GPIOC
#define W55_SCSN_Pin GPIO_PIN_4
#define W55_SCSN_GPIO_Port GPIOA
#define W55_RSTN_Pin GPIO_PIN_4
#define W55_RSTN_GPIO_Port GPIOC
#define W55_INTN_Pin GPIO_PIN_5
#define W55_INTN_GPIO_Port GPIOC
#define ON_ETHERNET_Pin GPIO_PIN_7
#define ON_ETHERNET_GPIO_Port GPIOE
#define ON_UART_CPU_Pin GPIO_PIN_12
#define ON_UART_CPU_GPIO_Port GPIOB
#define OUT_0_Pin GPIO_PIN_8
#define OUT_0_GPIO_Port GPIOD
#define OUT_1_Pin GPIO_PIN_9
#define OUT_1_GPIO_Port GPIOD
#define GATE_485_Pin GPIO_PIN_10
#define GATE_485_GPIO_Port GPIOD
#define WIFI_GPIO_0_Pin GPIO_PIN_13
#define WIFI_GPIO_0_GPIO_Port GPIOD
#define LED_Pin GPIO_PIN_14
#define LED_GPIO_Port GPIOD
#define RES_LCD_Pin GPIO_PIN_15
#define RES_LCD_GPIO_Port GPIOD
#define LED_LCD_Pin GPIO_PIN_6
#define LED_LCD_GPIO_Port GPIOC
#define DC_LCD_Pin GPIO_PIN_7
#define DC_LCD_GPIO_Port GPIOC
#define CS_LCD_Pin GPIO_PIN_8
#define CS_LCD_GPIO_Port GPIOC
#define USB_FAULT_Pin GPIO_PIN_9
#define USB_FAULT_GPIO_Port GPIOC
#define USB_ON_OFF_Pin GPIO_PIN_8
#define USB_ON_OFF_GPIO_Port GPIOA
#define GATE_V_SIM_Pin GPIO_PIN_0
#define GATE_V_SIM_GPIO_Port GPIOD
#define GATE_PWRKEY_Pin GPIO_PIN_1
#define GATE_PWRKEY_GPIO_Port GPIOD
#define BLUETOOTH_RES_Pin GPIO_PIN_3
#define BLUETOOTH_RES_GPIO_Port GPIOD
#define DOOR_Pin GPIO_PIN_7
#define DOOR_GPIO_Port GPIOD
#define GSM_STATUS_Pin GPIO_PIN_4
#define GSM_STATUS_GPIO_Port GPIOB
#define GSM_NETLIGHT_Pin GPIO_PIN_6
#define GSM_NETLIGHT_GPIO_Port GPIOB
#define BLUETOOTH_LED_Pin GPIO_PIN_7
#define BLUETOOTH_LED_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
