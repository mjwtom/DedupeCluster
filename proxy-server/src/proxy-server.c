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
#include <libconfig.h>

#if 1
#include "proxy-server.h"
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

int parse_conf(char * conf_filename, struct proxy_manager * pm)
{
	config_t conf;
	config_setting_t * servers_setting, *server_setting;
	char *s;
	int i;
	if(NULL == conf_filename)
	{
		return -1;
	}
	config_init(&conf);
	if(CONFIG_FALSE == config_read_file(&conf, conf_filename))
	{
		printf("cannot read configure file: %s, file: %s, line: %d, text:%s\n",
				conf_filename,
				config_error_file(&conf),
				config_error_line(&conf),
				config_error_text (&conf));
		return -1;
	}
	//start to parse
	//proxy-server
	if(CONFIG_FALSE == config_lookup_string(&conf, "proxy-network.ip", (const char **)&s))
	{
		printf("cannot get the proxy-server ip address\n");
		return -1;
	}
	pm->ip = strdup(s);
	if(CONFIG_FALSE == config_lookup_int(&conf, "proxy-network.port", (int*)&pm->port))
	{
		printf("cannot get the proxy-server port\n");
		return -1;
	}
	if(CONFIG_FALSE == config_lookup_string(&conf, "recipe-name", (const char **)&s))
	{
		printf("cannot get the recipe file name\n");
		return -1;
	}
	pm->recipe_filename = strdup(s);
	servers_setting = config_lookup(&conf, "dedupe-servers");
	if(NULL == servers_setting)
	{
		printf("can not find the dedupe server configuration");
		return -1;
	}
	pm->dedupe_server_num = config_setting_length(servers_setting);
	pm->ds = (struct dedupe_server*) malloc(pm->dedupe_server_num * sizeof(struct dedupe_server));
	if(NULL == pm->ds)
	{
		printf("cannot malloc for dedup-server\n");
		return -1;
	}
	for(i = 0 ; i < pm->dedupe_server_num ; i ++)
	{
		server_setting = config_setting_get_elem(servers_setting, i);
		if(NULL == server_setting)
		{
			printf("cannot get the %dth setting from dedupe server list\n", i);
			return -1;
		}
		if(CONFIG_FALSE == config_setting_lookup_string(server_setting, "ip", (const char **)&s))
		{
			printf("cannot get the %dth dedupe server ip address\n", i);
			return -1;
		}
		pm->ds[i].ip = strdup(s);
		if(CONFIG_FALSE == config_setting_lookup_int(server_setting, "port", (int*)&pm->ds[i].port))
		{
			printf("cannot get the proxy-server port\n");
			return -1;
		}
	}
	config_destroy(&conf);
	return 0;
}

int init_proxy(struct proxy_manager * pm)
{
	int i;
	for(i = 0 ; i < pm->dedupe_server_num ; i ++)
	{
		pm->ds[i].sock = connect_server(pm->ds[i].ip, pm->ds[i].port);
		if(pm->ds[i].sock == -1)
		{
			printf("error connect to dedupe server\n");
			return -1;
		}
	}
	pm->sock = get_server_socket(pm->port);
	if(pm->sock == -1)
	{
		printf("Error to get server socket");
		return -1;
	}
	pm->recipe_file = fopen(pm->recipe_filename, "a+");
	if(NULL == pm->recipe_file)
	{
		printf("error creating proxy metadata file");
		return -1;
	}
	return 0;
}

int run_proxy(struct proxy_manager * pm)
{
	return 0;
}

int main(int argc, char * argv[]) {
	char *conf_filename;
	struct proxy_manager *pm;
	if(argc < 2)
	{
		printf("please give a configure file");
		return -1;
	}
	//skip the program name
	argc --;
	argv ++;
	conf_filename = *argv;
	pm = (struct proxy_manager *) malloc(sizeof(struct proxy_manager));
	if(NULL == pm)
	{
		printf("cannot malloc proxy_manager\n");
		return -1;
	}
	if(parse_conf(conf_filename, pm) < 0)
	{
		printf("cannot parse the configure file\n");
		return -1;
	}
	if(init_proxy(pm) < 0)
	{
		printf("cannot initialize the proxy-server\n");
		return -1;
	}
	run_proxy(pm);
	return EXIT_SUCCESS;
}
