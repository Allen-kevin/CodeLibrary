#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>

using namespace std;

/* ArrayStack.cpp */
template <typename T>
class ArrayStack {
private:
	T *m_pStack;	//stack index.
	int m_iSize;	//stack capacity.
	int m_iTop;		//stack top.
public:
	ArrayStack(int size);
	~ArrayStack();
	bool StackEmpty(); //stack is empty.
	bool StackFull();		//stack is full.
	void ClearStack();		//clear stack.
	int StackLength();		//stack's length.
	void push(T elem);  //add a elem to stack top.
	void pop();  //delete elem form stack top
	T StackTop();	//return stack top.
	void StackTranverse();	//tranverse stack.
};


/* constructor */
template <typename T>
ArrayStack<T>::ArrayStack(int size)
{
	m_iSize = size;
	m_pStack = new T[m_iSize];
	m_iTop = 0;
}

/* destructor */
template <typename T>
ArrayStack<T>::~ArrayStack()
{
	delete m_pStack;
	m_pStack = NULL;
}

template <typename T>
bool ArrayStack<T>::StackEmpty()
{
	return m_iTop == 0 ? true:false;
}

template <typename T>
bool ArrayStack<T>::StackFull()
{
	return m_iTop == m_iSize ? true:false;
}

template <typename T>
int ArrayStack<T>::StackLength()
{
	return m_iTop;
}

/* clear stack */
template <typename T>
void ArrayStack<T>::ClearStack()
{
	m_iTop = 0;
}

/* add elem to stack */
template <typename T>
void ArrayStack<T>::push(T elem)
{
	if (StackFull()) {
		throw "stack full!";
	}
	else {
		m_pStack[m_iTop++] = elem;
	}
}

/* delete and return elem from stack */
template <typename T>
void ArrayStack<T>::pop()
{
	if (StackEmpty())
		throw "empth stack!";

	m_iTop--;
}

/* return elem from stack top */
template <typename T>
T ArrayStack<T>::StackTop()
{
	if (StackEmpty())
	{
		throw "empty stack!";
	}

	return m_pStack[m_iTop-1];
}

/* transverse stack */
template <typename T>
void ArrayStack<T>::StackTranverse()
{
	int i = 0;
	for (i = 0; i < m_iTop; i++)
		cout << m_pStack[i] << " ";
	cout << endl;
}
