#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

/*
 * @description : Recursive implementation of merge sort
 * @author		: wanwenkai
 * @date		: 20210221
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
