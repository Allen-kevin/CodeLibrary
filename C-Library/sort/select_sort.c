#include <stdio.h>
#include <limits.h>
#include "test_sort.h"
#include "../include/tools.h"

/*
 * @decription	: select sort
 * @author		: wanwenkai
 * @date		: 20210222
 * */

/* 
 * select sort algorithm, 
 * time complexity O(n^2)
 * space complexity O(1)
 * */
int select_sort(int arr[], int len)
{	
	int i, j, min;

	for (i = 0; i < len; i++) {
		min = i;
		for (j = i+1; j < len; j++) {
			if (arr[j] < arr[min]) {
				min = j;
			}
		}

		swap(&arr[min], &arr[i]);
	}
}
