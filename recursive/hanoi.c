#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void hanoi(char from,char mid,char to,int cnt)
{
	if(cnt <= 1)
	{
		printf("%c의 원판 %d을 -> %c\n",from,cnt,to);
		return;
	}
	else
	{
		hanoi(from,to,mid,cnt-1);
		printf("%c의 원판 %d을 -> %c\n",from,cnt,to);
		hanoi(mid,from,to,cnt-1);
	}
}

int main(void)
{
	char a = 'A';
	char b = 'B';
	char c = 'C';
	int cnt;

	printf("원판의 개수를 입력하시오 : ");
	scanf("%d", &cnt);

	hanoi(a,b,c,cnt);

	exit(EXIT_SUCCESS);
}
