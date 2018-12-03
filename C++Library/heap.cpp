#include "algorithm.h"

#define PARENT(i) (i-1)/2 	//node i's parent
#define LEFT(i) 2*i+1	  	//node i's left child
#define RIGHT(i) 2*i+2	//node i's right child

/* initial class heap's private variables */
void heap::set(int length) 
{
	array_length = length;
}

void heap::MaxHeapify(int* array, int i)
{
	int l = LEFT(i);	//get node i's left child
	int r = RIGHT(i);	//get node i's right child
	int largest;		//record index of maximum value

	//find maximum value bewteen array[l], array[r] and array[i]
	if (l <= heap_size && array[l] > array[i])
		largest = l;
	else
		largest = i;

	if (r <= heap_size && array[r] > array[largest])
		largest = r;

	//when largest does not equal i, then exchange array[largest] with array[i]
	if (largest != i) {
		exchange(array, largest, i);
		MaxHeapify(array, largest);
	}
}

void heap::MinHeapify(int* array, int i)
{
	int l = LEFT(i);	//get node i's left child
	int r = RIGHT(i);	//get node i's right child
	int least;			//record index of minimum value

	//find minimum value bewteen array[l], array[r] and array[i]
	if (l <= heap_size && array[l] < array[i])
		least = l;
	else
		least = i;

	if (r <= heap_size && array[r] < array[least])
		least = r;

	//when least does not equal i, then exchange array[least] with array[i]
	if (least != i) {
		exchange(array, least, i);
		MinHeapify(array, least);
	}
}

/* build a max heap */
void heap::BuildMaxHeap(int* array)
{
	heap_size = array_length;

	for (int i = array_length/2; i >= 0; i--)
		MaxHeapify(array, i);
}

/* build a min heap */
void heap::BuildMinHeap(int* array)
{
	heap_size = array_length;

	for (int i = array_length/2; i >= 0; i--)
		MinHeapify(array, i);
}

/* heap sort algorithm */
void heap::HeapSort(int* array)
{
	BuildMaxHeap(array);
	for (int i = array_length; i >= 1; i--) {
		exchange(array, 0, i);
		heap_size = heap_size - 1;
		MaxHeapify(array, 0);
	}
}


/************************** priority queue ****************************/

/* get max value of max heap */
int heap::HeapMaximum(int* array)
{
	return array[0];
}

/* get min value of min heap */
int heap::HeapMinimum(int* array)
{
	return array[0];
}

/* get max value of max heap and delete this element form max heap */
int heap::HeapExtractMax(int* array)
{
	if (heap_size < 0) {
		printf("error underflow!\n");
		exit(0);
	}

	int max = array[0];
	array[0] = array[heap_size];
	heap_size = heap_size - 1;
	MaxHeapify(array, 0);

	return max;
}

/* get min value of min heap and delete this element form min heap */
int heap::HeapExtractMin(int* array)
{
	if (heap_size < 0) {
		printf("error underflow!\n");
		exit(0);
	}

	int min = array[0];
	array[0] = array[heap_size];
	heap_size = heap_size - 1;
	MinHeapify(array, 0);

	return min;
}

/* increase array[i] to key */
void heap::HeapIncreaseKey(int* array, int i,int key)
{
	//key must more than array[i]
	if (key < array[i]) {
		printf("error: new key is smaller than curren key!\n ");
		exit(0);
	}

	array[i] = key;
	while (i > 0 && array[PARENT(i)] < array[i]) {
		exchange(array, i, PARENT(i));
		i = PARENT(i);
	}
}

/* decrease array[i] to key */
void heap::HeapDecreaseKey(int* array, int i,int key)
{
	//key must less than array[i]
	if (key > array[i]) {
		printf("error: new key is smaller than curren key!\n ");
		exit(0);
	}

	array[i] = key;
	while (i > 0 && array[PARENT(i)] > array[i]) {
		exchange(array, i, PARENT(i));
		i = PARENT(i);
	}
}

/* insert a element in max heap */
void heap::MaxHeapInsert(int* array, int key)
{
	heap_size = heap_size + 1;
	array[heap_size] = INT_MIN;
	HeapIncreaseKey(array, heap_size, key);
}

/* insert a element in min heap */
void heap::MinHeapInsert(int* array, int key)
{
	heap_size = heap_size + 1;
	array[heap_size] = INT_MAX;
	HeapDecreaseKey(array, heap_size, key);
}
