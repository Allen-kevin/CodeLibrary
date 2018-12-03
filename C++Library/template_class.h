/* template class contains stack, queue */

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>

using namespace std;

/* stack.cpp */
template <typename T>
class stack {
private:
	T *m_pStack;	//stack index.
	int m_iSize;	//stack capacity.
	int m_iTop;		//stack top.
public:
	stack(int size);
	~stack();
	bool IsEmpty(); //stack is empty.
	bool IsFull();		//stack is full.
	void ClearStack();		//clear stack.
	int StackLength();		//stack's length.
	bool Push(T elem);  //add a elem to stack top.
	bool Pop(T &elem);  //delete elem form stack top
	bool StackTop(T &elem);	//return stack top.
	void StackTranverse();	//tranverse stack.
};

/* constructor */

template <typename T>
stack<T>::stack(int size)
{
	m_iSize = size;
	m_pStack = new T[m_iSize];
	m_iTop = 0;
}

/* destructor */

template <typename T>
stack<T>::~stack()
{
	delete m_pStack;
	m_pStack = NULL;
}

template <typename T>
bool stack<T>::IsEmpty()
{
	return m_iTop == 0 ? true:false;
}

template <typename T>
bool stack<T>::IsFull()
{
	return m_iTop == m_iSize ? true:false;
}

template <typename T>
int stack<T>::StackLength()
{
	return m_iTop;
}

/* clear stack */
template <typename T>
void stack<T>::ClearStack()
{
	m_iTop = 0;
}

/* add elem to stack */
template <typename T>
bool stack<T>::Push(T elem)
{
	if (IsFull()) {
		return false;
	}
	else {
		m_pStack[m_iTop++] = elem;
		return true;
	}
}

/* delete and return elem from stack */
template <typename T>
bool stack<T>::Pop(T &elem)
{
	if (IsEmpty())
	{
		return false;
	}
	else {
		elem = m_pStack[--m_iTop];
		return true;
	}
}

/* return elem from stack top */
template <typename T>
bool stack<T>::StackTop(T &elem)
{
	if (IsEmpty())
	{
		return false;
	}
	else {
		elem = m_pStack[m_iTop-1];
		return true;
	}
}

/* transverse stack */
template <typename T>
void stack<T>::StackTranverse()
{
	int i = 0;
	for (i = 0; i < m_iTop; i++)
		cout << m_pStack[i] << " ";
	cout << endl;
}

/************************** queue ***************************/
template <typename T>
class queue {
private:
	T = *m_pQueue;
	int m_iTail;
	int m_iHead;
	int m_iSize;
public:
	queue();
	~queue();
	bool IsEmpty();
	bool IsFull();
	int QueueLength();
	bool EnQueue();
	int Dequeue();
	void QueueTranverse();

};

template <typename T>
queue<T>::queue(int size)
{
	m_iSize = size;
	m_pQueue = new T[m_iSize];
	m_iHead = 0;
	m_iTail = 0;
}

template <typename T>
~queue<T>::queue()
{
	delete m_pQueue;
	m_pQueue = NULL;
}

template <typename T>
bool queue<T>::IsEmpty()
{
	return m_pTail == m_pHead ? true : false;
}

template <typename T>
bool queue<T>::IsFull()
{
	if ((m_iTail+1)%m_iSize == m_iHead)
		return false;
	else
		return true;
}

template <typename T>
int queue<T>::QueueLength()
{
	return (m_pTail - m_pHead + m_iSize)%m_iSize;
}

template <typename T>
bool queue<t>::EnQueue(T elem)
{
	if (QueueFull()) {
		cout << "queue full!" <<endl;
		return false;
	}
	m_pQueue[m_iTail] = elem;
	m_iTail = (m_iTail + 1)%m_iSize;

	return true;
}

template <typename T>
int queue<t>::DeQueue()
{
	int x = m_pQueue[m_iHead];
	if (IsEmpty()) {
		cout << "queue empty!" <<endl;
	}
	m_iHead = (m_iHead + 1)%m_iSize;

	return x
}
