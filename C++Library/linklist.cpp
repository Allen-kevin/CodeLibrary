#include "algorithm.h"

int LinkList::Length()
{
	/* if list is empty! */
	if (head == NULL)
		return 0;

	Node *node = head;
	int length = 0;
	/* Use length count the length of list. */
	while (node != NULL) {
		length++;
		node = node->next;
	}
	
	delete node;//free node
	return length;
}

Node* LinkList::Insert(int value, int position)
{

	Node *node = new Node(value); 
	if (head == NULL) {
		head = node;
		return head;
	}

	if (position == 0) {
		node->next = head;
		head = node;
		return head;
	}
	if (position > Length()) {
		cout << "The index has more than the max length of link!" << endl;
		return NULL;
	}
	cout << "hello" << endl;
	int i = 0;
	Node *current_node = head;
	while (current_node->next != NULL && i < position-1) {
		current_node = current_node->next;
		i++;
	}
	if (i == position-1) {
		node->next = current_node->next;
		current_node->next = node;
	}

	return head;
}

void LinkList::PositivePrint(Node* head)
{
	if (head == NULL) {
		cout << "The link is empty!" << endl;
		return;
	}
	Node* current_node = head;
	while (current_node != NULL) {
		cout << current_node->data << " ";
		current_node = current_node->next;
	}
	cout << endl;
}


void LinkList::ReversePrint(Node* head)
{
	if (head == NULL) {
		cout << "The link is empty!" << endl;
		return;
	}
	vector<int> vec;
	Node* current_node = head;
	while (current_node != NULL) {
		vec.insert(vec.begin(),current_node->data);
		current_node = current_node->next;
	}
	for (int i = 0; i < vec.size(); i++)
		cout << vec[i] << " ";
	cout << endl;
}

void LinkList::Delete(int position) 
{
	if (head == NULL)
		return;
	Node* current_node = head;
	int i = 0;
	if (position == 0) {
		head = head->next;
		delete current_node;
		return;
	}
	if (position > Length()) {
		cout << "Index more than the max length of list!" << endl;
		return;
	}
	while (current_node->next != NULL && i < position-1) {
		current_node = current_node->next;
		i++;
	}
	if (i == position-1) {
		Node* delete_node = current_node->next;
		current_node->next = current_node->next->next;
		delete delete_node;
	}
}


int LinkList::Search(int value)
{
	int i = 0;
	//list is empty
	if (head == NULL) {
		cout << "Sorry, the List is empty!" << endl;
		return -1;
	}
	//walk list to find value
	Node *node = head;
	while (node != NULL && node->data != value) {
		i++;
		node = node->next;
	}

	if (node == NULL)
		cout << "Don't find the value you want!" << endl;
	else {
		return i;
	} 

	return -1;
}

Node* LinkList::Merge(Node* head, Node* head1)
{	
	/* If one of them is empty, then return the another one. */
	if (head == NULL)
		return head1;
	if (head1 == NULL)
		return head;

	/* Begin index to head of list, and end index to tail of list. */
	Node *begin = NULL;
	Node *end = NULL;
	begin = new Node(-1);
	end = begin; 
	while (head != NULL && head1 != NULL) {
		//With less element add to tail of new list.
		if (head->data < head1->data) {
			end->next = head;
			end = end->next;
			head = head->next;
		} else {
			end->next = head1;
			end = end->next;
			head1 = head1->next;
		}
	}
	//Add the remaining nodes to the tail of new list.
	if (head != NULL)
		end->next = head;
	if (head1 != NULL)
		end->next = head1;
	//Delete the first node of new list.
	Node *temp = begin;
	begin = begin->next;
	delete temp;

	return begin;
}

/* Find mid node of list. */
Node* GetMid(Node* head)
{
	if (head == NULL)
		return NULL;
	if (head->next == NULL)
		return head;

	/* Creating two index, fast's rate is twice of slow. when fast index 
	 * to tail of list, slow will index to mid of list.
	 * */
	Node *slow = head;
	Node *fast = head->next;

	while (fast && fast->next) {
		slow = slow->next;
		fast = fast->next->next;
	}

	return slow;
}

Node* LinkList::Sort(Node* head)
{
	if (head == NULL) {
		cout << "The list is empty" << endl;
		return NULL;
	}
	if (head->next == NULL)
		return head;

	Node *mid = GetMid(head);
	Node *nextPart = NULL;
	if (mid != NULL) {
		nextPart = mid->next;
		mid->next = NULL;
	}

	return Merge(Sort(head), Sort(nextPart));
}

Node* LinkList::ReverseList(Node* head)
{
	if (head == NULL || head->next == NULL)
		return head;
	else {
		Node *node = ReverseList(head->next);
		head->next->next = head;
		head->next = NULL;
		
		return node;
	}
}
