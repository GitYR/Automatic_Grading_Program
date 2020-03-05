#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0
#define INF 1000000

typedef _Bool bool;

typedef struct _node
{
	bool isVisited;
	int v;
	struct _node * next;
}Node;

int v,e,s;
bool flag[20001];
short dist[20001];
char data[20001][20001]; // weights

int main(void)
{
	int i,j,min;
	int bk,cnt;
	int tmp1,tmp2,tmp3;

	scanf("%d%d", &v, &e); // the numbers of vertexes and edges
	scanf("%d", &s); // starting point (1 <= s <= v)

	memset((char **)data,0,sizeof(data)); // initializing

	for(i = 1; i <= e; i++)
	{
		scanf("%d%d%d", &tmp1, &tmp2, &tmp3);
		data[tmp1][tmp2] = tmp3;
	}

	for(i = 0; i <= v; i++)
	{
		flag[i] = FALSE;
		dist[i] = INF;
	}

	dist[s] = 0;

	cnt = v;
	i = s;
	while(cnt > 0)
	{
		min = INF;
		for(j = 1; j <= v; j++)
		{
			if(min > dist[i] + data[i][j] && !flag[j] && data[i][j] != 0)
			{
				min = dist[i] + data[i][j];
				bk = i;
			}
		}

		flag[bk] = TRUE;

		for(j = 1; j <= v; j++)
		{
			if(dist[j] > data[bk][j] + dist[bk] && data[bk][j] != 0)
				dist[j] = data[bk][j] + dist[bk];
		}
		i++;
		i = (i % v) + 1;
		cnt--;
	}

	for(i = 1; i <= v; i++)
	{
		if(dist[i] != INF)
			printf("%d\n", dist[i]);
		else
			printf("INF\n");
	}
	return 0;
}
