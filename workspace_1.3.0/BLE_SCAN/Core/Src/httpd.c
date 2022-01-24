#include "httpd.h"
//-----------------------------------------------

//-----------------------------------------------
extern char str1[];
extern char tmpbuf[];
extern uint8_t sect[];
//-----------------------------------------------
http_sock_prop_ptr httpsockprop[8];


uint32_t bytesread;
volatile uint16_t tcp_size_wnd = 2048;
//-----------------------------------------------
const char http_header[] = { "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n"};
const char jpg_header[] = {"HTTP/1.0 200 OK\r\nServer: nginx\r\nContent-Type: image/jpeg\r\nConnection: close\r\n\r\n"};
const char icon_header[] = { "HTTP/1.1 200 OK\r\nContent-Type: image/x-icon\r\n\r\n"};
const char error_header[] = {"HTTP/1.0 404 File not found\r\nServer: nginx\r\nContent-Type: text/html\r\nConnection: close\r\n\r\n"};
const char json_header[] = {"HTTP/1.1 200 OK\r\nContent-Type: application/json;\r\n\r\n"};
char *header_send;
char *data_send;
//-----------------------------------------------
const uint8_t e404_htm[] = {
0x3c,0x68,0x74,0x6d,0x6c,0x3e,0x0a,0x20,0x20,0x3c,0x68,0x65,0x61,0x64,0x3e,0x0a,
0x20,0x20,0x20,0x20,0x3c,0x74,0x69,0x74,0x6c,0x65,0x3e,0x34,0x30,0x34,0x20,0x4e,
0x6f,0x74,0x20,0x46,0x6f,0x75,0x6e,0x64,0x3c,0x2f,0x74,0x69,0x74,0x6c,0x65,0x3e,
0x0a,0x20,0x20,0x3c,0x2f,0x68,0x65,0x61,0x64,0x3e,0x0a,0x3c,0x62,0x6f,0x64,0x79,
0x3e,0x0a,0x3c,0x68,0x31,0x20,0x73,0x74,0x79,0x6c,0x65,0x3d,0x22,0x74,0x65,0x78,
0x74,0x2d,0x61,0x6c,0x69,0x67,0x6e,0x3a,0x20,0x63,0x65,0x6e,0x74,0x65,0x72,0x3b,
0x22,0x3e,0x34,0x30,0x34,0x20,0x45,0x72,0x72,0x6f,0x72,0x20,0x46,0x69,0x6c,0x65,
0x20,0x4e,0x6f,0x74,0x20,0x46,0x6f,0x75,0x6e,0x64,0x3c,0x2f,0x68,0x31,0x3e,0x0a,
0x3c,0x68,0x32,0x20,0x73,0x74,0x79,0x6c,0x65,0x3d,0x22,0x74,0x65,0x78,0x74,0x2d,
0x61,0x6c,0x69,0x67,0x6e,0x3a,0x20,0x63,0x65,0x6e,0x74,0x65,0x72,0x3b,0x22,0x3e,
0x20,0x54,0x68,0x65,0x20,0x70,0x61,0x67,0x65,0x20,0x79,0x6f,0x75,0x20,0x61,0x72,
0x65,0x20,0x6c,0x6f,0x6f,0x6b,0x69,0x6e,0x67,0x20,0x66,0x6f,0x72,0x20,0x6d,0x69,
0x67,0x68,0x74,0x20,0x68,0x61,0x76,0x65,0x20,0x62,0x65,0x65,0x6e,0x20,0x72,0x65,
0x6d,0x6f,0x76,0x65,0x64,0x2c,0x20,0x3c,0x62,0x72,0x20,0x2f,0x3e,0x68,0x61,0x64,
0x20,0x69,0x74,0x73,0x20,0x6e,0x61,0x6d,0x65,0x20,0x63,0x68,0x61,0x6e,0x67,0x65,
0x64,0x2c,0x20,0x6f,0x72,0x20,0x69,0x73,0x20,0x74,0x65,0x6d,0x70,0x6f,0x72,0x61,
0x72,0x69,0x6c,0x79,0x20,0x75,0x6e,0x61,0x76,0x61,0x69,0x6c,0x61,0x62,0x6c,0x65,
0x2e,0x3c,0x2f,0x68,0x32,0x3e,0x0a,0x3c,0x2f,0x62,0x6f,0x64,0x79,0x3e,0x3c,0x2f,
0x68,0x74,0x6d,0x6c,0x3e};

