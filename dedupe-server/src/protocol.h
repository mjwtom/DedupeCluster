/*
 * protocol.h
 *
 *  Created on: May 4, 2015
 *      Author: mjwtom
 */

#ifndef INCLUDE_PROTOCOL_H_
#define INCLUDE_PROTOCOL_H_

#include "config.h"

struct msg_head
{
	uint8_t type;
	uint32_t len;
};

#define PRO_START 0
#define PRO_END 1
#define PRO_DATA 2
#define PRO_OK 3

#endif /* INCLUDE_PROTOCOL_H_ */
