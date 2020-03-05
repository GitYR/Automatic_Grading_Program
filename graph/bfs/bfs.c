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
	int n;
	graphNode * adjList[MAX_VERTEX];
	bool visited[MAX_VERTEX];
}graphType;

typedef struct QNode
{
	int data;
	struct QNode * link;
}QNode;

typedef struct LQueueType
{
	QNode *front,*rear;
}LQueueType;

LQueueType * createLinkQueue(void)
{
	LQueueType *LQ;
	LQ = (LQueueType *)malloc(sizeof(LQueueType));
	LQ->front = NULL;
	LQ->rear = NULL;
	return LQ;
}

int isEmpty(LQueueType *LQ)
{
	if(LQ->front == NULL)
	{
		fprintf(stderr,"\n Linked Queue is empty! \n");
		return TRUE;
	}
	return FALSE;
}

/* insert a data into the queue */
void enQueue(LQueueType *LQ,int item)
{
	QNode * newNode = (QNode *)malloc(sizeof(QNode));
	newNode->data = item;
	newNode->link = NULL;

	if(!LQ->front)
	{
		LQ->front = newNode;
		LQ->rear = newNode;
	}
	else
	{
		LQ->rear->link = newNode;
		LQ->rear = newNode;
	}
}

/* delete a data from the queue */
int deQueue(LQueueType *LQ)
{
	QNode *old = LQ->front;
	int item;

	if(isEmpty(LQ))
		return ERROR_CODE;
	item = old->data;
	LQ->front = LQ->front->link;
	if(!LQ->front)
		LQ->rear = NULL;
	free(old);
	return item;
}

void createGraph(graphType * g)
{
	int v;
	g->n = 0;

	for(v = 0; v < MAX_VERTEX; v++)
	{
		g->visited[v] = FALSE;
		g->adjList[v] = NULL;
	}
}

void insertVertex(graphType * g,int v)
{
	if(((g->n) + 1)> MAX_VERTEX)
	{
		fprintf(stderr,"\n 그래프 정점의 개수를 초과하였습니다!");
		return;
	}
	g->n++;
}

void insertEdge(graphType * g,int u,int v)
{
	graphNode * node;

	if(u >= g->n || v >= g->n)
	{
		fprintf(stderr,"\n 그래프에 없는 정점입니다!");
		return;
	}

	node = (graphNode *)malloc(sizeof(graphNode));
	node->vertex = v;
	node->link = g->adjList[u];
	g->adjList[u] = node;
}

void print_adjList(graphType * g)
{
	int i;
	graphNode * p;

	for(i = 0; i < g->n; i++)
	{
		printf("\n\n정점 %c의 인접리스트",i+65);
		p = g->adjList[i];
		while(p)
		{
			printf(" -> %c",p->vertex+65);
			p = p->link;
		}
	}
}

void bfs(graphType * g,int v)
{
	graphNode * w;
	LQueueType * Q;

	Q = createLinkQueue();

	g->visited[v] = TRUE;
	printf(" %c",v+65);
	enQueue(Q,v);

	while(!isEmpty(Q))
	{
		v = deQueue(Q);

		for(w = g->adjList[v]; w; w=w->link)
		{
			if(!g->visited[w->vertex]) // if the vertex is not visited so far,
			{
				g->visited[w->vertex] = TRUE;
				printf(" %c",w->vertex+65);
				enQueue(Q,w->vertex);
			}
		}
	}
}

void main(void)
{
	int i;
	graphType * G;
	G = (graphType *)malloc(sizeof(graphType));

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
	printf("\n G의 인접리스트 ");
	print_adjList(G);

	printf("\n\n//////////////\n\n너비우선탐색 >> ");
	bfs(G,0);
	getchar();

	exit(EXIT_SUCCESS);
}
