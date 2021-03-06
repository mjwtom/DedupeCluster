/*
 * metadata.h
 *
 *  Created on: 2012-7-12
 *      Author: BadBoy
 */

#ifndef METADATA_H_
#define METADATA_H_

#include <stdint.h>
#include "config.h"
#include "storage-manager.h"

struct mtdata_seg_header
{
	uint64_t previous;
	uint64_t next;
};

struct metadata
{
	char fingerprint[FINGERPRINT_LEN];
	uint32_t len;
#ifdef COMPRESS
	uint32_t origin_len;
#endif
	uint64_t offset;
};

#define MTDATA_PER_SEGMENT ((SEG_SIZE - sizeof(struct mtdata_seg_header)) / sizeof(struct metadata))

struct mtdata_seg
{
	struct mtdata_seg_header header;
	//uint64_t mtdata_offset;
	uint64_t mt_seg_offset;
	uint32_t len;
	struct storage_manager * manager;
	struct metadata mtdata[MTDATA_PER_SEGMENT];
};

int metadata_init(struct mtdata_seg * mt_seg)
{
	mt_seg->mt_seg_offset = get_new_seg(mt_seg->manager);
	mt_seg->len = 0;
	return 0;
}

uint64_t add_metadata(struct metadata mtdata, struct mtdata_seg * mt_seg)
{
	uint64_t offset;
	uint64_t write_offset;
	if(mt_seg->len >= MTDATA_PER_SEGMENT)
	{
		write_offset = mt_seg->mt_seg_offset;
		mt_seg->mt_seg_offset = get_new_seg(mt_seg->manager);
		mt_seg->header.next = mt_seg->mt_seg_offset;
		simplewrite(write_offset, &mt_seg->header, sizeof(struct mtdata_seg_header), mt_seg->manager);
		write_offset += sizeof(struct mtdata_seg_header);
		simplewrite(write_offset, mt_seg->mtdata, MTDATA_PER_SEGMENT * sizeof(struct metadata), mt_seg->manager);
		mt_seg->len = 0;
	}
	offset = mt_seg->mt_seg_offset + sizeof(struct mtdata_seg_header) + mt_seg->len * sizeof(struct metadata);
	mt_seg->mtdata[mt_seg->len] = mtdata;
	mt_seg->len ++;
	return offset;
}

int get_metadata(struct mtdata_seg * mt_seg, struct metadata * mtdata, int len, uint64_t offset)
{
	uint32_t diff;
	struct mtdata_seg_header header;
	uint64_t seg_offset;
	uint32_t left;
	uint32_t init_len;
	init_len = len;
	while((len > 0))
	{
		if(offset > mt_seg->mt_seg_offset)
		{
			diff = (offset - mt_seg->mt_seg_offset - sizeof(struct mtdata_seg_header)) / sizeof(struct metadata);
			if(len + diff <= mt_seg->len)
			{
				memcpy(mtdata, &(mt_seg->mtdata[diff]), len * sizeof(struct metadata));
				len -= len;
				mtdata += len;
			}
			else
			{
				memcpy(mtdata, &(mt_seg->mtdata[diff]), (mt_seg->len - diff) * sizeof(struct metadata));
				len -= (mt_seg->len - diff);
				mtdata += (mt_seg->len - diff);
				return init_len - len;
			}
		}
		else
		{
			seg_offset = offset / SEG_SIZE * SEG_SIZE;
			left = (seg_offset + SEG_SIZE - offset) / sizeof(struct metadata);
			if(left >= len)
			{
				simpleread(offset, mtdata, len * sizeof(struct metadata), mt_seg->manager);
				len -= len;
				mtdata += len;
			}
			else
			{
				simpleread(offset, mtdata, left * sizeof(struct metadata), mt_seg->manager);
				len -= left;
				mtdata += left;

				simpleread(seg_offset, &header, sizeof(struct mtdata_seg_header), mt_seg->manager);
				offset = header.next + sizeof(struct mtdata_seg_header);
			}
		}
	}
	return init_len - len;
}

#endif /* METADATA_H_ */
