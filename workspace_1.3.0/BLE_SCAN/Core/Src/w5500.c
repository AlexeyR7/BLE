#include "w5500.h"
#include "main.h"
//-----------------------------------------------
extern SPI_HandleTypeDef hspi1;

//-----------------------------------------------
extern char str1[60];
char tmpbuf[30];
uint8_t sect[515];
extern http_sock_prop_ptr httpsockprop[8];
//-----------------------------------------------
uint8_t macaddr[6]=MAC_ADDR;
extern uint8_t ipaddr[4];
extern uint8_t ipgate[4];
extern uint8_t ipmask[4];
extern uint16_t local_port;
//-----------------------------------------------
void w5500_writeReg(uint8_t op, uint16_t addres, uint8_t data)
{
  uint8_t buf[] = {addres >> 8, addres, op|(RWB_WRITE<<2), data};
  SS_SELECT();
  HAL_SPI_Transmit(&hspi1, buf, 4, 0xFFFFFFFF);
  SS_DESELECT();
}
//-----------------------------------------------
void w5500_writeBuf(data_sect_ptr *datasect, uint16_t len)
{
  SS_SELECT();
  HAL_SPI_Transmit(&hspi1, (uint8_t*) datasect, len, 0xFFFFFFFF);
  SS_DESELECT();
}
//-----------------------------------------------
void w5500_writeSockBuf(uint8_t sock_num, uint16_t point, uint8_t *buf, uint16_t len)
{
  data_sect_ptr *datasect = (void*)buf;
  datasect->opcode = (((sock_num<<2)|BSB_S0_TX)<<3)|(RWB_WRITE<<2)|OM_FDM0;
  datasect->addr = be16toword(point);
  w5500_writeBuf(datasect,len+3);//3 служебных байта
}
//-----------------------------------------------

uint8_t w5500_readReg(uint8_t op, uint16_t addres)
{
  uint8_t data;
  uint8_t wbuf[] = {addres >> 8, addres, op, 0x0};
  uint8_t rbuf[4];
  SS_SELECT();
  HAL_SPI_TransmitReceive(&hspi1, wbuf, rbuf, 4, 0xFFFFFFFF);
  SS_DESELECT();
  data = rbuf[3];
  return data;
}
//-----------------------------------------------
void w5500_readBuf(data_sect_ptr *datasect, uint16_t len)
{
  SS_SELECT();
  HAL_SPI_Transmit(&hspi1, (uint8_t*) datasect, 3, 0xFFFFFFFF);
  HAL_SPI_Receive(&hspi1, (uint8_t*) datasect, len, 0xFFFFFFFF);
  SS_DESELECT();
}
//-----------------------------------------------
uint8_t w5500_readSockBufByte(uint8_t sock_num, uint16_t point)
{
  uint8_t opcode, bt;
  opcode = (((sock_num<<2)|BSB_S0_RX)<<3)|OM_FDM1;
  bt = w5500_readReg(opcode, point);
  return bt;
}
//-----------------------------------------------
void w5500_readSockBuf(uint8_t sock_num, uint16_t point, uint8_t *buf, uint16_t len)
{
  data_sect_ptr *datasect = (void*)buf;
  datasect->opcode = (((sock_num<<2)|BSB_S0_RX)<<3)|OM_FDM0;
  datasect->addr = be16toword(point);
  w5500_readBuf(datasect,len);
}
//-----------------------------------------------
void SetSockPort(uint8_t sock_num, uint16_t port)

