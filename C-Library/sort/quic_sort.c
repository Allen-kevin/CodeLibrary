#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

/*
 * @description : Quic sort algorithm based on recursion
 * @author		: wanwenkai
 * @date		: 20210222
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
