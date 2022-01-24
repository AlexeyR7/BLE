/*
 * BLE_programm.h
 *
 *  Created on: Jul 11, 2020
 *      Author: ra97
 */

#ifndef INC_BLE_PROGRAMM_H_
#define INC_BLE_PROGRAMM_H_

#include "main.h"
// Commands
#define SBEGIN			0x01
#define SDATA			0x02
#define SRSP			0x03
#define SEND			0x04
#define ERRO			0x05
#define WAITING			0x00
#define RECEIVING		0x01

//Pins, Ports
#define RESET_Pin 	BLE_RST_Pin
#define RESET_Port 	BLE_RST_GPIO_Port

#define DD_Pin 		BLE_DD_Pin
#define DD_Port 	BLE_DD_GPIO_Port

#define DC_Pin 		BLE_DC_Pin
#define DC_Port 	BLE_DC_GPIO_Port

#define LED1_Pin 	LED_D2_Pin
#define LED1_Port 	LED_D2_GPIO_Port




void ProgrammerInitGPIO(void);
void BLE_Programmer_main(void);

#endif /* INC_BLE_PROGRAMM_H_ */
