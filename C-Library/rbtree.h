#ifndef _RED_BLACK_TREE_H_
#define _RED_BLACK_TREE_H_

#define RED		0   //red node
#define BLACK	1	//black node

struct rbtree_node {
	uint32_t char color;
	int key;
	struct rbtree_node *left;  	//left child
	struct rbtree_node *right;	//right child
	struct rbtree_node *parent; //parent
};

typedef struct rbtree_root {
	struct rbtree_node *node;
}RBROOT;

/* create a rbtree, and return the root of tree. */
RBROOT* create_rbtree();
void destroy_rbtree(RBROOT*);
int intsert_rbtree(RBROOT*, int key);
void delete_rbtree(RBROOT*, int key);

/**/
void preorder_rbtree(RBROOT*);
void inorder_rbtree(RBROOT*);
void postorder_rbtree(RBROOT*);

int rbtree_search(RBROOT*, int key);
int iterative_rbtree_search(RBROOT*, int key);

int rbtree_minimum(RBROOT*, int *val);
int rbtree_maximum(RBROOT*, int *val);

void print_rbtree(RBROOT*);

#endif
