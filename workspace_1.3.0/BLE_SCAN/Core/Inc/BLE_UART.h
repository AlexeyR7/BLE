/*
 * BLE_UART.h
 *
 *  Created on: 11 июн. 2020 г.
 *      Author: ra97
 */

#ifndef INC_BLE_UART_H_
#define INC_BLE_UART_H_
#include "main.h"
UART_HandleTypeDef *BLE_UART;

#define CMD_AT "AT"


#define maxCountBeacon 32//32

typedef struct {
char P0_FactoryID[8];
char P1_iBeacon_UUID[32];
char P2_1MajorValue[4];
char P2_2MinorValue[4];
char P2_3MeasuredPower[2];
char P3_MAC[12];
char P4_RSSI[4];
} BLE_Beacon;


typedef struct {
	uint8_t count;
	BLE_Beacon BLE_Beacons[maxCountBeacon];
}Mass_Beacon;

Mass_Beacon Beacons;

typedef enum
{
  BLE_RCV_STOP       = 0x00U,
  BLE_RCV_ACTIVE    = 0x01U,
  BLE_RCV_READY    = 0x02U,
  BLE_Recive_DISI    = 0x03U,
  BLE_Recive_DISC    = 0x04U,
  BLE_Recive_DISC_beacon = 0x05U,
  BLE_Recive_CALLBACKERROR    = 0x0FU,
  BLE_Recive_ERROR    = 0xFFU
} RCV_STAT;
RCV_STAT RCV_Status;

uint8_t NadoPodnyat;





void copyStructBLE_Beacon(BLE_Beacon *s1, BLE_Beacon *s2);

HAL_StatusTypeDef BLE_init(UART_HandleTypeDef *huart);
HAL_StatusTypeDef BLE_CMD_AT(void);
void BLE_CMD_DISI(void);
void BLE_Callback();
void BLE_Search_Start(void);
void BLE_Search_Stop(void);
void  BLE_Search_up(void);

#endif /* INC_BLE_UART_H_ */
