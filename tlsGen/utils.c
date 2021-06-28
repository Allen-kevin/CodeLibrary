#include <time.h>
#include "utils.h"

int myRandom(const unsigned int x)
{
	int index = 0;
	int list[] = {100, 100};
	//int list[] = {100, 100, 90, 90, 90, 80, 80, 80, 70, 70, 70, 60, 60, 60, 50, 50, 50, 40, 40, 40, 30, 30, 30};
	index = x % (sizeof(list)/sizeof(int));

	return list[index];
}

/*
 * shuffer the array in random order
 */
void shuffer(unsigned int *array, const int size)
{
	srand((int)time(0));
	if (size > 1) {
		int i, j, tmp;
		for (i=0; i<size; i++) {
			j = i + rand() / (RAND_MAX / (size - i) + 1);
			tmp = array[j];
			array[j] = array[i];
			array[i] = tmp;
		}
	}
}


void showMap(unsigned int *array, const int size)
{
	int i;
	for (i=0; i<size; i++) {
		if (array[i] != 0) {
			printf("%d, %d\t", i, array[i]);
		}
	}
}
