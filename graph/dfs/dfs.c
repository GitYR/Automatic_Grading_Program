#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#define MAX_VERTEX 10
#define TRUE 1
#define FALSE 0

/*
	1) 시작 정점 v를 결정하여 방문

	2) 정점 v에서 인접한 정점 중

		2-1) 방문하지 않은 정점 w가 있으면 정점 v를 스택에 push하고 w를 방문, w를 v로 하여 다시 2) 반복

		2-2) 방문하지 않은 정점이 없으면 스택을 다시 pop하여 받은 가장 마지막 방문지점을 v로 설정한 뒤 다시 2) 수행

	3) 스택이 공백이 될 때까지 2)를 반복
*/

// 정점의 정의
typedef struct graphNode
{
	int vertex;
	struct graphNode * link; // 연결 고리
}graphNode;

// 그래프의 정의
typedef struct graphType
{
	int n; // graph 정점의 개수
	graphNode * adjList_H[MAX_VERTEX]; // 각 정점에 대한 인접리스트( MAX_VERTEX : 10 ) - 포인터 배열
	int visited[MAX_VERTEX]; // 방문 여부
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

	if(!top) // top이 NULL이라면
	{
		printf("\n\n Stack is empty !\n");
		return 0;
	}
	else
	{
		item = temp->data;
		top = temp->link;
		free(temp);
		return item;
	}
}

void createGraph(graphType * g)
{
	int v;
	g->n = 0; // 시작 정점은 0번 정점(vertex)

	for(v = 0; v < MAX_VERTEX; v++) // 초기화
	{
		g->visited[v] = FALSE;
		g->adjList_H[v] = NULL;
	}
}

void insertVertex(graphType * g,int v)
{
	if(((g->n) + 1) > MAX_VERTEX)
	{
		printf("\n 그래프 정점의 개수를 초과하였습니다!\n");
		return;
	}
	g->n++;
}

// u :  , v :
void insertEdge(graphType * g,int u,int v)
{
	graphNode * node;

	if(u >= g->n || v >= g->n)
	{
		printf("\n 그래프에 없는 정점입니다!\n");
		return;
	}
	node = (graphNode *)malloc(sizeof(graphNode));
	node->vertex = v;
	node->link = g->adjList_H[u];
	g->adjList_H[u] = node;
}

void print_adjList(graphType * g)
{
	int i;
	graphNode * p;
	for(i = 0; i < g->n; i++)
	{
		printf("\n\t\t 정점 %c의 인접리스트", i+65);
		p = g->adjList_H[i];
		while(p)
		{
			printf(" -> %c", p->vertex+65);
			p = p->link;
		}
	}
}

void DFS_adjList(graphType * g,int v)
{
	graphNode * w;
	top = NULL;
	push(v);
	g->visited[v] = TRUE;
	printf(" %c", v+65);

	while(top != NULL)
	{
		w = g->adjList_H[v];
		while(w)
		{
			if(!g->visited[w->vertex])
			{
				push(w->vertex);
				g->visited[w->vertex] = TRUE;

				printf(" %c",w->vertex+65);
				v = w->vertex;
				w = g->adjList_H[v];
			}
			else
				w = w->link;
		}
		v = pop();
	}
}

int main(void)
{
	int i;
	graphType * G;
	G = (graphType *)malloc(sizeof(graphType)); // graph 생성(동적할당)

	createGraph(G);

	for(i = 0; i < 7; i++)
		insertVertex(G,i);

	insertEdge(G,0,2);
	insertEdge(G,0,1);
	insertEdge(G,1,4);
	insertEdge(G,1,3);
	
	insertEdge(G,1,0);
	insertEdge(G,2,4);
	insertEdge(G,2,0);
	insertEdge(G,3,6);

	insertEdge(G,3,1);
	insertEdge(G,4,6);
	insertEdge(G,4,2);
	insertEdge(G,4,1);
	
	insertEdge(G,5,6);
	insertEdge(G,6,5);
	insertEdge(G,6,4);
	insertEdge(G,6,3);

	printf("\n G의 인접 리스트 ");
	print_adjList(G);

	printf("\n\n//////////////\n\n깊이우선탐색 >> ");
	DFS_adjList(G,0);
	getchar();
}
