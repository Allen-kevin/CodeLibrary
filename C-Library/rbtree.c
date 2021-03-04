#include <stdio.h>
#include <stdlib.h>
#include "rbtree.h"

#define RB_PARENT(r)	((r)->parent)
#define RB_COLOR(r)		((r)->color)
#define RB_IS_RED(r)	((r)->color==RED)
#define RB_IS_BLACK(r)	((r)->color==BLACK)
#define RB_SET_RED(r)	do {(r)->color = RED;} while (0)
#define RB_SET_BLACK(r)	do {(r)->color = BLACK;} while (0)
#define RB_SET_PARENT(r, p) do {(r)->parent = (p);} while (0)
#define RB_SET_COLOR(r, c) do {(r)->color = (c);} while (0)

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
			tree = tree->right;
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
static void rbtree_left_rotate(RBROOT *root, RBTREE *x)
{
	RBTREE *y = x->right;

	x->right = y->left;
	if (y->left != NULL)
		y->left->parent = x;

	y->parent = x->parent;

	if (x->parent == NULL)
		root->node = y;
	else {
		/* If tree was a left child before, then node should 
		 * also be a left child, and vice versa. */
		if (x->parent->left == x)
			x->parent->left = y;
		else
			x->parent->right = y;
	}

	/* When complete rotate, node will become parent of tree, and
	 * tree is node's child. */
	y->left = x;
	x->parent = y;
}

/*
 * Rotate a tree to the right.
 * */
static void rbtree_right_rotate(RBROOT *root, RBTREE *y)
{
	RBTREE *x = y->left;
	
	y->left = x->right;
	if (x->right != NULL)
		x->right->parent = y;

	x->parent = y->parent;

	if (y->parent == NULL)
		root->node = x;
	else {
		if (y->parent->right == y)
			y->parent->right = x;
		else
			y->parent->left = x;
	}

	x->right = y;
	y->parent = x;
}


static void rbtree_insert_fixup(RBROOT *root, RBTREE *node)
{
	RBTREE *parent, *gparent;

	while ((parent = RB_PARENT(node)) && RB_IS_RED(parent)) {
		gparent = RB_PARENT(parent);

		if (parent == gparent->left) {
			{
				RBTREE *uncle = gparent->right;
				if (uncle && RB_IS_RED(uncle)) {
					RB_SET_BLACK(uncle);
					RB_SET_BLACK(parent);
					RB_SET_RED(gparent);
					node = gparent;
					continue;
				}
			}
			
			/* case 2:
			 * uncle is black, and node is right child. */
			if (parent->right == node) {
				RBTREE *tmp;
				rbtree_left_rotate(root, parent);
				tmp = parent;
				parent = node;
				node = tmp;
			}

			/* case 3:
			 * uncle is black, and node is left child. */
			RB_SET_BLACK(parent);
			RB_SET_RED(gparent);
			rbtree_right_rotate(root, gparent);
		} else {//
			/* uncle is red*/
			{
				RBTREE *uncle = gparent->left;
				if (uncle && RB_IS_RED(uncle)) {
					RB_SET_BLACK(uncle);
					RB_SET_BLACK(parent);
					RB_SET_RED(gparent);
					node = gparent;
					continue;
				}
			}

			if (parent->left == node) {
				RBTREE *tmp;
				rbtree_right_rotate(root, parent);
				tmp = parent;
				parent = node;
				node = tmp;
			}

			RB_SET_BLACK(parent);
			RB_SET_RED(gparent);
			rbtree_left_rotate(root, gparent);

		}
	}
	/* root node need to be black */
	RB_SET_BLACK(root->node);
}


static void _rbtree_insert(RBROOT *root, RBTREE *node)
{
	RBTREE *y = NULL;
	RBTREE *x = root->node;

	while (x != NULL) {
		y = x;
		if (node->key < x->key)
			x = x->left;
		else
			x = x->right;
	}
	RB_PARENT(node) = y;

	if (y != NULL) {
		if (node->key < y->key)
			y->left = node;
		else
			y->right = node;
	} else {
		root->node = node;
	}

	node->color = RED;

	rbtree_insert_fixup(root, node);
}

static RBTREE* create_rbtree_node(TYPE key, RBTREE *parent, 
		RBTREE *left, RBTREE *right)
{
	RBTREE *p;
	if ((p = (RBTREE*)malloc(sizeof(RBTREE))) == NULL)
		return NULL;

	p->key = key;
	p->left = left;
	p->right = right;
	p->parent = parent;
	p->color = BLACK; //default color

	return p;
}


/*
 * Create a node, and insert it to rbtree.
 *
 * @root: root of red-black tree
 * @key : key value of waiting to be insertd  
 * @return
 * -1, insert failure
 * 0, insert success
 **/
int rbtree_insert(RBROOT *root, TYPE key)
{
	RBTREE *node;

	if (_recursive_rbtree_search(root->node, key) != NULL)
		return -1;

	if ((node = create_rbtree_node(key, NULL, NULL, NULL)) == NULL)
		return -1;

	_rbtree_insert(root, node);

	return 0;
}


