#include "algorithm.h"

void sort::set(int l)
{
	length = l;
}

/*********************** insert sort **************************/
void sort::InsertionSort(int *array, int length)
{
	int i, j, key;
	for (j = 1; j < length; j++) {
		key = array[j];
		i = j - 1;
		while (i >= 0 && array[i] > key) {
			array[i+1] = array[i];
			i = i - 1;
		}
		array[i+1] = key;
	}
}


/************************ merge sort **************************/
void sort::Merge(int *array, int begin, int mid, int end)
{
	int LLength = mid - begin + 1;
	int RLength = end - mid;
	int LArray[LLength+1];
	int RArray[RLength+1];

	for (int i = 0; i < LLength; i++)
		LArray[i] = array[begin+i];
	for (int j = 0; j < RLength; j++)
		RArray[j] = array[mid+j+1];
	LArray[LLength] = INT_MAX;
	RArray[RLength] = INT_MAX;

	int i = 0;
	int j = 0;
	for (int k = begin; k <= end; k++) {
		if (LArray[i] <= RArray[j]) {
			array[k] = LArray[i];
			i++;
		} else {
			array[k] = RArray[j];
			j++;
		}
	}
}

void sort::MergeSort(int *array, int begin, int end)
{
	if (begin < end) {
		int mid = (begin+end)/2;
		MergeSort(array, begin, mid);
		MergeSort(array, mid+1, end);
		Merge(array, begin, mid, end);
	}
}


/************************* quick sort ***************************/
int sort::RandomzedPartition(int* array, int p, int r)
{
	int i = p + rand()%(r-p+1);	//p <= i <= r
	swap(array, array[r], array[i]);
	return Partition(array, p, r);
}

int sort::Partition(int* array, int p, int r)
{
	int temp = array[r];
	int i = p - 1;

	for (int j = p; j <= r-1; j++)
		if (array[j] <= temp) {
			i = i + 1;
			swap(array, array[i], array[j]);
		}
	swap(array, array[i+1], array[r]);	//put array[r] to correct position
	return i+1;
}

void sort::QuickSort(int* array, int p, int r)
{
	if (p < r) {
		int q = Partition(array, p, r);
		//all element of it's index less than q which value less than array[q].
		QuickSort(array, p, q-1);
		//all element of it's index more than q which value more than array[q].
		QuickSort(array, q+1, r);
	}
}

void sort::RandomzedQuickSort(int* array, int p, int r)
{
	if (p < r) {
		int q = RandomzedPartition(array, p, r);
		RandomzedQuickSort(array, p, q-1);
		RandomzedQuickSort(array, q+1, r);
	}
}


/************************** counting sort ************************/
void sort::CountingSort(int* array, int *Output, int k)
{
	/*Output[] be used to store  element of sorted
	 *temp[] is a temp array.
	 */
	int temp[k+1];

	for (int i = 0; i <= k; i++) {
		temp[i] = 0;
	}

	for (int j = 0; j <= length-1; j++)
		temp[array[j]] = temp[array[j]] +1;
	//temp[i] now contains the number of elements equal to i.
	for (int i = 1; i <= k; i++)
		temp[i] = temp[i] + temp[i-1];
	//temp[i] now contains the number of elements less than or equal to i.
	for (int j = length-1; j >= 0; j--) {
		Output[temp[array[j]]-1] = array[j];
		temp[array[j]] = temp[array[j]] - 1;
	}	
}

void sort::BucketSort(int* array, int size, int bucketSize)
{
	Node **bucketTable = (Node**)malloc(bucketSize*sizeof(Node*));
	for (int i = 0; i < bucketSize; i++) {//初始化桶
		bucketTable[i] = (Node*)malloc(sizeof(Node));//分配一个节点大小
		bucketTable[i]->data = 0;//0表示桶中还没有元素
		bucketTable[i]->next = NULL;//给next指针初始值赋值NULL
	}

	for (int j = 0; j < size; j++) {
		Node* node = (Node*)malloc(sizeof(Node));
		node->data = array[j];
		node->next = NULL;

		int index = array[j]/10; //假设数据满足0<= array[j] << 99
		Node *p = bucketTable[index];//p指向桶中数据链表的头

		if (p->data == 0) {//桶为空时
			bucketTable[index]->next = node;//将第一个节点挂载头指针后面
			(bucketTable[index]->data)++;//头指针的data域记录桶中元素个数
		} else {//桶不为空时，将相应节点插入到链表中
			while (p->next != NULL && p->next->data < node->data)
				p = p->next;
			//添加新节点
			node->next = p->next;
			p->next = node;
			(bucketTable[index]->data)++;//记录桶中元素个数
		}
	}
}
