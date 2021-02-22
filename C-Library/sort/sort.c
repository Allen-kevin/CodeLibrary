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

#define PARENT(i) ((i)/2-1)
#define LEFT(i) ((i)*2+1)
#define RIGHT(i) ((i)*2+2)

/* build max heap */
void max_heapify(int arr[], int i, int heap_size)
{
	int left = LEFT(i);
	int right = RIGHT(i);

	while (left <= heap_size) {
		if (right <= heap_size && arr[left] < arr[right])
			left++;
		if (arr[i] > arr[left])
			return;
		else {
			swap(&arr[i], &arr[left]);
			i = left;
			left = LEFT(i);
		}
	}
}


void heap_sort(int arr[], int len)
{
	int i;
	for (i = PARENT(len); i >= 0; i--) {
		max_heapify(arr, i, len-1);
	}
	for (i = len-1; i > 0; i--) {
		swap(&arr[0], &arr[i]);
		max_heapify(arr, 0, i-1);
	}
}
