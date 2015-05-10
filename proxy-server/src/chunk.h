/*
 * chunk.h
 *
 *  Created on: May 7, 2015
 *      Author: mjwtom
 */

#ifndef CHUNK_H_
#define CHUNK_H_

#define CHUNK_OK 0
#define CHUNK_END 1

struct chunk_head
{
	char * data;
	int len;
};

#endif /* CHUNK_H_ */