//uint8_t DATA_htm[]={"WORDHELLO WORDHELLO WORDHELLO WORDHELLOWORDHELLO WORDHELLO WORDHELLO WORDHEWORDHELLO WORDHELLO WORDHELLO WORDHEWORDHELLO WORDHELLO WORDHELLO WORDHEWORDHELLO WORDHELLO WORDHELLO WORDHEWORDHELLO WORDHELLO WORDHELLO WORDHEWORDHELLO WORDHELLO WORDHELLO WORDHEWORDHELLO WORDHELLO WORDHELLO WORDHEWORDHELLO WORDHELLO WORDHELLO WORDHEWORDHELLO WORDHELLO WORDHELLO WORDHEWORDHELLO WORDHELLO WORDHELLO WORDHEWORDHELLO WORDHELLO WORDHELLO WORDHEWORDHELLO WORDHELLO WORDHELLO WORDHE WORDHELLO WORDHELLO WORDHELLO WORDHELLO WORDHELLO WORDHELLO WORDHELLO WORDHELLO WORDHELLO WORDHELLO WORDHELLO WORDHELLO WORDHELLO WORDHELLO WORDHELLO WORDHELLO WORDHELLO WORDHELLO WORDHELLO WORDHELLO WORDHELLO WORDHELLO WORDHELLO WORDHELLO WORDHELLO WORDHELLO WORDHELLO WORDHELLO WORDHELLO WORDHELLO WORDHELLO WORDHELLO WORDHELLO WORDHELLO WORDHELLO WORDHELLO WORDHELLO WORDHELLO WORDHELLO WORDHELLO WORDHELLO WORDHELLO WORDHELLO WORDHELLO WORDHELLO WORDHELLO WORDHELLO WORDHELLO WORDHELLO WORDHELLO WORDHELLO WORDHELL"};
//uint8_t DATA_htm2[]={"qwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyuioqwertyui"};
extern uint8_t JSON_DATA_SEND[];
//uint8_t DATA_htm[]={"WORDHELLO WORDHELLO WORDHELLO WORDHELLO WORDHELLO"};
//-----------------------------------------------

