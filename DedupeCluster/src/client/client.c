/*
 * client.c
 *
 *  Created on: May 4, 2015
 *      Author: mjwtom
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include "../include/network.h"
#include "../include/protocol.h"

unsigned long long used_time = 0;
unsigned long long transfored_data = 0;
unsigned long long throughput = 0;

int sendFile(int fd, char *file_name)
{
	FILE *inf;
	char *buf;
	struct msg_head msg;

	size_t	read_size;

	buf = malloc(MAX_BUF_SIZE);
	if(NULL == buf)
	{
		printf("error: malloc buffer");
		return -1;
	}

	/****************send file name******************/
	bzero(buf, MAX_BUF_SIZE);
	bzero(&msg, sizeof(msg));

	msg.type = 1;
	msg.len = strlen(file_name);

	printf("File_name %s \n", file_name);

	if(send(fd, buf, MAX_BUF_SIZE, 0) < 0)
	{
		perror("Send file name error\n");
		return -1;
	}

	/**********************send the content of the file**********************/
	if((inf = fopen(file_name, "r"))  == NULL )
	{
		fprintf(stderr,"Fail to open the file: %s\n", file_name);
		return -1;
	}


	read_size = 0;
	while((read_size = fread(buf, sizeof(char), MAX_BUF_SIZE, inf)) > 0 )
	{
		if(send(fd, buf, read_size, 0) < 0)
		{
			printf("Send file: \t%s Failed", file_name);
			break;
		}
		transfored_data += read_size;
	}
	fclose(inf);
	free(buf);
	return 0;

}


/********************************send the directory********************************/

int sendDir(int fd, char * dir_name)
{
	DIR		*source = NULL;
	struct	dirent *ent = NULL;
	char name[FILENAME_SIZE];


	source = opendir(dir_name);
	if(source == NULL)
	{
		perror("Fail to opendir\n");
		return 0;
	}
	while((ent = readdir(source)) != NULL)
	{
		if((strcmp(ent->d_name,"..") != 0) && (strcmp(ent->d_name, ".") != 0))
		{
			strcpy(name,"\0");
			strcat(name, dir_name);
			strcat(name,"/");
			strcat(name,ent->d_name);
			if(ent->d_type == 4)
				sendDir(fd, name);
			else if(ent->d_type == 8)
				sendFile(fd, name);
		}
	}
	return 0;
}


/***********************************back up process***********************************/
int backup(int fd, char * in_file, char *out_file, int trans_flag)
{

	struct msg_head msg;

	unsigned char buf[MAX_BUF_SIZE];
	size_t rcv_size;


	/***********************store the backup name*************************/
	bzero(buf, MAX_BUF_SIZE);
	strncpy(buf, out_file, strlen(out_file));
	printf("OUT_FILE length: %d\n", strlen(out_file));
	msg.type = 0;
	msg.len = strlen(out_file);

	if(send(fd, &msg, sizeof(msg), 0) < 0)
	{
		perror("Send file name error\n");
		exit(1);
	}

	while((rcv_size = recv(fd, buf, MAX_BUF_SIZE, 0)) < MAX_BUF_SIZE)
	{
		if(rcv_size < 0 )
		{
			perror("Recy check information  error\n");
			exit(1);
		}
	}
	memcpy(&msg, buf, sizeof(msg));


	if(trans_flag == 0)
	{

		if(sendFile(fd, in_file) == 1)
			printf("Success to send file: %s\n", in_file);
		else
			printf("Failed to send file: %s\n", in_file);
	}
	else if(trans_flag == 1)
	{
		if(sendDir(fd, in_file) == 1)
			printf("Success to send Dir: %s\n", in_file);
		else
			printf("Failed to send Dir: %s\n", in_file);
	}
	return 0;
}

int main_t(int argc, char *argv[])
{
	int trans_flag;
	char *in_file;
	char *out_file;
	char *server_ip;
	short port;
	int fd;
	char *backup_name;

	//skip the program name
	argc --;
	argv ++;

	if(!strcmp("0", *argv))
		trans_flag = 0;
	else if(!strcmp("1", *argv))
		trans_flag = 1;
	else if(!strcmp("2", *argv))
		trans_flag = 2;

	//skipe the backup option
	argc --;
	argv ++;

	//get the ip and port
	server_ip = *argv;
	argc --;
	argv ++;

	port = PROXY_SERVER_PORT;

	if(trans_flag == 1 || trans_flag == 0)
		in_file = *argv;
	else if(trans_flag == 2)
		backup_name = *argv;

	argc --;
	argv ++;

	out_file = *argv;
	/*end of transfer arguments*/

	//connect to the proxy-server
	fd = connect_server(server_ip, port);
	if(fd == -1)
	{
		printf("cannot connect to the server");
		return -1;
	}


	/***********************start to backup**************************/
	if(0 == backup(fd, in_file, out_file, trans_flag))
	{
		printf("Success to trans the backup data directory\n");
		printf("transfored data %f MB\n", (transfored_data / 1024.0 /1024));
		printf("time used %f Seconds\n", (used_time / 1000000.0));
		printf("throughput %f MB/Sec\n", (transfored_data * 1.0 / 1024 /1024) / (used_time / 1000000.0));
	}

	return 0;
}
