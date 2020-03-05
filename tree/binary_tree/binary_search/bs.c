#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TRUE 1
#define FALSE 0

typedef struct _treeNode
{
	int key;
	struct _treeNode * left, * right;
}TreeNode;

void insert_node(TreeNode ** root,int key) // why this function uses a double pointer?
{
	TreeNode *p,*t;
	TreeNode *n;

	t = *root;
	p = NULL;

	while(t != NULL)
	{
		if(key == t->key) // 이미 존재함
			return;
		p = t;
		if(key < t->key)
			t = t->left;
		else
			t = t->right;
	}

	n = (TreeNode *)malloc(sizeof(TreeNode));
	if(n == NULL) // exception handling
		return;
	n->key = key;
	n->left = n->right = NULL;

	if(p != NULL)
	{
		if(key < p->key)
			p->left = n;
		else
			p->right = n;
	}
	else
		*root = n;
}

void delete_node(TreeNode ** root,int key)
{
	TreeNode *p,*child,*succ,*succ_p,*t;

	p = NULL;
	t = *root;

	while(t != NULL && t->key != key) // 탐색
	{
		p = t;
		t = (key < t->key) ? t->left : t->right;
	}

	if(t == NULL)
	{
		printf("key is not in the tree\n");
		return;
	}

	if((t->left == NULL) && (t->right == NULL)) // no children
	{
		if(p != NULL)
		{
			if(p->left == t)
				p->left = NULL;
			else
				p->right = NULL;
		}
		else
			*root = child;
	}
	else if((t->left == NULL) || (t->right == NULL)) // 반반
	{
		child = (t->left != NULL) ? t->left : t->right;
		if(p != NULL)
		{
			if(p->left == t)
				p->left = child;
			else
				p->right = child;
		}
		else
			*root = child;
	}
	else // 자식이 모두 존재
	{
		succ_p = t; // parent
		succ = t->right; // t의 오른쪽 자식

		// 왜 좌측으로 갈까? -> 이진 탐색 논리에 따라 좌측은 부모보다 작은 값, 우측은 부모보다 큰 값이므로
		while(succ->left != NULL) // (t->right)의 왼쪽 끝까지 가보자
		{
			succ_p = succ;
			succ = succ->left;
		}
		if(succ_p->left == succ)
			succ_p->left = succ->right;
		else
			succ_p->right = succ->right;
		t->key = succ->key; // 값 바꿔치기 (삭제할 노드 t에 succ->key를 넣어주고)
		t = succ; // succ를 삭제하기 위해 바꿔치기
	}
	free(t);
}

int main(void)
{
	int i;
	TreeNode * root;

	srand(time(NULL));

	for(i = 0; i < 8; i++)
	{
		int random = rand() % 100;
		insert_node(&root,random);
	}
	exit(EXIT_SUCCESS);
}