{
  uint8_t opcode;
  opcode = (((sock_num<<2)|BSB_S0)<<3)|OM_FDM1;
  w5500_writeReg(opcode, Sn_PORT0,port>>8);
  w5500_writeReg(opcode, Sn_PORT1,port);
}
//-----------------------------------------------
void OpenSocket(uint8_t sock_num, uint16_t mode)
{
  uint8_t opcode=0;
  opcode = (((sock_num<<2)|BSB_S0)<<3)|OM_FDM1;
  w5500_writeReg(opcode, Sn_MR, mode);
  w5500_writeReg(opcode, Sn_CR, 0x01);
}
//-----------------------------------------------
void SocketInitWait(uint8_t sock_num)
{
  uint8_t opcode=0;
  opcode = (((sock_num<<2)|BSB_S0)<<3)|OM_FDM1;
  while(1)
  {
    if(w5500_readReg(opcode, Sn_SR)==SOCK_INIT)
    {
      break;
    }
  }
}
//-----------------------------------------------
void ListenSocket(uint8_t sock_num)
{
  uint8_t opcode=0;
  opcode = (((sock_num<<2)|BSB_S0)<<3)|OM_FDM1;
  w5500_writeReg(opcode, Sn_CR, 0x02); //LISTEN SOCKET
}
//-----------------------------------------------
void SocketListenWait(uint8_t sock_num)
{
  uint8_t opcode=0;
  opcode = (((sock_num<<2)|BSB_S0)<<3)|OM_FDM1;
  while(1)
  {
    if(w5500_readReg(opcode, Sn_SR)==SOCK_LISTEN)
    {
      break;
    }
  }
}
//-----------------------------------------------
void SocketClosedWait(uint8_t sock_num)
{
  uint8_t opcode=0;
  opcode = (((sock_num<<2)|BSB_S0)<<3)|OM_FDM1;
  while(1)
  {
    if(w5500_readReg(opcode, Sn_SR)==SOCK_CLOSED)
    {
      break;
    }
  }
}
//-----------------------------------------------
void DisconnectSocket(uint8_t sock_num)
{
  uint8_t opcode=0;
  opcode = (((sock_num<<2)|BSB_S0)<<3)|OM_FDM1;
  w5500_writeReg(opcode, Sn_CR, 0x08); //DISCON
}
//-----------------------------------------------
uint8_t GetSocketStatus(uint8_t sock_num)
{
  uint8_t dt;
  uint8_t opcode=0;
  opcode = (((sock_num<<2)|BSB_S0)<<3)|OM_FDM1;
  dt = w5500_readReg(opcode, Sn_SR);
  return dt;
}
//-----------------------------------------------
void RecvSocket(uint8_t sock_num)
{
  uint8_t opcode;
  opcode = (((sock_num<<2)|BSB_S0)<<3)|OM_FDM1;
  w5500_writeReg(opcode, Sn_CR, 0x40); //RECV SOCKET
}
//-----------------------------------------------
void SendSocket(uint8_t sock_num)
{
  uint8_t opcode;
  opcode = (((sock_num<<2)|BSB_S0)<<3)|OM_FDM1;
  w5500_writeReg(opcode, Sn_CR, 0x20); //SEND SOCKET
}
//-----------------------------------------------
uint16_t GetSizeRX(uint8_t sock_num)
{
  uint16_t len;
  uint8_t opcode=0;
  opcode = (((sock_num<<2)|BSB_S0)<<3)|OM_FDM1;
  len = (w5500_readReg(opcode,Sn_RX_RSR0)<<8|w5500_readReg(opcode,Sn_RX_RSR1));
  return len;
}
//-----------------------------------------------
uint16_t GetReadPointer(uint8_t sock_num)
{
  uint16_t point;
  uint8_t opcode;
  opcode = (((sock_num<<2)|BSB_S0)<<3)|OM_FDM1;
  point = (w5500_readReg(opcode,Sn_RX_RD0)<<8|w5500_readReg(opcode,Sn_RX_RD1));
  return point;
}
//-----------------------------------------------
uint16_t GetWritePointer(uint8_t sock_num)
{
  uint16_t point;
  uint8_t opcode;
  opcode = (((sock_num<<2)|BSB_S0)<<3)|OM_FDM1;
  point = (w5500_readReg(opcode,Sn_TX_WR0)<<8|w5500_readReg(opcode,Sn_TX_WR1));
  return point;
}
//-----------------------------------------------
void SetWritePointer(uint8_t sock_num, uint16_t point)
{
  uint8_t opcode;
  opcode = (((sock_num<<2)|BSB_S0)<<3)|OM_FDM1;
  w5500_writeReg(opcode, Sn_TX_WR0, point>>8);
  w5500_writeReg(opcode, Sn_TX_WR1, (uint8_t)point);
}
//-----------------------------------------------
void w5500_ini(void)
{
	uint8_t i;
  uint8_t opcode=0;
  //Hard Reset
  HAL_GPIO_WritePin(RESET_GPIO_Port, RESET_Pin, GPIO_PIN_RESET);
    HAL_Delay(70);
    HAL_GPIO_WritePin(RESET_GPIO_Port, RESET_Pin, GPIO_PIN_SET);
    HAL_Delay(70);
	//Soft Reset
	opcode = (BSB_COMMON<<3)|OM_FDM1;
	w5500_writeReg(opcode, MR, 0x80);
	HAL_Delay(100);
	//Configute Net
	w5500_writeReg(opcode, SHAR0,macaddr[0]);
	w5500_writeReg(opcode, SHAR1,macaddr[1]);
	w5500_writeReg(opcode, SHAR2,macaddr[2]);
	w5500_writeReg(opcode, SHAR3,macaddr[3]);
	w5500_writeReg(opcode, SHAR4,macaddr[4]);
	w5500_writeReg(opcode, SHAR5,macaddr[5]);
	w5500_writeReg(opcode, GWR0,ipgate[0]);
	w5500_writeReg(opcode, GWR1,ipgate[1]);
	w5500_writeReg(opcode, GWR2,ipgate[2]);
	w5500_writeReg(opcode, GWR3,ipgate[3]);
	w5500_writeReg(opcode, SUBR0,ipmask[0]);
	w5500_writeReg(opcode, SUBR1,ipmask[1]);
	w5500_writeReg(opcode, SUBR2,ipmask[2]);
	w5500_writeReg(opcode, SUBR3,ipmask[3]);
	w5500_writeReg(opcode, SIPR0,ipaddr[0]);
	w5500_writeReg(opcode, SIPR1,ipaddr[1]);
	w5500_writeReg(opcode, SIPR2,ipaddr[2]);
	w5500_writeReg(opcode, SIPR3,ipaddr[3]);
	for(i=3;i<8;i++)
	  {
	    SetSockPort(i, local_port);
	    //Открываем сокет
	    OpenSocket(i,Mode_TCP);
	    SocketInitWait(i);
	    //Начинаем слушать сокет
	    ListenSocket(i);
	    SocketListenWait(i);
	  }
	  HAL_Delay(500);
	  //Посмотрим статусы
	  for(i=3;i<8;i++)
	  {
	   // dtt = GetSocketStatus(i);
	    //sprintf(str1,"First Status Sn%d: 0x%02X\r\n",i,dtt);
	    //HAL_UART_Transmit(&huart2,(uint8_t*)str1,strlen(str1),0x1000);
	  }
}
//-----------------------------------------------
void w5500_packetReceive(uint8_t sn)
{
  uint16_t point;
  uint16_t len;
	if(GetSocketStatus(sn)==SOCK_ESTABLISHED)
	{

		if(httpsockprop[sn].data_stat == DATA_COMPLETED)
		{
			//Отобразим размер принятых данных
			len = GetSizeRX(sn);
			//sprintf(str1,"len_rx_buf:0x%04X\r\n",len);
			//HAL_UART_Transmit(&huart2,(uint8_t*)str1,strlen(str1),0x1000);
			//Если пришел пустой пакет, то уходим из функции
			if(!len){
				DisconnectSocket(sn); //Разъединяемся
				        SocketClosedWait(sn);
				        OpenSocket(sn,Mode_TCP);
				        //Ждём инициализации сокета (статус SOCK_INIT)
				        SocketInitWait(sn);
				        //Продолжаем слушать сокет
				        ListenSocket(sn);
				        SocketListenWait(sn);
				        return;
			}
			//здесь обмениваемся информацией: на запрос документа от клиента отправляем ему запрошенный документ
			//указатель на начало чтения приёмного буфера
			point = GetReadPointer(sn);
			//sprintf(str1,"Sn_RX_RD:0x%04X\r\n",point);
			//HAL_UART_Transmit(&huart2,(uint8_t*)str1,strlen(str1),0x1000);
			w5500_readSockBuf(sn, point, (uint8_t*)tmpbuf, 5);
			if (strncmp(tmpbuf,"GET /", 5) == 0)
			{
				//HAL_UART_Transmit(&huart2,(uint8_t*)"HTTP\r\n",6,0x1000);
				httpsockprop[sn].prt_tp = PRT_TCP_HTTP;
				http_request(sn);
			}
			else
			{

			}
		}
		else if(httpsockprop[sn].data_stat==DATA_MIDDLE)
    {
      if(httpsockprop[sn].prt_tp == PRT_TCP_HTTP)
      {
				tcp_send_http_middle(sn);
      }
    }
    else if(httpsockprop[sn].data_stat==DATA_LAST)
    {
      if(httpsockprop[sn].prt_tp == PRT_TCP_HTTP)
      {
				tcp_send_http_last(sn);
        DisconnectSocket(sn); //Разъединяемся
        SocketClosedWait(sn);
        OpenSocket(sn,Mode_TCP);
        //Ждём инициализации сокета (статус SOCK_INIT)
        SocketInitWait(sn);
        //Продолжаем слушать сокет
        ListenSocket(sn);
        SocketListenWait(sn);
      }
    }
	}
	else
	{

		//HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
	}


}
//-----------------------------------------------
