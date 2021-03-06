/*
 * BLE_UART.c
 *
 *  Created on: 11 июн. 2020 г.
 *      Author: ra97
 */

#include "BLE_UART.h"

#define Data_RCV_size 100

uint8_t Data_RCV[Data_RCV_size];
uint8_t Data_RCV_buf_uart[Data_RCV_size];
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart1;
extern IWDG_HandleTypeDef hiwdg;
int status_RCV=0;

extern uint8_t Data_Send1[256];
extern char str1[];




HAL_StatusTypeDef BLE_init(UART_HandleTypeDef *huart){
	BLE_UART = huart;
	Beacons.count=0;

	sprintf(str1,"BLE_initT()\r\n");
				HAL_UART_Transmit(&huart2,(uint8_t*)str1,strlen(str1),0x1000);
	return HAL_OK;

}


HAL_StatusTypeDef BLE_CMD_AT(void){
	return HAL_ERROR;/*
	HAL_StatusTypeDef status;
	uint8_t buf[10];
	HAL_UART_Transmit(BLE_UAR,(uint8_t*)CMD_AT,strlen(CMD_AT),100);
	status = HAL_UART_Receive(BLE_UAR, (uint8_t*)buf, 2, 500);
	if(status != HAL_OK )return status;
	if((buf[0]=='O')&&(buf[1]=='K')) return HAL_OK;
	sprintf(str1,"BLE_CMD_AT HAL_ERROR\r\n");
			HAL_UART_Transmit(&huart2,(uint8_t*)str1,strlen(str1),0x1000);
	return HAL_ERROR;*/
}

void copyStructBLE_Beacon(BLE_Beacon *s1, BLE_Beacon *s2)
{
	int i=0;
	for(i=0;i<8;i++)s1->P0_FactoryID[i]=s2->P0_FactoryID[i];
	for(i=0;i<32;i++)s1->P1_iBeacon_UUID[i]=s2->P1_iBeacon_UUID[i];
	for(i=0;i<4;i++)s1->P2_1MajorValue[i]=s2->P2_1MajorValue[i];
	for(i=0;i<4;i++)s1->P2_2MinorValue[i]=s2->P2_2MinorValue[i];
	for(i=0;i<2;i++)s1->P2_3MeasuredPower[i]=s2->P2_3MeasuredPower[i];
	for(i=0;i<12;i++)s1->P3_MAC[i]=s2->P3_MAC[i];
	for(i=0;i<4;i++)s1->P4_RSSI[i]=s2->P4_RSSI[i];
}

uint8_t Dcmp(uint8_t *s1, uint8_t *s2, uint8_t N)
{

	for(int i=0;i< N; i++)
		if(s1[i]!=s2[i])return 0;
	return 1;
}







void BLE_Search_Start(void)
{

	if(RCV_Status == BLE_RCV_STOP){
	RCV_Status = BLE_RCV_READY;
	BLE_CMD_DISI();
	}

}
void  BLE_Search_Stop(void)
{
	if(RCV_Status != BLE_RCV_STOP){
		RCV_Status = BLE_RCV_STOP;
	}
}
void  BLE_Search_up(void)
{
	if(NadoPodnyat){
		HAL_UART_Abort_IT(BLE_UART);
		sprintf(str1,"BLE_Search_up()-----%s----\r\n",Data_RCV);
				HAL_UART_Transmit(&huart2,(uint8_t*)str1,strlen(str1),0x1000);
		RCV_Status = BLE_RCV_STOP;
		//HAL_Delay(5500);///очень очень плохо
		for(int i=0; i<200;i++){//100
			HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
				    HAL_Delay(25);
				    HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
				    HAL_Delay(25);
		}
		RCV_Status = BLE_RCV_READY;
		NadoPodnyat=0;
		BLE_CMD_DISI();
	}
}

