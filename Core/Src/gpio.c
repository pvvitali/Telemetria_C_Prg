/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
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

/* Includes ------------------------------------------------------------------*/
#include "gpio.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, WIFI_ON_OFF_Pin|W55_RSTN_Pin|LED_LCD_Pin|DC_LCD_Pin
                          |CS_LCD_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, W55_SCSN_Pin|USB_ON_OFF_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(ON_ETHERNET_GPIO_Port, ON_ETHERNET_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(ON_UART_CPU_GPIO_Port, ON_UART_CPU_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, OUT_0_Pin|OUT_1_Pin|WIFI_GPIO_0_Pin|LED_Pin
                          |RES_LCD_Pin|GATE_V_SIM_Pin|GATE_PWRKEY_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GATE_485_Pin|BLUETOOTH_RES_Pin, GPIO_PIN_SET);

  /*Configure GPIO pins : WIFI_ON_OFF_Pin LED_LCD_Pin DC_LCD_Pin CS_LCD_Pin */
  GPIO_InitStruct.Pin = WIFI_ON_OFF_Pin|LED_LCD_Pin|DC_LCD_Pin|CS_LCD_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : W55_SCSN_Pin */
  GPIO_InitStruct.Pin = W55_SCSN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(W55_SCSN_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : W55_RSTN_Pin */
  GPIO_InitStruct.Pin = W55_RSTN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(W55_RSTN_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : W55_INTN_Pin USB_FAULT_Pin */
  GPIO_InitStruct.Pin = W55_INTN_Pin|USB_FAULT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : ON_ETHERNET_Pin */
  GPIO_InitStruct.Pin = ON_ETHERNET_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(ON_ETHERNET_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : ON_UART_CPU_Pin */
  GPIO_InitStruct.Pin = ON_UART_CPU_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(ON_UART_CPU_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : OUT_0_Pin OUT_1_Pin WIFI_GPIO_0_Pin LED_Pin
                           RES_LCD_Pin GATE_V_SIM_Pin GATE_PWRKEY_Pin BLUETOOTH_RES_Pin */
  GPIO_InitStruct.Pin = OUT_0_Pin|OUT_1_Pin|WIFI_GPIO_0_Pin|LED_Pin
                          |RES_LCD_Pin|GATE_V_SIM_Pin|GATE_PWRKEY_Pin|BLUETOOTH_RES_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : GATE_485_Pin */
  GPIO_InitStruct.Pin = GATE_485_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GATE_485_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : USB_ON_OFF_Pin */
  GPIO_InitStruct.Pin = USB_ON_OFF_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(USB_ON_OFF_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : DOOR_Pin */
  GPIO_InitStruct.Pin = DOOR_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(DOOR_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : GSM_STATUS_Pin GSM_NETLIGHT_Pin BLUETOOTH_LED_Pin */
  GPIO_InitStruct.Pin = GSM_STATUS_Pin|GSM_NETLIGHT_Pin|BLUETOOTH_LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */
