/*
 * BLE_programm.c
 *
 *  Created on: Jul 11, 2020
 *      Author: ra97
 */

#include "BLE_programm.h"

void ProgrammerInitGPIO(void)
{
  //init GPIO
  HAL_GPIO_WritePin(DD_Port, DD_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(DC_Port, DC_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(RESET_Port, RESET_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(LED1_Port, LED1_Pin, GPIO_PIN_SET);
}

void BLE_Programmer_main(void)
{
	ProgrammerInitGPIO();
	HAL_Delay(5000);
	while(1)
	{
		HAL_GPIO_WritePin(LED1_Port, LED1_Pin, GPIO_PIN_RESET);
		HAL_Delay(500);
		HAL_GPIO_WritePin(LED1_Port, LED1_Pin, GPIO_PIN_SET);
		HAL_Delay(500);
	}
}
