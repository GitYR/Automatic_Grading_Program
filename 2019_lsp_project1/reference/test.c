#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define EXTENDED_STRLEN 1024
#define DATATYPE_SIZE 35
#define MINLEN 64

char datatype[DATATYPE_SIZE][MINLEN] = {"int", "char", "double", "float", "long"
			, "short", "ushort", "FILE", "DIR","pid"
			,"key_t", "ssize_t", "mode_t", "ino_t", "dev_t"
			, "nlink_t", "uid_t", "gid_t", "time_t", "blksize_t"
			, "blkcnt_t", "pid_t", "pthread_mutex_t", "pthread_cond_t", "pthread_t"
			, "void", "size_t", "unsigned", "sigset_t", "sigjmp_buf"
			, "rlim_t", "jmp_buf", "sig_atomic_t", "clock_t", "struct"};

void remove_space(char * buf)
{
	int i = 0,j,len;

	len = strlen(buf);

	while(i < len && buf[i] != '\0')
	{
		if(buf[i] == ' ')
		{
			for(j = i; buf[j] != '\0'; j++)
				buf[j] = buf[j+1];
			continue;
		}
		i++;
	}
}

int is_typeStatement(char *str)
{ 
	char *start;
	char str2[EXTENDED_STRLEN] = {0}; 
	char tmp[EXTENDED_STRLEN] = {0}; 
	char tmp2[EXTENDED_STRLEN] = {0}; 
	int i;	 
	
	start = str;
	strncpy(str2,str,strlen(str)); // str2에 str 복사
	remove_space(str2); // 공백 제거

	while(start[0] == ' ') // 공백문자 무시
		start += 1;

	if(strstr(str2, "gcc") != NULL) // gcc가 포함되어 있다면
	{
		strncpy(tmp2, start, strlen("gcc"));
		if(strcmp(tmp2,"gcc")) // 시작단어가 gcc가 아니라면
			return 0;
		else // 맞다면
			return 2;
	}
	
	for(i = 0; i < DATATYPE_SIZE; i++)
	{
		if(strstr(str2,datatype[i]) != NULL) // 특정data type 이 존재한다면
		{	
			strncpy(tmp, str2, strlen(datatype[i])); 
			strncpy(tmp2, start, strlen(datatype[i])); 
			
			if(!strcmp(tmp, datatype[i])) // str이 특정 자료형으로 시작한다면
				if(strcmp(tmp, tmp2)) // 
					return 0;  
				else // 
					return 2;
		}
	}
	return 1;
}

int main(void)
{
	int ret;
	char str1[64];
	char str2[64];
	char str3[64];

	memset((char *)str1,0,64);
	memset((char *)str2,0,64);
	memset((char *)str3,0,64);

	strcpy(str1,"int a = 6;");
	strcpy(str2,"  memset((char *)str3,0,64);");
	strcpy(str3,"char * test_function(void)");

	ret = is_typeStatement(str1);
	if(ret)
	{
		printf("ret : %d\n", ret); // 2
		printf("str1 is type statement!\n");
	}
	ret = is_typeStatement(str2);
	if(ret)
	{
		printf("ret : %d\n", ret); // 1
		printf("str2 is type statement!\n");
	}
	ret = is_typeStatement(str3);
	if(ret)
	{
		printf("ret : %d\n", ret);
		printf("str3 is type statement!\n");
	}

	exit(EXIT_SUCCESS);
}
