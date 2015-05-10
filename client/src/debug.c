/*
 * debug.c
 *
 *  Created on: May 4, 2015
 *      Author: mjwtom
 */

#if 1
#include "debug.h"
#else
#include "../include/debug.h"
#endif
#include <stdio.h>

#define DEBUGFILE "/home/mjwtom/debuginformation.txt"

int filedebug(char *str)
{
	FILE *poutfile;
	if((poutfile = fopen(DEBUGFILE,"a+")) == NULL){
		printf("file open failed!\n");
		return -1;
		}
	fprintf(poutfile, "%s", str);
	fclose(poutfile);
	return 0;
}
