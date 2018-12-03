#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

/* LinkQueue.h */
template <typename T>
struct QueueNode {
	T m_data;
	QueueNode* next;

};

template <typename T>
class LinkQueue {
private:
	QueueNode<T>* head;
	QueueNode<T>* tail;
public:
	LinkQueue();
	~LinkQueue();
	bool QueueEmpty();
	void ClearQueue();
	void EnQueue(T elem);
	T DeQueue();
	int QueueLength();
	void QueueTranverse();
};

template <typename T>
LinkQueue<T>::LinkQueue()
{
	head = NULL;
	tail = head;
}

template <typename T>
LinkQueue<T>::~LinkQueue()
{
	QueueNode<T>* node = NULL;
	while (head != NULL) {
		node = head;
		head = head->next;
		delete node;
	}
}

template <typename T>
bool LinkQueue<T>::QueueEmpty()
{
	return head == NULL ? true : false;
}

template <typename T>
void LinkQueue<T>::ClearQueue()
{
	QueueNode<T>* node = NULL;
	while (head != NULL) {
		node = head;
		head = head->next;
		delete node;
	}
}

template <typename T>
void LinkQueue<T>::EnQueue(T data)
{
	QueueNode<T>* node = new QueueNode<T>;
	node->m_data = data;
	
	node->next = NULL;
	if (QueueEmpty()) {
		head = node;
		tail = node;
	} else {
		tail->next = node;
		tail = node;
	}
}

template <typename T>
T LinkQueue<T>::DeQueue()
{
	if (QueueEmpty()) {
		cout << "queue empty!" << -1 << endl;
		return -1;
	}
	
	T value = head->m_data;
	QueueNode<T>* node = head;
	head = head->next;
	delete node;

	return value;
}

template <typename T>
int LinkQueue<T>::QueueLength()
{
	int length = 0;
	QueueNode<T>* node = head;
	while (node != NULL) {
		node = node->next;
		length++;
	}

	return length;
}

template <typename T>
void LinkQueue<T>::QueueTranverse()
{
	if (head == NULL) {
		cout << "queue empty" << endl;
		return;
	}
	
	QueueNode<T>* node = head;
	while (node != NULL) {
		cout << node->m_data << " ";
		node = node->next;
	}
	cout << endl;
}
