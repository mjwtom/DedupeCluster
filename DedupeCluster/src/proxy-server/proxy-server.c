/*
 * proxy-server.c
 *
 *  Created on: May 4, 2015
 *      Author: mjwtom
 */


#include <stdio.h>
#include <stdlib.h>

#include "../include/config.h"
#include "../include/network.h"

int main(int argc, char * argv[]) {
	short port;
	char * recipe_filename;
	short dedupe_server_ports[MAX_DEDUPE_NODE];
	int dedupe_server_fds[MAX_DEDUPE_NODE];
	int dedupe_server_number;
	int fd;
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
	/*end of transfer arguments*/
	return EXIT_SUCCESS;
}
