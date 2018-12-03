#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include "algorithm.h"

using namespace std;

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
bool stack<T>::StackEmpty()
{
	return m_iTop == 0 ? true:false;
}

template <typename T>
bool stack<T>::StackFull()
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
	if (StackFull()) {
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
	if (StackEmpty())
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
	if (StackEmpty())
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
		cout << m_pStack[i];
}
