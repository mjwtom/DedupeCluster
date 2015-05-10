/*
 * proxy-server.c
 *
 *  Created on: May 4, 2015
 *      Author: mjwtom
 */




#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#define DEFAULT_PORT 8000
#define MAXLINE 4096




#include <stdio.h>
#include <stdlib.h>

#if 1
#include "config.h"
#include "network.h"
#include "protocol.h"
#else
#include "../include/config.h"
#include "../include/network.h"
#endif

int distribute_data()
{
	return 0;
}

int proxy(int client_fd, int dedupe_fd[], int dedupe_number, FILE* dedupe_file)
{
	struct msg_head head;
	char * buf, *recv_buf;
	int read_size, send_size;
	int running = 1;
	buf = malloc(MAX_BUF_SIZE);
	if(NULL == buf)
	{
		printf("cannot malloc");
		return -1;
	}
	while(running)
	{
		if((read_size = recv(client_fd, &head, sizeof(struct msg_head), 0)) != sizeof(struct msg_head))
		{
			printf("proxy-server recv msg head error");
			return -1;
		}
		switch(head.type)
		{
		case PRO_START:
		case PRO_DATA:
			recv_buf = buf;
			while(head.len > 0)
			{
				if((read_size = recv(client_fd, recv_buf, head.len, 0)) < 0)
				{
					printf("proxy-server recv msg head error");
					return -1;
				}
				recv_buf += read_size;
				head.len -= read_size;
			}
			head.type = PRO_OK;
			if((send_size = send(client_fd, &head, sizeof(struct msg_head), 0)) < 0)
			{
				printf("proxy-server recv msg head error");
				return -1;
			}
			break;
		case PRO_END:
			head.type = PRO_OK;
			if((send_size = send(client_fd, &head, sizeof(struct msg_head), 0)) < 0)
			{
				printf("proxy-server recv msg head error");
				return -1;
			}
			printf("client end data transfer");
			running = 0;
			break;
		}
	}
	free(buf);
	return 0;
}

int main(int argc, char * argv[]) {
	short port;
	char * recipe_filename;
	short dedupe_server_ports[MAX_DEDUPE_NODE];
	int dedupe_server_fds[MAX_DEDUPE_NODE];
	int dedupe_server_number;
	int fd, client_fd;
	int i;
	FILE * proxy_file;
	//skip the program name
	argc --;
	argv ++;

	port = PROXY_SERVER_PORT;
	recipe_filename = *argv;

	argc --;
	argv ++;
	i = 0;
	while(argc > 0)
	{
		dedupe_server_ports[i] = atoi(*argv);
		i ++;
		argc --;
		argv ++;
	}
	dedupe_server_number = i;

	for(i = 0 ; i < dedupe_server_number ; i ++)
	{
		dedupe_server_fds[i] = connect_server("127.0.0.1", dedupe_server_ports[i]);
		if(dedupe_server_fds[i] == -1)
		{
			printf("error connect to dedupe server\n");
			return -1;
		}
	}

	fd = get_server_socket(port);
	if(fd == -1)
	{
		printf("Error to get server socket");
		return -1;
	}
	proxy_file = fopen(recipe_filename, "a+");
	if(NULL == proxy_file)
	{
		printf("error creating proxy metadata file");
		return -1;
	}
	client_fd = accept_client(fd);
	proxy(client_fd, dedupe_server_fds, dedupe_server_number, proxy_file);
	close(client_fd);
	close(fd);
	return EXIT_SUCCESS;
}
