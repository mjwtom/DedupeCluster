/*
 * proxy-server.h
 *
 *  Created on: May 4, 2015
 *      Author: mjwtom
 */

#ifndef PROXY_SERVER_PROXY_SERVER_H_
#define PROXY_SERVER_PROXY_SERVER_H_

#if 1
#include "config.h"
#else
#include "../include/config.h"
#endif

struct recipe
{
	unsigned char fingerprint[FINGERPRINT_LEN];
	uint64_t dedupe_server_id;
	uint64_t offset;
};

struct dedupe_server
{
	char * ip;
	unsigned short port;
	int sock;
};

struct proxy_manager
{
	char * ip;
	unsigned short port;
	int sock;
	char * recipe_filename;
	FILE * recipe_file;
	int dedupe_server_num;
	struct dedupe_server * ds;
};

#endif /* PROXY_SERVER_PROXY_SERVER_H_ */
