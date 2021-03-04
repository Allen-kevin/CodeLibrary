#include <stdio.h>
#include "../rbtree.h"

#define CHECK_INSERT 1
#define CHECK_DELETE 1
#define LENGTH(a) ((sizeof(a))/(sizeof(a[0])))

int main()
{
	int arr[] = {10, 40, 30, 60, 90, 70, 20, 50, 80};
	int i, len = LENGTH(arr);
	RBROOT *root = NULL;

	root = create_rbtree();
	printf("== 原始数据： ");
	for (i = 0; i < len; i++)
		printf("%d ", arr[i]);
	printf("\n");

	for (i = 0; i < len; i++) {
		rbtree_insert(root, arr[i]);
#if CHECK_INSERT
		printf ("== 添加节点： %d\n", arr[i]);
		printf("== 树的详细信息：\n");
		rbtree_print(root);
		printf("\n");
#endif
	}

#if CHECK_DELETE
	for (i = 0; i < len; i++) {
		rbtree_delete(root, arr[i]);
		printf("== 删除节点：%d\n", arr[i]);
		if (root) {
			printf("== 树的详细信息：\n");
			rbtree_print(root);
			printf("\n");
		}
	}
#endif
	rbtree_destroy(root);

	return 0;
}
