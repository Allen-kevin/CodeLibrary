#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>

using namespace std;

template <class T>
struct StackNode {
	T data;
	StackNode* next;
};

template <typename T>
class LinkStack {
private:
	StackNode<T>* top;

public:
	LinkStack();
	~LinkStack();
	bool StackEmpty();//stack is empty.
	void ClearStack();
	void push(T elem);
	void pop();
	int StackLength();
	T StackTop();
	void StackTranverse();
};

/* constructor function. 
 * 初始化时将top指针置空，避免当遍历到栈底时出现异常
 * */
template <typename T>
LinkStack<T>::LinkStack()
{
	top = NULL;
}

/* destructor function. 
 * 在析构函数中，所有的结构都要进行内存释放。
 * */
template <typename T>
LinkStack<T>::~LinkStack()
{
	StackNode<T>* node = NULL;
	while (top != NULL) {
		node = top;
		top = top->next;
		delete node;
	}
}

/* 判断栈是否为空，直接根据top指针是否指向NULL便可。*/
template <typename T>
bool LinkStack<T>::StackEmpty()
{
	return top == NULL ? true : false;
}

template <typename T>
void LinkStack<T>::ClearStack()
{
	StackNode<T>* node = NULL;
	while (top != NULL) {
		node = top;
		top = top->next;
		delete node;
	}
}

/* 根据栈的特点先进后出，所以入栈时直接将新节点插入到链表的最前面
 * 这样出栈时方便。
 * */
template <typename T>
void LinkStack<T>::push(T data)
{
	StackNode<T>* node = new StackNode<T>;
	node->data = data;
	node->next = top;
	top = node;
}

/* 出栈直接删除第一个节点便可。*/
template <typename T>
void LinkStack<T>::pop()
{
	//出栈前判断栈是否为空
	if (StackEmpty()) {
		cout << "This is a empty stack, no element to deleted!";
		return;
	}

	StackNode<T>* node = top;
	top = top->next;
	delete node;
}

template <typename T>
T LinkStack<T>::StackTop()
{
	if (StackEmpty()) {
		throw  "There is no element in stack!";
	}

	return top->data;
}

template <typename T>
int LinkStack<T>::StackLength()
{
	int length = 0;
	StackNode<T>* node = top;
	while (node != NULL) {
		node = node->next;
		length++;
	}

	return length;
}

template <typename T>
void LinkStack<T>::StackTranverse()
{
	StackNode<T>* node = top;
	while (node != NULL) {
		cout << node->data << " ";
		node = node->next;
	}
	cout << endl;
}
