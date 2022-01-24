/*
 * net.h
 *
 *  Created on: Jun 8, 2020
 *      Author: ra97
 */
#ifndef INC_NET_H_
#define INC_NET_H_
//--------------------------------------------------
#include "stm32f1xx_hal.h"
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "w5500.h"
//--------------------------------------------------
#define IP_ADDR {192,168,0,219}
#define IP_GATE {192,168,0,1}
#define IP_MASK {255,255,255,0}
#define LOCAL_PORT 35784
/*#define IP_ADDR {192,168,1,200}
#define IP_GATE {192,168,1,1}
#define IP_MASK {255,255,255,0}
#define LOCAL_PORT 80*/
/*
 * #define IP_ADDR {10,0,0,200}
#define IP_GATE {10,0,0,1}
#define IP_MASK {255,0,0,0}
#define LOCAL_PORT 80
 */
//--------------------------------------------------
#define be16toword(a) ((((a)>>8)&0xff)|(((a)<<8)&0xff00))
//--------------------------------------------------
void net_poll(void);
void net_ini(void);
//-----------------------------------------------
#endif /* INC_NET_H_ */
