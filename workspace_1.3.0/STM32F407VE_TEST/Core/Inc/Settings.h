/*
 * Settings.h
 *
 *  Created on: Jul 11, 2020
 *      Author: ra97
 */

#ifndef INC_SETTINGS_H_
#define INC_SETTINGS_H_

typedef struct {
	//System
	uint8_t uID[32];
	uint8_t version[8];
	//Network Lan
	uint8_t Network_LAN_IP_ADDR[4];
	uint8_t Network_LAN_IP_GATE[4];
	uint8_t Network_LAN_IP_MASK[4];
	uint8_t Network_LAN_LOCAL_PORT[16];
	//Network WiFi
	uint8_t Network_WiFi_IP_ADDR[4];
	uint8_t Network_WiFi_IP_GATE[4];
	uint8_t Network_WiFi_IP_MASK[4];
	uint8_t Network_WiFi_LOCAL_PORT[16];
	//Network server
	uint8_t Network_Server_LAN_IP_ADDR[4];
	uint8_t Network_Server_LAN_LOCAL_PORT[16];
	uint8_t Network_Server_WiFi_IP_ADDR[4];
	uint8_t Network_Server_WiFi_LOCAL_PORT[16];


	//Scaner BLE
	uint8_t P2_3MeasuredPower[2];
	uint8_t P3_MAC[12];
	uint8_t P4_RSSI[4];
} Settings_BLE;


//
typedef struct {

char test[4];
} CRC_BLE;

#endif /* INC_SETTINGS_H_ */
