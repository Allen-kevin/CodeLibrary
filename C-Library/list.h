#include <stdio.h>
#include <stdlib.h>
#include <cstring.h>

/* single linked list node. */
struct Node {
   char *data;
   struct Node *next;
};

struct LinkedList {
    struct Node *head;
    struct Node *tail;
    struct Node *current;
};

/* doubly linked list node. */
struct DoublyListNode {
    char *data;
    struct DoublyListNode *prev;
    struct DoubleListNode *next;
};

/* Adding a node to the head of unidirectional linked list. */
void AddUnidirectionalListHead(struct  LinkedList *list, void *data);

/* Adding a node to the tail of unidirectional linked list. */
void AddUnidirectionalListTail(struct  LinkedList *list, void *data);

/* Deleting a node from the unidrectional linked list. */
void DeleteUnidirectionalList(struct LinkedList *list, struct Node *node);

/* Initialize a unidirectional linked list. */
void InitializeUnidirectionalList(struct LinkedList *list);

/* Adding a node to the head of doubly linked list. */
void AddDoublyListHead(struct LinkedList *list, void *data);

/* Adding a node to the tail of doubly linked list. */
void AddDoublyListTail(struct LinkedList *list, void *data);

/* Deleting a node from the doubly linked list. */
void DeleteDoublyListNode(struct LinkedList *list, struct DoublyListNode *node);
