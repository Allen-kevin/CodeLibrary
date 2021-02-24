#include <stdio.h>
#include <stdlib.h>
#include "rbtree.h"

#define rb_parent(r)	((r)->parent)
#define rb_color(r)		((r)->color)
#define rb_is_red(r)	((r)->color==RED)
#define rb_is_black(r)	((r)->color==BLACK)
#define rb_set_red(r)	do {(r)->color = RED;} while (0)
#define rb_set_black(r)	do {(r)->color = BLACK;} while (0)
#define rb_set_parent(r, p) do {(r)->parent = (p);} while (0)
#define rb_set_color(r, c) do {(r)->color = (c);} while (0)

/* Create a red-black tree */
RBROOT* create_rbtree()
{
	RBROOT *root = (RBROOT*)malloc(sizeof(RBROOT));
	root->node = NULL;

	return root;
}

/* Recursive preorder to traverse the red-black tree */
static void preorder(struct rbtree_node *tree)
{
	if (tree != NULL) {
		preorder(tree->left);
		preorder(tree->right);
	}
}

void preorder_tree(RBROOT *root)
{
	if (root)
		preorder(root->node);
}

/* Recursive inorder to traverse the red-black tree */
static void inorder(struct rbtree_node *tree)
{
	if (tree != NULL) {
		inorder(tree->left);
		inorder(tree->right);
	}
}

/* Recursively find the specified element */
static RBTREE* _recursive_rbtree_search(RBTREE *tree, TYPE key)
{
	if (tree == NULL || tree->key == key)
		return tree;

	if (tree->key > key)
		return _recursive_rbtree_search(tree->left, key);
	else
		return _recursive_rbtree_search(tree->right, key);
}

/* Iteratively find the specified element */
static RBTREE* _iterative_rbtree_search(RBTREE *tree, TYPE key)
{
	while ((tree != NULL) && (tree->key != key)) {
		if (tree->key > key)
			tree = tree->left;
		else
			tree = tree->right
	}

	return tree;
}

/* 
 * Find the specified element in the tree.
 *
 * @root, root node of tree
 * @key, the element need to be fond in tree
 * @return 
 * return -2, tree is null
 * return -1, not find specified element
 * return 0, success
 * */
int rbtree_search(RBROOT *root, TYPE key)
{
	if (unlikely(root == NULL))
		return -2;

	return _recursive_rbtree_search(root->node, key)?0:-1;
}


static RBTREE* _rbtree_minimum(RBTREE *tree)
{
	while (tree->left != NULL)
		tree = tree->left;

	return tree;
}

/* 
 * Find the smallest element in the tree, and return it via 
 * parameters.
 *
 * @return 
 * return 0, if find minimum value
 * otherwise return -1
 * */
int rbtree_minimum(RBROOT *root, TYPE *val)
{
	RBTREE *tree;

	if (unlikely(root == NULL) || unlikely(root->node == NULL))
		return -1;

	tree = _rbtree_minimum(root->node);
	*val = tree->key;

	return 0;
}


/*
 * Rotate a tree to the left.
 * */
static void rbtree_left_rotate(RBROOT *root, RBTREE *tree)
{
	RBTREE *node = tree->right;

	tree->right = node->left;
	if (node->left != NULL)
		node->left->parent = tree;

	node->parent = tree->parent;

	if (tree->parent == NULL)
		root->node = node;
	else {
		/* If tree was a left child before, then node should 
		 * also be a left child, and vice versa. */
		if (tree->parent->left == tree)
			x->parent->left = node;
		else
			x->parent->right = node;
	}

	/* When complete rotate, node will become parent of tree, and
	 * tree is node's child. */
	node->left = tree;
	tree->parent = node;
}

/*
 * Rotate a tree to the right.
 * */
static void rbtree_right_rotate(RBROOT *root, RBTREE *tree)
{
	RBTREE *node = tree->left;
	
	node->left = tree->right;
	if (x->right != NULL)
		tree->right->parent = node;

	tree->parent = node->parent;

	if (node->parent == NULL)
		root->node = tree;
	else {
		if (node->parent->right == node)
			node->parent->right = tree;
		else
			node->parent->left = tree;
	}

	tree->right = node;
	node->parent = tree;
}
