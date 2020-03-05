#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#define MAX_VERTEX 10
#define TRUE 1
#define FALSE 0

#define ERROR_CODE 0x7FFFFFFF

typedef _Bool bool;

typedef struct graphNode
{
	int vertex;
	struct graphNode * link;
}graphNode;

typedef struct graphType
{
	int n; // the number of vertexs
	graphNode * adjList[MAX_VERTEX];
	bool visited[MAX_VERTEX];	
}graphType;

typedef struct stackNode
{
	int data;
	struct stackNode * link;
}stackNode;

stackNode * top;

void push(int item)
{
	stackNode * temp = (stackNode *)malloc(sizeof(stackNode));
	temp->data = item;
	temp->link = top;
	top = temp;
}

int pop(void)
{
	int item;
	stackNode * temp = top;

	if(!top) // top == NULL
	{
		fprintf(stderr,"stack is empty now\n");
		return ERROR_CODE; // the maximum value of integer
	}
	item = top->data;
	top = top->link;
	free(temp);
	return item;
}

int create_graph(graphType * G)
{
	int i;

	G = (graphType *)malloc(sizeof(graphType));	
	G->n = 0;
	for(i = 0; i < MAX_VERTEX; i++)
	{
		G->adjList[i] = NULL;
		G->visited[i] = FALSE;
	}
}

int insert_vertex(graphType * G,int v)
{
	if(v > G->n && v >= MAX_VERTEX)
	{
		fprintf(stderr,"%d is out of the maximum vertex's index\n", v);
		return ERROR_CODE;
	}
	G->n++;
}

int insert_edge(graphType * G,int u,int v)
{
	if(u >= MAX_VERTEX || v >= MAX_VERTEX)
	{
		fprintf(stderr,"out of boundary\n");
		return ERROR_CODE;
	}
	graphNode * node = (graphNode *)malloc(sizeof(graphNode *));
	node->vertex = v;
	node->link = G->adjList[u];
	G->adjList[u] = node;
	// 먼저 생긴 노드일 수록 밑으로 내려감
}

void print_graph(graphType * G)
{
	int i;
	graphNode * v;

	for(i = 0; i < G->n; i++)
	{
		printf("%c의 인접리스트 ",i+65);
		v = G->adjList[i];
		while(v)
		{
			printf("-> %c ",(v->vertex)+65);
			v = v->link;
		}
		printf("\n");
	}
}

void dfs(graphType * G,int v) // 깊이 우선 방문
{
	graphNode * w;
	top = NULL;
	push(v);
	G->visited[v] = TRUE;
	printf(" %c", v+65);

	while(top)
	{
		w = G->adjList[v]; // v의 인접리스트로부터 시작

		while(w)
		{
			if(!G->visited[w->vertex]) // 방문하지 않았다면
			{
				push(w->vertex); // 스택에 추가
				G->visited[w->vertex] = TRUE; // 방문 처리
				printf(" %c", w->vertex+65);

				v = w->vertex;
				w = G->adjList[v];
			}
			else
				w = w->link;
		}
		v = pop(); // 모든 인접리스트를 방문했다면 그 전에 방문했던 곳으로 돌아가기
	}
}

int main(void)
{
	int i;
	graphType * G;
	G = (graphType *)malloc(sizeof(graphType)); // graph 생성(동적할당)

	create_graph(G);

	for(i = 0; i < 7; i++)
		insert_vertex(G,i);

	insert_edge(G,0,2);
	insert_edge(G,0,1);
	insert_edge(G,1,4);
	insert_edge(G,1,3);
	
	insert_edge(G,1,0);
	insert_edge(G,2,4);
	insert_edge(G,2,0);
	insert_edge(G,3,6);

	insert_edge(G,3,1);
	insert_edge(G,4,6);
	insert_edge(G,4,2);
	insert_edge(G,4,1);
	
	insert_edge(G,5,6);
	insert_edge(G,6,5);
	insert_edge(G,6,4);
	insert_edge(G,6,3);

	printf("\n G의 인접 리스트\n");
	print_graph(G);

	printf("\n\n//////////////\n\n깊이우선탐색 >> ");
	dfs(G,0);
	getchar();
}
