#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

/*
 * @description : Quic sort algorithm based on recursion
 * @author		: wanwenkai
 * @date		: 20210222
 * */


void swap(int *p, int *q)
{
	int temp = *p;
	*p = *q;
	*q = temp;
}


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

int main()
{
	int i = 0;
	int arr[10] = {6, 9,1, 55, -22, 7, 4, 44, 66, 2};
	quic_sort(arr, 0, 9);
	for (i = 0; i < 9; i++)
		printf("%d ", arr[i]);

	return 0;
}