static void rbtree_delete_fixup(RBROOT *root, RBTREE *node, RBTREE *parent)
{
	RBTREE *other;

	while ((!node || RB_IS_BLACK(node)) && node != root->node) {
		if (parent->left == node) {
			/* the node other is brother of node. */
			other = parent->right;
			/* case 1:
			 * the node other is red */
			if (RB_IS_RED(other)) {
				RB_SET_BLACK(other);	
				RB_SET_RED(parent);
				rbtree_left_rotate(root, parent);
				other = parent->right;
			}

			/* case 2:
			 * the node other is black, both child is black */
			if ((!other->left || RB_IS_BLACK(other->left)) &&
				(!other->right || RB_IS_BLACK(other->right))) {
				RB_SET_RED(other);
				node->parent;
				parent = RB_PARENT(node);
			} else {
				/* case 3:
				 * the node other is black, and other's
				 * left child is red, right child is black.*/
				if (!other->right || RB_IS_BLACK(other->right)) {
								RB_SET_BLACK(other->left);
					RB_SET_RED(other);
					rbtree_right_rotate(root, other);
					other = parent->right;
				}
				
				/* case 4:
				 * the node other is black, and other's right
				 * child is red, left child can be any color. */
				RB_SET_COLOR(other, RB_COLOR(parent));
				RB_SET_BLACK(parent);
				RB_SET_BLACK(other->right);
				rbtree_left_rotate(root, parent);
				node = root->node;
				break;
			}
		} else {
			other = parent->left;
			/* case 1:
			 * the node other is red */
			if (RB_IS_RED(other)) {
				RB_SET_BLACK(other);
				RB_SET_RED(parent);
				rbtree_right_rotate(root, parent);
				other = parent->left;
			}
			if ((!other->left || RB_IS_BLACK(other->left)) &&
				(!other->right || RB_IS_BLACK(other->right))) {
				RB_SET_RED(other);
				node = parent;
				parent = RB_PARENT(node);
			} else {
				if (!other->left || RB_IS_BLACK(other->left)) {
					RB_SET_BLACK(other->right);
					RB_SET_RED(other);
					rbtree_left_rotate(root, other);
					other = parent->left;
				}

				RB_SET_COLOR(other, RB_COLOR(parent));
				RB_SET_BLACK(parent);
				RB_SET_BLACK(other->left);
				rbtree_right_rotate(root, parent);
				node = root->node;
				break;
			}
		}
	}

	if (node)
		RB_SET_BLACK(node);
}


void _rbtree_delete(RBROOT *root, RBTREE *node)
{
	RBTREE *child, *parent;
	int color;

	if ( (node->left != NULL) && (node->right != NULL)) {
		RBTREE *replace = node;

		replace = replace->right;
		while (replace->left != NULL)
			replace = replace->left;

		if (RB_PARENT(node)) {
			if (RB_PARENT(node)->left == node)
				RB_PARENT(node)->left = replace;
			else
				RB_PARENT(node)->right = replace;
		} else
			root->node = replace;

		child = replace->right;
		parent = RB_PARENT(replace);
		color = RB_COLOR(replace);

		if (parent == node)
			parent = replace;
		else {
			if (child)
				RB_SET_PARENT(child, parent);
			parent->left = child;

			replace->right = node->right;
			RB_SET_PARENT(node->right, replace);
		}

		replace->parent = node->parent;
		replace->color = node->color;
		replace->left = node->left;
		node->left->parent = replace;

		if (color == BLACK)
			rbtree_delete_fixup(root, child, parent);
		free(node);

		return;
	}

	if (node->left != NULL)
		child = node->left;
	else
		child = node->right;

	parent = node->parent;
	color = node->color;

	if (child)
		child->parent = parent;

	if (parent) {
		if (parent->left == node)
			parent->left = child;
		else
			parent->right = child;
	}
	else
		root->node = child;

	if (color == BLACK)
		rbtree_delete_fixup(root, child, parent);

	free(node);
}


void rbtree_delete(RBROOT *root, TYPE key)
{
	RBTREE *z, *node;

	if ((z = _recursive_rbtree_search(root->node, key)) != NULL)
		_rbtree_delete(root, z);
}


static void _rbtree_destroy(RBTREE *tree)
{
	if (tree == NULL)
		return;
	if (tree->left != NULL)
		_rbtree_destroy(tree->left);
	if (tree->right != NULL)
		_rbtree_destroy(tree->right);

	free(tree);
}


void rbtree_destroy(RBROOT *root)
{
	if (root != NULL)
		_rbtree_destroy(root->node);

	free(root);

}


/*
 * Print red-black tree. 
 *
 * @tree : node of red-black tree
 * @key  : value of node
 * @direction: 
 * 		0, the node is root,
 * 		-1, the node is left child,
 * 		1, the node is right child.
 * */
static void _rbtree_print(RBTREE *tree, TYPE key, int direction)
{
	if (tree != NULL) {
		if (direction == 0)
			printf("%2d(B) is root\n", tree->key);
		else
			printf("%2d(%s) is %2d's %6s child\n", 
					tree->key, RB_IS_RED(tree)?"R":"B",
					key, direction==1?"right":"left");
		_rbtree_print(tree->left, tree->key, -1);
		_rbtree_print(tree->right, tree->key, 1);
	}
}

void rbtree_print(RBROOT *root)
{
	if (root != NULL && root->node != NULL)
		_rbtree_print(root->node, root->node->key, 0);
}
