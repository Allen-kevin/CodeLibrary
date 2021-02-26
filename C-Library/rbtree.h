#ifndef _RED_BLACK_TREE_H_
#define _RED_BLACK_TREE_H_

#include <stdint.h>
#define RED		0   //red node
#define BLACK	1	//black node
typedef int TYPE;

#define likely(x)	__builtin_expect(!!(x), 1)
#define unlikely(x)	__builtin_expect(!!(x), 0)
typedef struct rbtree_node {
	unsigned char color;
	int key;
	struct rbtree_node *left;  	//left child
	struct rbtree_node *right;	//right child
	struct rbtree_node *parent; //parent
}RBTREE;

typedef struct rbtree_root {
	struct rbtree_node *node;
}RBROOT;

/* create a rbtree, and return the root of tree. */
RBROOT* create_rbtree();
void rbtree_destroy(RBROOT*);
int rbtree_insert(RBROOT*, int key);
void rbtree_delete(RBROOT*, int key);

/**/
void preorder_rbtree(RBROOT*);
void inorder_rbtree(RBROOT*);
void postorder_rbtree(RBROOT*);

int rbtree_search(RBROOT*, int key);
int iterative_rbtree_search(RBROOT*, int key);

int rbtree_minimum(RBROOT*, int *val);
int rbtree_maximum(RBROOT*, int *val);

void rbtree_print(RBROOT*);

#endif
