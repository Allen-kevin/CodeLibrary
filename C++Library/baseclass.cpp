#include "algorithm.h"

void baseclass::exchange(int* array, int a, int b)
{
	int temp = array[a];
	array[a] = array[b];
	array[b] = temp;
}
