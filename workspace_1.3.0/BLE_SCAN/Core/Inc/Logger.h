/*
 * Logger.h
 *
 *  Created on: 12 июн. 2020 г.
 *      Author: ra97
 */

#ifndef SRC_LOGGER_H_
#define SRC_LOGGER_H_

#include "main.h"

#define LOGGER_UART_Level   0x01U

UART_HandleTypeDef *LOGGER_UART;


typedef enum{
		LOG_DEGUB    = 0x00U,
		LOG_INFO       = 0x01U,
		LOG_WARNING    = 0x02U,
		LOG_ERROR     = 0x03U
}LOG_TypeDef;
HAL_StatusTypeDef LOGGER_init(UART_HandleTypeDef huart);
void LOG(char *str_log, LOG_TypeDef Type);
void LOG_UART(char *str_log);

#endif /* SRC_LOGGER_H_ */
