/*
 * chunk.c
 *
 *  Created on: May 5, 2015
 *      Author: mjwtom
 */

#include "config.h"
#include "chunk.h"
#include "rabin-hash.h"

int chunk_head_init(struct chunk_head * head, char * buf, int len)
{
	head->len = len;
	head->data = buf;
	return 0;
}

int get_chunk(struct chunk_head * head, char * chunk)
{
	char head_char, tail_char;
	int restrice_len;
	unsigned rabin_fp;
	chunk = head->data;
	if(head->len <= MIN_CHUNK_LEN)
	{
		head->data += head->len;
	}
	else
	{
		rabin_fp = rabin_hash(head->data + MIN_CHUNK_LEN - WIN_LEN, WIN_LEN);
		head->data += MIN_CHUNK_LEN;
		if(head->len > MAX_CHUNK_LEN)
		{
			restrice_len = MAX_CHUNK_LEN;
		}
		else
		{
			restrice_len = head->len;
		}
		while((head->data - chunk) < restrice_len)
		{
			if((rabin_fp % MEAN_CHUNK_LEN) == CHUNK_CDC_R)
			{
				break;
			}
			head_char = *(head->data - WIN_LEN);
			tail_char = *(head->data);
			head->data ++;
			rabin_fp = rabin_karp(head_char, rabin_fp, tail_char);
		}
	}
	head->len -= (head->data - chunk);
	return (head->data - chunk);
}
