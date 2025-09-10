/*
 * network.h
 *
 *  Created on: Aug 9, 2025
 *      Author: Nguyen Trieu
 */

#ifndef INC_NETWORK_H_
#define INC_NETWORK_H_

#include "lwip/api.h"
#include "lwip/netdb.h"
#include "MQTTClient.h"
#include "string.h"
#include "stdint.h"
#include "cmsis_os.h"
#include "FreeRTOS.h"

int  NetworkInit(Network *n);
int NetworkConnect(Network* n);
int NetworkRead(Network *n,  unsigned char * buffer, int len, int timeout_ms);
int NetworkWrite(Network *n,  unsigned char * buffer, int len, int timeout_ms);
void NetworkDisconnect(Network *n);

#endif /* INC_NETWORK_H_ */
