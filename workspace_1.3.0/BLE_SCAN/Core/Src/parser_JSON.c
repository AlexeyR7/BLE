/*
 * parser_JSON.c
 *
 *  Created on: 12 июн. 2020 г.
 *      Author: ra97
 */
#include "parser_JSON.h"
#define MAXANSWERLENGTH 6300 //7168
extern UART_HandleTypeDef huart2;
uint8_t JSON_DATA_SEND[MAXANSWERLENGTH];
char str2[500];



volatile uint32_t __UniqueID = 0x460A3330;


HAL_StatusTypeDef ParserBeacons(void)
{
	char buff[33];
	  jwOpen(JSON_DATA_SEND, MAXANSWERLENGTH, JW_OBJECT, JW_COMPACT );    // open root node as object
	  jwObj_int("uID", __UniqueID);
	  jwObj_int("Count", Beacons.count);
	  jwObj_array("Beacons");
	  int i =0;
		for(i=0;i<Beacons.count; i++){
		  	jwArr_object("BeaconI");
			memcpy(buff,Beacons.BLE_Beacons[i].P0_FactoryID,8);
			buff[8]='\0';
			jwObj_string("P0_FactoryID", buff);
			memcpy(buff,Beacons.BLE_Beacons[i].P1_iBeacon_UUID,32);
			buff[32]='\0';
			jwObj_string("P1_iBeacon_UUID", buff);
			memcpy(buff,Beacons.BLE_Beacons[i].P2_1MajorValue,4);
			buff[4]='\0';
			jwObj_string("P2_1MajorValue", buff);
			memcpy(buff,Beacons.BLE_Beacons[i].P2_2MinorValue,4);
			buff[4]='\0';
			jwObj_string("P2_2MinorValue", buff);
			memcpy(buff,Beacons.BLE_Beacons[i].P2_3MeasuredPower,2);
			buff[2]='\0';
			jwObj_string("P2_3MeasuredPower", buff);
			memcpy(buff,Beacons.BLE_Beacons[i].P3_MAC,12);
			buff[12]='\0';
			jwObj_string("P3_MAC", buff);
			memcpy(buff,Beacons.BLE_Beacons[i].P4_RSSI,4);
			buff[4]='\0';
			jwObj_string("P4_RSSI", buff);
			jwEnd();
		}
	  jwEnd();
	  int resultCode = jwClose();
	  if( resultCode != JWRITE_OK ) {JSON_DATA_SEND[MAXANSWERLENGTH-1]='\0';return HAL_ERROR;}
	  return HAL_OK;
}
