#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <vector>
#include <stack>

using namespace std;

/* creat a list node class */
class Node {
public:
	int data;
	Node *next;
	Node(int _data) {//Node constructor function
		data = _data;//initial 
		next = NULL;//initial
	}
};

class LinkList {
public:
	LinkList () {
		head = NULL;	
	}
	
	int Length();//get the length of link
	Node* Insert(int , int );//add a new node to link
	void PositivePrint(Node* );//positive print link
	void ReversePrint(Node* );//reverse print link
	void Delete(int );//delete a node from link
	Node* Sort(Node* );//
	//return a position of given value in link
	int Search(int value);
	//merge two link to a new link
	Node* Merge(Node* , Node* );
	Node* Getmid(Node* );
	Node* ReverseList(Node* );

private:
	Node* head;
};


/* base class contains some commonly used function */
/* baseclass.cpp */
class baseclass {
private:
public:
	void exchange(int *, int, int);
};

/* sort.cpp */
class sort: public baseclass {
private:
	int length;		//array length

public:
	void set(int);
	/* insert sort */
	void InsertionSort(int *, int);

	/* merge sort */
	void Merge(int *, int, int , int);
	void MergeSort(int *, int, int);

	/* quick sort */
	void QuickSort(int *, int ,int);
	int Partition(int *, int, int);
	int RandomzedPartition(int *, int, int);
	void RandomzedQuickSort(int *, int, int);

	/* counting sort */
	void CountingSort(int *, int *,int);
	void BucketSort(int*, int, int);
};

/* subarray.cpp */
class maxsubarray: public baseclass {
private:
public:
	int* FindMaxCrossingSubArray(int *, int, int, int);
	int* FindMaximumSubArray(int *, int, int);
	int MaximumSubArray_Linear(int *, int, int);

};

/* heap.cpp */
class heap: public baseclass {
private:
	int array_length;
	int heap_size;
public:
	void MaxHeapify(int *, int);
	void MinHeapify(int *, int);
	void BuildMaxHeap(int *);
	void BuildMinHeap(int *);
	void HeapSort(int *);
	void set(int);

	/* max priority queue */
	void MaxHeapInsert(int *, int);
	void HeapIncreaseKey(int *, int, int);
	int HeapMaximum(int *);
	int HeapExtractMax(int *);

	/* min priority queue */
	void MinHeapInsert(int *, int);
	void HeapDecreaseKey(int *, int, int);
	int HeapMinimum(int *);
	int HeapExtractMin(int *);
};
