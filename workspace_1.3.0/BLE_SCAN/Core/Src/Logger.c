/*
 * Logger.cpp
 *
 *  Created on: 12 июн. 2020 г.
 *      Author: ra97
 */

#include "Logger.h"

char str_log[128]={0};

HAL_StatusTypeDef LOGGER_init(UART_HandleTypeDef huart){

	LOGGER_UART = &huart;

	return HAL_OK;
}


void LOG(char *str_log, LOG_TypeDef Type)
{
	if(Type>=LOGGER_UART_Level){
		LOG_UART(str_log);
	}

}


void LOG_UART(char *str_log)
{
	HAL_UART_Transmit(LOGGER_UART,(uint8_t*)str_log, strlen(str_log),0x1000);
	HAL_UART_Transmit(LOGGER_UART,(uint8_t*)"\r\n",strlen("\r\n"),0x1000);
}
