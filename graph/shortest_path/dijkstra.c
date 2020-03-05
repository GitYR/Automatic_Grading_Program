#include <stdio.h>
#include <stdlib.h>

#define n 6
#define M 0x7FFFFFFF

int flag[n+1],dist[n+1];
int i,j,min,bk; // bk : 돌아올 위치
int data[n+1][n+1] = 
{
	{0,0,0,0,0,0,0},
	{0,0,2,M,3,M,M},
	{0,M,0,4,1,M,M},
	{0,M,M,0,4,1,3},
	{0,M,2,2,0,1,M},
	{0,M,M,1,M,0,6},
	{0,M,M,M,M,M,0}
}; // M으로 설정되어 있다는 것은 끊어져있음을 의미함

int main(void)
{
	FILE * fp = fopen("dist.txt","w+");
	if(fp == NULL)
	{
		fprintf(stderr,"fopen() error!\n");
		exit(EXIT_FAILURE);
	}
	fprintf(fp,"%d",n);
	fprintf(fp,"\n");

	for(i = 0; i <= n; i++)
	{
		for(j = 0; j <= n; j++)
		{
			fprintf(fp,"%d ", data[i][j]);
		}
		fprintf(fp,"\n");
	}

	fclose(fp);

	for(i = 1; i <= n; i++)
	{
		flag[i] = 0;
		dist[i] = M; 
	}

	dist[1] = 0; // 1번 점부터 다른 어떤 점까지의 거리, 0으로 초기화

	for(i = 1; i <= n; i++)
	{
		min = M; // 일단 무한대로 초기화
		for(j = 1; j <= n; j++)
		{
			if(min > dist[i] && flag[j] == 0) // j번째 node를 방문한적 없으며, 거리값이 최대값보다 작다면
			{
				min = dist[i] + data[i][j];
				bk = j; // 현재 위치 백업
			}
		}

		flag[bk] = 1;

		for(j = 1; j <= n; j++)
			if(dist[j] > data[bk][j] + dist[bk] && data[bk][j] != M) // data[a][b] : a->b의 가중치, M이라면 끊어져 있는 상태임
				dist[j] = data[bk][j] + dist[bk]; // 더 작은 값으로 교체
	}

	for(i = 1; i <= n; i++)
		printf("1 ~ %d : %d\n", i,dist[i]); // 1부터 각 정점까지의 최소 거리
	exit(EXIT_SUCCESS);
}
