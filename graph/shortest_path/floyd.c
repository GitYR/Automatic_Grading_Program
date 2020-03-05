#include <stdio.h>
#include <stdlib.h>

#define INF 0x7FFFFFFF

int n; // the number of vertexes
int **data;

void printDist(void)
{
	int i,j;
	char ch = ' ';
	printf("%15c", ch);
	for(i = 0; i < n; i++)
		printf("%15c", 'A'+i);
	printf("\n");
	for(i = 1; i < n+1; i++)
	{
		printf("%15c", 'A'+(i-1));
		for(j = 1; j < n+1; j++)
			printf("%15d", data[i][j]);
		printf("\n");
	}
	printf("\n");
}

void floyd(void)
{
	int i,j,k;

	for(k = 1; k < n+1; k++)
	{
//		printDist();
		for(i = 1; i < n+1; i++)
		{
			for(j = 1; j < n+1; j++)
			{
				if((data[i][k] * data[k][j]) != 0 && (i != j))
				{
					if(data[i][k] != INF && data[k][j] != INF)
						if((data[i][k] + data[k][j]) < data[i][j] || data[i][j] == 0)
							data[i][j] = data[i][k] + data[k][j];
				}
			}
		}
	}
	printDist();
}

int main(int argc,char *argv[])
{
	FILE * fp;
	int i,j,k;

	if(argc < 2)
	{
		fprintf(stderr,"Usage : %s [FILE_NAME]\n",argv[0]);
		exit(EXIT_FAILURE);
	}
	if((fp = fopen(argv[1],"r")) == NULL)
	{
		fprintf(stderr,"fopen() error\n");
		exit(EXIT_FAILURE);
	}

	fscanf(fp,"%d", &n);
	fgetc(fp);

	//printf("[TEST] n : %d\n", n);
	data = (int **)malloc(sizeof(int *)*(n+1));
	for(i = 0; i <= n; i++)
		data[i] = (int *)malloc(sizeof(int)*(n+1));

	for(i = 0; i <= n; i++)
	{
		for(j = 0; j <= n; j++)
		{
			fscanf(fp,"%d", &data[i][j]);
			fgetc(fp);
		}
	}

	floyd();

	/*
	for(i = 0; i <= n; i++)
	{
		for(j = 0; j <= n; j++)
			printf("data[%d][%d] : %-10d ",i,j,data[i][j]);
		printf("\n");
	}
	*/
	for(i = 0; i <= n; i++)
		free(data[i]);
	free(data);

	fclose(fp);
	exit(EXIT_SUCCESS);
}