int kk;
void BLE_CMD_DISI(void){
	if(RCV_Status == BLE_RCV_STOP) return;
	if(RCV_Status == BLE_Recive_CALLBACKERROR) return;
	//OK+DISCS
	//OK+DISC:[P0:P1:P2:P3:P4]
	//OK+DISC:[P0:P1:P2:P3:P4]
	//...
	//OK+DISCE

	BLE_Beacon iBeacon;
	int i=0;
	int j=0;
	switch (RCV_Status)
	{

	case BLE_RCV_READY:
		Beacons.count=0;
		HAL_UART_Transmit(BLE_UART,"AT+DISI?",strlen("AT+DISI?"),100);
	    if(HAL_UART_Receive_IT(BLE_UART, (uint8_t*)Data_RCV_buf_uart, 8)!=HAL_OK){
	    	sprintf(str1,"HAL_UART_Receive_IT ERROR\r\n");
	    					HAL_UART_Transmit(&huart2,(uint8_t*)str1,strlen(str1),0x1000);
	    }
	    RCV_Status = BLE_Recive_DISI;
		break;
	case BLE_Recive_DISI:
		kk=0;
		Beacons.count=0;
		if(!Dcmp(Data_RCV,(uint8_t*)"OK+DISIS",8)) {RCV_Status = BLE_Recive_ERROR; break;}
	    HAL_UART_Receive_IT(BLE_UART, (uint8_t*)Data_RCV_buf_uart, 8);
	    RCV_Status = BLE_Recive_DISC;

		break;
	case BLE_Recive_DISC: //данные о метке
		if(!Dcmp(&Data_RCV[0],(uint8_t*)"OK+DISC",7)) {RCV_Status = BLE_Recive_ERROR; break;}
		if(Data_RCV[7]=='E') {///поиск завершен
			//calcDistanse();
			ParserBeacons();
			HAL_UART_Transmit(BLE_UART,"AT+DISI?",strlen("AT+DISI?"),100);

			HAL_UART_Receive_IT(BLE_UART, (uint8_t*)Data_RCV_buf_uart, 8);
			RCV_Status = BLE_Recive_DISI;
		}
		else if(Data_RCV[7]==':') {
			RCV_Status = BLE_Recive_DISC_beacon;
			HAL_UART_Receive_IT(BLE_UART, (uint8_t*)Data_RCV_buf_uart, 70);
		}
		else{
			sprintf(str1,"Data_RCV[7] ERROR\r\n");
				    					HAL_UART_Transmit(&huart2,(uint8_t*)str1,strlen(str1),0x1000);
			RCV_Status = BLE_Recive_ERROR; break;}

		break;
	case BLE_Recive_DISC_beacon:
		j=0;
		for(i=0;i<8;i++) iBeacon.P0_FactoryID[i]= Data_RCV[j++];

		if(Data_RCV[j++]!=':') {RCV_Status = BLE_Recive_ERROR; break;}
		for(i=0;i<32;i++) iBeacon.P1_iBeacon_UUID[i]= Data_RCV[j++];

		if(Data_RCV[j++]!=':') {RCV_Status = BLE_Recive_ERROR; break;}
		for(i=0;i<4;i++) iBeacon.P2_1MajorValue[i]= Data_RCV[j++];
		for(i=0;i<4;i++) iBeacon.P2_2MinorValue[i]= Data_RCV[j++];
		for(i=0;i<2;i++) iBeacon.P2_3MeasuredPower[i]= Data_RCV[j++];

		if(Data_RCV[j++]!=':') {RCV_Status = BLE_Recive_ERROR; break;}
		for(i=0;i<12;i++) iBeacon.P3_MAC[i]= Data_RCV[j++];

		if(Data_RCV[j++]!=':') {RCV_Status = BLE_Recive_ERROR; break;}
		for(i=0;i<4;i++) iBeacon.P4_RSSI[i]= Data_RCV[j++];
		Beacons.count++;
		RCV_Status = BLE_Recive_DISC;

		copyStructBLE_Beacon(&Beacons.BLE_Beacons[kk], &iBeacon);
		kk++;
		HAL_UART_Receive_IT(BLE_UART, (uint8_t*)Data_RCV_buf_uart, 8);



		break;
	default:
	break;

	}

	if(RCV_Status == BLE_Recive_ERROR)
	{
		sprintf(str1,"ERROR j=%d\r\n",j);
		HAL_UART_Transmit(&huart2,(uint8_t*)str1,strlen(str1),0x1000);

		RCV_Status = BLE_RCV_READY;
		HAL_UART_Transmit(BLE_UART,"AT",strlen("AT"),100);
		    HAL_UART_Receive_IT(BLE_UART, (uint8_t*)Data_RCV_buf_uart, 2);

	}

	HAL_IWDG_Refresh(&hiwdg);
}





void BLE_Callback()
{
	//проверить какая команда выполнялась до

  memcpy(Data_RCV,Data_RCV_buf_uart,Data_RCV_size);//сохранить данные. буффер мб не нужен
  BLE_CMD_DISI();
}



