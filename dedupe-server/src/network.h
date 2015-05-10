/*
 * network.h
 *
 *  Created on: May 4, 2015
 *      Author: mjwtom
 */

#ifndef INCLUDE_NETWORK_H_
#define INCLUDE_NETWORK_H_

#include <sys/socket.h>
#include <netinet/in.h>

int connect_server(char * ip, short port);
int get_server_socket(short port);
int accept_client(int server_fd);

#endif /* INCLUDE_NETWORK_H_ */
