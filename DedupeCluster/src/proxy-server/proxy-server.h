/*
 * proxy-server.h
 *
 *  Created on: May 4, 2015
 *      Author: mjwtom
 */

#ifndef PROXY_SERVER_PROXY_SERVER_H_
#define PROXY_SERVER_PROXY_SERVER_H_

#include "../include/config-a.h"

struct recipe
{
	unsigned char fingerprint[FINGERPRINT_LEN];
	uint64_t dedupe_server_id;
	uint64_t offset;
};

#endif /* PROXY_SERVER_PROXY_SERVER_H_ */
