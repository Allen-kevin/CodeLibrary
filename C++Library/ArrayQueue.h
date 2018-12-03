#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

/* ArrayQueue.h */
template <typename T>
class ArrayQueue {
private:
	T *m_pQueue;
	int m_iSize;
	int m_iHead;
	int m_iTail;

public:
	ArrayQueue(int size);
	~ArrayQueue();
	bool QueueEmpty();
	bool QueueFull();
	void ClearQueue();
	int QueueLength();
	T QueueBack();
	T QueueFront();
	void EnQueue(T elem);
	T DeQueue();
	void QueueTranverse();
};

/* constructor function. */
template <typename T>
ArrayQueue<T>::ArrayQueue(int size)
{
	m_iSize = size;
	m_pQueue = new T[m_iSize];
	m_iHead = m_iTail = 0;
}

/* destructor function. */
template <typename T>
ArrayQueue<T>::~ArrayQueue()
{
	delete m_pQueue;
	m_pQueue = NULL;
}

template <typename T>
bool ArrayQueue<T>::QueueEmpty()
{
	return m_iHead == m_iTail ? true : false;
}

template <typename T>
bool ArrayQueue<T>::QueueFull()
{
	return (m_iTail+1+m_iSize)%m_iSize == m_iHead ? true : false;
}

template <typename T>
void ArrayQueue<T>::ClearQueue()
{
	m_iHead = m_iTail;
}

template <typename T>
int ArrayQueue<T>::QueueLength()
{
	if (m_iHead > m_iTail)
		return m_iTail + m_iSize - m_iHead;
	
	return m_iTail - m_iHead;
	
}

template <typename T>
void ArrayQueue<T>::EnQueue(T elem)
{
	if (QueueFull()) {
		cout << "queue full!" << -1 << endl;
		return;
	} else {
		m_pQueue[m_iTail] = elem;
		m_iTail = (m_iTail + 1 + m_iSize)%m_iSize;
	}
}

template <typename T>
T ArrayQueue<T>::DeQueue()
{
	if (QueueEmpty()) {
		cout << "queue empty!" << endl;
		return -1;
	}
	
	m_iHead = (m_iHead+1+m_iSize)%m_iSize;

	return m_pQueue[m_iHead];
}

template <typename T>
T ArrayQueue<T>::QueueBack()
{
	return m_pQueue[(m_iTail+2*m_iSize-1)%m_iSize];
}

template <typename T>
T ArrayQueue<T>::QueueFront()
{
	return m_pQueue[m_iHead];
}

template <typename T>
void ArrayQueue<T>::QueueTranverse()
{	
	if (QueueEmpty()) {
		cout << "queue empty!" << -1 << endl;
		return;
	}
	
	int temp = m_iHead;
	while (temp != m_iTail) {
		cout << m_pQueue[temp] << " ";
		temp = (temp + m_iSize + 1)%m_iSize;
	}
	cout << endl;
}
