/*
 ============================================================================
 Name        : practice.c
 Author      : Jingwei Ma
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

bool isHappy(int n) {
	int dig;
	int sum;
	int current, list_len, passed;
	int base = 2;
	if (0 == n)
	{
		return false;
	}
	list_len = base;
	passed = 0;
	while (1!=n)
	{
//		printf("n:%d\n\r", n);
		sum = 0;
		while(n != 0)
		{
			dig = n % 10;
			n = n /10;
			sum += dig*dig;
		}
		n = sum;
		passed ++;
		if(n == current)
		{
			break;
		}
		if(passed == list_len)
		{
			passed = 0;
			list_len *= base;
			current = n;
		}
	}
	if(1 == n)
	{
		return true;
	}
	else
	{
		return false;
	}
}

int main(void) {
	printf("%d\n\t", isHappy(168));
	puts("!!!Hello World!!!"); /* prints !!!Hello World!!! */
	return EXIT_SUCCESS;
}
