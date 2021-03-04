#include "test_sort.h"
#include <stdio.h>

int main()
{
	int arr[] = {5, 7, 11, 9, 4, 0, 22, 6, 8, 3, -1};
	int len = sizeof(arr)/sizeof(int), i;

	shell_sort(arr, len);
	for (i = 0; i < len; i++) {
		printf("%d ", arr[i]);
	}
	printf("\n");
	return 0;
}