void tcp_send_http_one(uint8_t sn)
{

  uint16_t i=0;
  uint16_t data_len=0;
  uint16_t header_len=0;
  uint16_t end_point;
  uint8_t num_sect=0;
  uint16_t len_sect=0;

				header_send = (void*)http_header;

		header_len = strlen(header_send);
		data_len = strlen(data_send);
		end_point = GetWritePointer(sn);
		end_point+=header_len+data_len;
		//Заполним данными буфер для отправки пакета
		SetWritePointer(sn, end_point);
		end_point = GetWritePointer(sn);
		memcpy(sect+3,header_send,header_len);
		w5500_writeSockBuf(sn, end_point, (uint8_t*)sect, header_len);
		end_point+=header_len;
				num_sect = data_len / 512;
				for(i=0;i<=num_sect;i++)
				{
					//sprintf(str1,"i=%d\tnum_sect=%d\r\n",i,num_sect);
					//												HAL_UART_Transmit(&huart2,(uint8_t*)str1,strlen(str1),0x1000);
					//не последний сектор
					if(i<num_sect) len_sect=512;
					else len_sect=data_len;

					//sprintf(str1,"data_len=%dlen_sect=%d\t\r\n",data_len,len_sect);
					//												HAL_UART_Transmit(&huart2,(uint8_t*)str1,strlen(str1),0x1000);
					memcpy(sect+3,data_send+i*512,len_sect);
					w5500_writeSockBuf(sn, end_point, (uint8_t*)sect, len_sect);
					end_point+=len_sect;
					data_len -= len_sect;



				}
/*
			memcpy(sect+3,DATA_htm,512);
			w5500_writeSockBuf(sn, end_point, (uint8_t*)sect, 512);
			end_point+=512;
			memcpy(sect+3,DATA_htm2,512);
			w5500_writeSockBuf(sn, end_point, (uint8_t*)sect, 512);
			end_point+=512;
			*/


	//отправим данные
  RecvSocket(sn);
  SendSocket(sn);
  httpsockprop[sn].data_stat = DATA_COMPLETED;
  //sprintf(str1,"tcp_send_http_one()end\r\n");
  //			HAL_UART_Transmit(&huart2,(uint8_t*)str1,strlen(str1),0x1000);
}
//-----------------------------------------------
void tcp_send_http_first(uint8_t sn)
{
  uint8_t prt;
  uint16_t i=0;
  uint16_t data_len=0;
  uint16_t header_len=0;
  uint16_t end_point;
  uint8_t num_sect=0;
  uint16_t len_sect;
  uint16_t last_part, last_part_size;
	//На EXISTING проверять не будем, так как будем считать, что error404_htm у нас всегда будет умещаться в один буфер
  header_send = (void*)http_header;
	header_len = strlen(header_send);
	data_len = tcp_size_wnd-header_len;
	end_point = GetWritePointer(sn);
	end_point+=header_len+data_len;
	SetWritePointer(sn, end_point);
	end_point = GetWritePointer(sn);
	//сохраним некоторые параметры, а то почему-то они теряются
	last_part = httpsockprop[sn].cnt_rem_data_part;
	last_part_size = httpsockprop[sn].last_data_part_size;
	prt = httpsockprop[sn].prt_tp;
	//Заполним данными буфер для отправки пакета
	memcpy(sect+3,header_send,header_len);
	w5500_writeSockBuf(sn, end_point, (uint8_t*)sect, header_len);
	end_point+=header_len;
	num_sect = data_len / 512;
	for(i=0;i<=num_sect;i++)
		{
			//sprintf(str1,"i=%d\tnum_sect=%d\r\n",i,num_sect);
			//												HAL_UART_Transmit(&huart2,(uint8_t*)str1,strlen(str1),0x1000);
			//не последний сектор
			if(i<num_sect) len_sect=512;
			else len_sect=data_len;

			//sprintf(str1,"data_len=%dlen_sect=%d\t\r\n",data_len,len_sect);
			//												HAL_UART_Transmit(&huart2,(uint8_t*)str1,strlen(str1),0x1000);
			memcpy(sect+3,data_send+i*512,len_sect);
			w5500_writeSockBuf(sn, end_point, (uint8_t*)sect, len_sect);
			end_point+=len_sect;
			data_len -= len_sect;



		}
	//вернем параметры
	httpsockprop[sn].cnt_rem_data_part = last_part;
	httpsockprop[sn].last_data_part_size = last_part_size;
	httpsockprop[sn].prt_tp = prt;
	//отправим данные
	RecvSocket(sn);
	SendSocket(sn);
	//будем считать, что одну часть отправили, поэтому количество оставшихся частей декрементируем
	httpsockprop[sn].cnt_rem_data_part--;
	if(httpsockprop[sn].cnt_rem_data_part>1)
	{
		httpsockprop[sn].data_stat=DATA_MIDDLE;
	}
	else
	{
		httpsockprop[sn].data_stat=DATA_LAST;
	}
	//Количество переданных байтов
  httpsockprop[sn].total_count_bytes = tcp_size_wnd - header_len;
}
//-----------------------------------------------
void tcp_send_http_middle(uint8_t sn)
{
  uint16_t i=0;
  uint16_t data_len=0;
  uint16_t end_point;
  uint8_t num_sect=0;
  uint8_t prt;
  uint16_t len_sect;
  uint16_t last_part, last_part_size;
  uint32_t count_bytes;
	data_len = tcp_size_wnd;
	end_point = GetWritePointer(sn);
	end_point+=data_len;
	SetWritePointer(sn, end_point);
	end_point = GetWritePointer(sn);
	//сохраним некоторые параметры, а то почему-то они теряются
	last_part = httpsockprop[sn].cnt_rem_data_part;
	last_part_size = httpsockprop[sn].last_data_part_size;
	count_bytes = httpsockprop[sn].total_count_bytes;
	prt = httpsockprop[sn].prt_tp;
	//Заполним данными буфер для отправки пакета
	num_sect = data_len / 512;
	//борьба с неправильным расчётом, когда размер данных делится на размер сектора без остатка
	if(data_len%512==0) num_sect--;
	for(i=0;i<=num_sect;i++)
			{
				//sprintf(str1,"i=%d\tnum_sect=%d\r\n",i,num_sect);
				//												HAL_UART_Transmit(&huart2,(uint8_t*)str1,strlen(str1),0x1000);
				//не последний сектор
				if(i<num_sect) len_sect=512;
				else len_sect=data_len;

				//sprintf(str1,"data_len=%dlen_sect=%d\t\r\n",data_len,len_sect);
				//												HAL_UART_Transmit(&huart2,(uint8_t*)str1,strlen(str1),0x1000);
				memcpy(sect+3,data_send+i*512+count_bytes,len_sect);
				w5500_writeSockBuf(sn, end_point, (uint8_t*)sect, len_sect);
				end_point+=len_sect;
				data_len -= len_sect;



			}
	//вернем параметры
	httpsockprop[sn].cnt_rem_data_part = last_part;
	httpsockprop[sn].last_data_part_size = last_part_size;
	httpsockprop[sn].total_count_bytes = count_bytes;
	httpsockprop[sn].prt_tp = prt;
	RecvSocket(sn);
	SendSocket(sn);
	//будем считать, что одну часть отправили, поэтому количество оставшихся частей декрементируем
	httpsockprop[sn].cnt_rem_data_part--;
	if(httpsockprop[sn].cnt_rem_data_part>1)
	{
		httpsockprop[sn].data_stat=DATA_MIDDLE;
	}
	else
	{
		httpsockprop[sn].data_stat=DATA_LAST;
	}
	//Количество переданных байтов
	httpsockprop[sn].total_count_bytes += (uint32_t) tcp_size_wnd;
}
//-----------------------------------------------
void tcp_send_http_last(uint8_t sn)
{
  uint16_t i=0;
  uint16_t data_len=0;
  uint16_t end_point;
  uint8_t num_sect=0;
  uint16_t len_sect;
	data_len = httpsockprop[sn].last_data_part_size;
	end_point = GetWritePointer(sn);
	end_point+=data_len;
	SetWritePointer(sn, end_point);
	end_point = GetWritePointer(sn);
	//Заполним данными буфер для отправки пакета
	num_sect = data_len / 512;
	//борьба с неправильным расчётом, когда размер данных делится на размер сектора без остатка
	if(data_len%512==0) num_sect--;
	for(i=0;i<=num_sect;i++)
				{
					//sprintf(str1,"i=%d\tnum_sect=%d\r\n",i,num_sect);
					//												HAL_UART_Transmit(&huart2,(uint8_t*)str1,strlen(str1),0x1000);
					//не последний сектор
					if(i<num_sect) len_sect=512;
					else len_sect=data_len;

					//sprintf(str1,"data_len=%dlen_sect=%d\t\r\n",data_len,len_sect);
					//												HAL_UART_Transmit(&huart2,(uint8_t*)str1,strlen(str1),0x1000);
					memcpy(sect+3,data_send+i*512+httpsockprop[sn].total_count_bytes,len_sect);
					w5500_writeSockBuf(sn, end_point, (uint8_t*)sect, len_sect);
					end_point+=len_sect;
					data_len -= len_sect;



				}
	//отправим данные
	RecvSocket(sn);
	SendSocket(sn);
	httpsockprop[sn].data_stat = DATA_COMPLETED;
}
//-----------------------------------------------
void http_request(uint8_t sn)
{
	reboot_min=0;
  uint16_t point;
  uint8_t RXbyte;
  uint16_t i=0;
  char *ss1;
  int ch1='.';

  // ищем первый "/" в HTTP заголовке
  point = GetReadPointer(sn);
  i = 0;
  while (RXbyte != (uint8_t)'/')
  {
    RXbyte = w5500_readSockBufByte(sn,point+i);
    i++;
  }
	point+=i;
	RXbyte = w5500_readSockBufByte(sn,point);
	if(RXbyte==(uint8_t)' ')
	{
		strcpy(httpsockprop[sn].fname,"index.htm");
		httpsockprop[sn].http_doc = EXISTING_HTML;
	}
	else
	{
		// ищем следующий пробел (" ") в HTTP заголовке, таким образом считывая имя документа из запроса
		i=0;
		while (1)
		{
			tmpbuf[i] = w5500_readSockBufByte(sn, point+i);
			if(tmpbuf[i] == (uint8_t)' ') break;
			i++;
		}
		tmpbuf[i] = 0; //закончим строку
		strcpy(httpsockprop[sn].fname,tmpbuf);
	}
	//HAL_UART_Transmit(&huart2,(uint8_t*)httpsockprop[sn].fname,strlen(httpsockprop[sn].fname),0x1000);
	//HAL_UART_Transmit(&huart2,(uint8_t*)"\r\n",2,0x1000);


	//HAL_UART_Transmit(&huart2,(uint8_t*)str1,strlen(str1),0x1000);

	//HAL_UART_Transmit(&huart2,(uint8_t*)str1,strlen(str1),0x1000);
	if (1)
	{
		//изучим расширение файла
		ss1 = strchr(httpsockprop[sn].fname,ch1);
		ss1++;
		if (strncmp(ss1,"jpg", 3) == 0)
		{
			httpsockprop[sn].http_doc = EXISTING_JPG;
			//сначала включаем в размер размер заголовка
			httpsockprop[sn].data_size = strlen(jpg_header);
		}
		if (strncmp(ss1,"ico", 3) == 0)
		{
			httpsockprop[sn].http_doc = EXISTING_ICO;
			//сначала включаем в размер размер заголовка
			httpsockprop[sn].data_size = strlen(icon_header);
		}
		else
		{
			httpsockprop[sn].http_doc = EXISTING_HTML;
			//сначала включаем в размер размер заголовка
			httpsockprop[sn].data_size = strlen(http_header);
		}
		//затем размер самого документа
		data_send=(void*)JSON_DATA_SEND;
		httpsockprop[sn].data_size += strlen(data_send);
	}
	else
	{
		httpsockprop[sn].http_doc = E404_HTML;
		//сначала включаем в размер размер заголовка
		httpsockprop[sn].data_size = strlen(error_header);
		//затем размер самого документа
		httpsockprop[sn].data_size += sizeof(e404_htm);
	}
	httpsockprop[sn].cnt_rem_data_part = httpsockprop[sn].data_size / tcp_size_wnd + 1;
	httpsockprop[sn].last_data_part_size = httpsockprop[sn].data_size % tcp_size_wnd;
	//борьба с неправильным расчётом, когда общий размер делится на минимальный размер окна без остатка
	if(httpsockprop[sn].last_data_part_size==0)
	{
		httpsockprop[sn].last_data_part_size=tcp_size_wnd;
		httpsockprop[sn].cnt_rem_data_part--;
	}
	httpsockprop[sn].cnt_data_part = httpsockprop[sn].cnt_rem_data_part;
	//sprintf(str1,"data size:%lu; cnt data part:%u; last_data_part_size:%u\r\n",
	//(unsigned long)httpsockprop[sn].data_size, httpsockprop[sn].cnt_rem_data_part, httpsockprop[sn].last_data_part_size);
	//HAL_UART_Transmit(&huart2,(uint8_t*)str1,strlen(str1),0x1000);
	if (httpsockprop[sn].cnt_rem_data_part==1)
	{
		httpsockprop[sn].data_stat = DATA_ONE;
	}
	else if (httpsockprop[sn].cnt_rem_data_part>1)
	{
		httpsockprop[sn].data_stat = DATA_FIRST;
	}
	if(httpsockprop[sn].data_stat==DATA_ONE)
	{
		 tcp_send_http_one(sn);
		DisconnectSocket(sn); //Разъединяемся
		SocketClosedWait(sn);
		OpenSocket(sn,Mode_TCP);
		//Ждём инициализации сокета (статус SOCK_INIT)
		SocketInitWait(sn);
		//Продолжаем слушать сокет
		ListenSocket(sn);
		SocketListenWait(sn);

	}
	else if(httpsockprop[sn].data_stat==DATA_FIRST)
	{
		tcp_send_http_first(sn);
	}
}
//-----------------------------------------------
