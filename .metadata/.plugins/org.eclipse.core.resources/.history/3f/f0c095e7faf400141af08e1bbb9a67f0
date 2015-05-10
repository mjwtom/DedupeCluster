/*
 * bloom-filter.h
 *
 *  Created on: 2012-5-31
 *      Author: badboy
 */

#ifndef BLOOM_FILTER_H_
#define BLOOM_FILTER_H_

#include "config.h"

unsigned int hash_func0(unsigned int *finger_prints)
{
	unsigned int position = 0;
	position =  *(finger_prints + 4) & 0x3ffffff;
	return position;
}

unsigned int hash_func1(unsigned int *finger_prints)
{
	unsigned int position = 0;
	position =  ((*(finger_prints + 4)) >> 26) | (((*(finger_prints + 3)) & 0xfffff) << 6);
	return position;
}

unsigned int hash_func2(unsigned int  *finger_prints)
{
	unsigned int position = 0;
	position = ((*(finger_prints + 3)) >> 20) | ((*(finger_prints + 2) & 0x7fff) << 12);
	return position;
}

unsigned int hash_func3(unsigned int  *finger_prints)
{
	unsigned int position = 0;
	position = ((*(finger_prints + 2)) >> 15) | ((*(finger_prints + 1) & 0x3ff) << 17);
	return position;
}

unsigned int hash_func4(unsigned int  *finger_prints)
{
	unsigned int position = 0;
	position = ((*(finger_prints + 1)) >> 10) | ((*(finger_prints) & 0x1f) << 22);
	return position;
}

unsigned int hash_func5(unsigned int *finger_prints)
{
	unsigned int position = 0;
	position = (*(finger_prints)) >> 5;
	return position;
}


int set_bit(int nr,int * addr)
{
        int     mask, retval;

        addr += nr >> 5;
        mask = 1 << (nr & 0x1f);
        retval = (mask & *addr) != 0;
        *addr |= mask;
        return retval;
}
 int test_bit(int nr, int * addr)
{
        int     mask;

        addr += nr >> 5;
        mask = 1 << (nr & 0x1f);
        return ((mask & *addr) != 0);
}
/***********************************************************************
* function name = bloomfilter_lookup
* function = lookup the abstract in the bf and update the bf line
* input :
*         @bfset               the bloom filter bit set
*	    @element          the data abstract from the sha1
*
* return:
*          ret = 1                  find
*          ret = 0 		     not find
***********************************************************************/
int bloom_filter_lookup(void * bf, unsigned int * element)
{
	int ret = 1;
	if(test_bit(hash_func0(element),bf) &&
	  	test_bit(hash_func1(element),bf+ (1 << 23))&&
	  	test_bit(hash_func2(element),bf+ (1 << 24)) &&
	  	test_bit(hash_func3(element),bf+ (1 << 25)) &&
	  	test_bit(hash_func4(element),bf+ (1 << 25) + (1 << 24)) &&
	  	test_bit(hash_func5(element),bf+ (1 << 26)) ) {
		return ret;
   	}

	ret = 0;
	set_bit(hash_func0(element),bf);
  	set_bit(hash_func1(element),bf + (1 << 23));
  	set_bit(hash_func2(element),bf + (1 << 24));
  	set_bit(hash_func3(element),bf + (1 << 25));
  	set_bit(hash_func4(element),bf + (1 << 25) + (1 << 24));
  	set_bit(hash_func5(element),bf + (1 << 26));

	return ret;
}

int bloomfilter_init(char * bf)
{
	memset(bf, 0, BF_LEN);
	return 0;
}


#endif /* BLOOM_FILTER_H_ */
