#include "list.h"

/* Initialize a Unidirectional linked list. */
void InitializeUnidirectionalList(struct LinkedList *list)
{
    list->head = NULL;
    list->tail = NULL;
    list->current = NULL;
}

/* add node to tail of single linked list. */
void AddUnidirectionalListTail(struct  LinkedList *list, void *data)
{
    struct Node *node = (struct Node*)malloc(sizeof(struct Node));
    node->data = data;
    node->next = NULL;
    if (list->head == NULL) {
        list->head = node;
    } else {
        list->tail->next = node;
    }

    list->tail = node;
}


/* add node to head of single linked list. */
void AddUnidirectionalListHead(struct  LinkedList *list, void *data)
{
    struct Node *node = (struct Node*)malloc(sizeof(struct Node));
    node->data = data;
    node->next = NULL;
    if (list->head == NULL) {
        list->tail = node;
    } else {
        node->next = list->head;
    }

    list->head = node;
}

/* Deleting a node from the unidrectional linked list. */
void DeleteUnidirectionalList(struct LinkedList *list, struct Node *node)
{
    /* linked list is empty. */
    if (list->head == NULL)
        exit(-1);
    /* the head node is that one needed be deleted. */
    if (node == list->head) {
        if (list->head->next == NULL) {
            list->head = list->tail = NULL;
        } else {
            list->head = list->head->next;
        }
    } else {/* the node that should be deleted is not the first node. */
        struct Node* tmp = list->head;
        while (tmp != NULL && tmp->next != node)
            tmp = tmp->next;

        if (tmp != NULL)
            tmp->next = node->next;
    }
    free(node);
}

/* Add a node to the doubly linked list's head. */
void AddDoublyListHead(struct LinkedList *list, void *data)
{
    struct DoublyListNode *node;
    node = (struct DoublyListNode *)malloc(sizeof(struct DoublyListNode));
    node->data = data;

    /* linked list is empty. */
    if (list->head == NULL) {
        list->tail = node; 
    } else {/* linked list is not empty. */
        node->next = list->head;
        list->prev = node;
    }

    list->head = node;
}

/* Add a node to the doubly linked list's tail. */
void AddDoublyListTail(struct LinkedList *list, void *data)
{
    struct DoubleListNode *node;
    node = (struct DoublyListNode *)malloc(sizeof(struct DoublyListNode));
    node->data = data;
    node->prev = NULL;
    node->next = NULL;

    /* linked list is empty. */
    if (list->head = NULL) {
        list->head = node;
    } else {/* linked list is not empty. */
        list->tail->next = node;
        node->prev = list->tail;
    }

    list->tail = node;
}

/* Delete a node from the doubly linked list. */
void DeleteDoublyListNode(struct LinkedList *list, struct DoublyListNode *node)
{
    /* doubly linked list is empty. */
    if (list->head == NULL || list->tail == NULL)
        exit(-1);
    
    /* the node to be deleted is the first node. */
    if (node == list->head) {
        /* the doubly linked list has only one node. */
        if (list->head->next == NULL) {
            list->head = list->tail = NULL;
        } else {
            list->head = list->head->next;
            list->head->prev = NULL;
        }
    } else {
        struct DoublyListNode *tmp;
        tmp = list->head;

        while (tmp != NULL && tmp->next != node)
            tmp = tmp->next;

        if (tmp != NULL) {
            tmp->next = node->next;
            node->next->prev = tmp;
        }
    }

    free(node);
}
