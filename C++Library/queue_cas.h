#include <iostream>

template<typename ElemType>
class Queue {
public:
	Queue();
	~Queue();
public:
	void push(ElemType elem);
	bool pop();
	bool show();
private:
	struct _qNode {
		_qNode(): _next(nullptr) { }
		_qNode(ElemType elem): _elem(elem), _next(nullptr) { }
		ElemType	_elem;
		struct _qNode *_next;

	};
private:
	struct _qNode *_head;
	struct _qNode *_tail;
};

template<typename ElemType>
Queue<ElemType>::Queue()
{
	_head = _tail = new _qNode();
}

template<typename ElemType>
Queue<ElemType>::~Queue()
{
	while (_head != nullptr) {
		struct _qNode *tempNode = _head;
		_head = _head->_next;
		delete tempNode;
	}
}

template<typename ElemType>
void Queue<ElemType>::push(ElemType elem)
{
	struct _qNode *newNode = new struct _qNode(elem);

	struct _qNode *p = _tail;
	struct _qNode *oldp = _tail;

	do {
		while (p->_next != nullptr)
			p = p->_next;
	} while(__sync_bool_compare_and_swap(&_tail->_next, nullptr,
				newNode) != true);
	__sync_bool_compare_and_swap(&_tail, oldp, newNode);
}

template<typename ElemType>
bool Queue<ElemType>::pop()
{
	struct _qNode *p;
	do {
		p = _head;
		if (p->_next == nullptr)
			return false;
	} while (__sync_bool_compare_and_swap(&_head, p, p->_next) 
			!= true);
	delete p;
	return true;
}

template<typename ElemType>
void Queue<ElemType>::show()
{
	struct _qNode* tempNode = _head->_next;

	if (tempNode == nullptr) {
		std::out << "Empty" << std::endl;
		return;
	}
	while (tempNode != null) {
		std::cout << tempNode->_elem << " ";
		tempNode = tempNode->_next;
	}

	std::cout << std::endl;
}
