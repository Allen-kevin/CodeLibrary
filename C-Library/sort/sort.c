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


/*
 * Recursive implementation of merge sort
 * */

/* merge two sorted array into anothor array */
void merge(int arr[], int left, int mid, int right)
{
	int i, j, k;
	int left_arr_len = mid - left + 1;
	int right_arr_len = right - mid;
	int left_arr[left_arr_len + 1];
	int right_arr[right_arr_len + 1];

	left_arr[left_arr_len] = INT_MAX;
	right_arr[right_arr_len] = INT_MAX;

	/* init left_arr and right_arr */	
	for (i = 0; i < left_arr_len; i++)
		left_arr[i] = arr[left+i];

	for (j = 0; j < right_arr_len; j++)
		right_arr[j] = arr[mid+1+j];

	/* begin merge two sub-arrays */
	j = 0; i = 0;
	for (k = left; k <= right; k++) {
		if (left_arr[i] <= right_arr[j]) {
			arr[k] = left_arr[i++];
		} else {
			arr[k] = right_arr[j++];
		}
	}
}

/* 
 * Decompose the array to be sorted into two sub-arrays, and 
 * call the two sub-arrays recursively respectively.
 * */
void merge_sort(int arr[], int left, int right)
{
	if (left == right)
		return;
	
	int mid = (left+right)>>1;

	merge_sort(arr, left, mid);
	merge_sort(arr, mid+1, right);
	merge(arr, left, mid, right);
}

/*
 * Quic sort algorithm based on recursion
 * */

int quic_sort(int arr[], int left, int right)
{
	if (left >= right)
		return 0;
	
	int i = left, j = right;
	int temp = arr[left];

	while (i != j) {
		while (i < j && arr[j] >= temp)
			j--;
		swap(&arr[i], &arr[j]);
		while (i < j && arr[i] <= temp)
			i++;
		swap(&arr[i], &arr[j]);
	}

	quic_sort(arr, i+1, right);
	quic_sort(arr, left, i-1);
}
