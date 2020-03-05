#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0
#define INF 987654321

typedef _Bool bool;

int v,e,s;
bool flag[1001];
int dist[1001];
int data[1001][1001]; // weights

int main(void)
{
	int i,j,min;
	int bk,cnt,end;
	int tmp1,tmp2,tmp3;

	scanf("%d", &v); 
	scanf("%d", &e); 

	memset((int **)data,0,sizeof(data)); // initializing

	for(i = 1; i <= e; i++)
	{
		scanf("%d%d%d", &tmp1, &tmp2, &tmp3);
		data[tmp1][tmp2] = tmp3;
	}

	scanf("%d%d", &s, &end); // starting point , ending point

	/*
	printf("[TEST] v : %d, e : %d, start : %d, end : %d\n", v,e,s,end);

	for(i = 0; i <= v; i++)
	{
		printf("[TEST] ");
		for(j = 0; j <= v; j++)
			printf("data[%d][%d] : %d ",i,j,data[i][j]);
		printf("\n");
	}
	*/

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

	printf("%d\n", dist[end]);
	return 0;
}
