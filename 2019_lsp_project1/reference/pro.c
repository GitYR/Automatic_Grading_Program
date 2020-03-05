#include "./score.h"

int main(int argc,char * argv[])
{
	int i,j;
	int opt; // 옵션 처리용
	bool check = FALSE;
	DIR * dir_std,* dir_ans;
	bool opt_h,opt_c,opt_t,opt_p,opt_e; // 옵션이 설정 여부 확인
	Optind oidx; // 가변인자 개수 저장
	struct timeval begin,end; // 시간 측정용

	gettimeofday(&begin,NULL); // 시간 측정 시작!
	count_arg = argc;
	store = (char **)malloc(sizeof(char *) * argc);

	for(i = 0; i < count_arg; i++)
		store[i] = (char *)malloc(sizeof(char) * (strlen(argv[i])+1));
	oidx.idx_e = oidx.idx_t = oidx.idx_c = oidx.idx_p = 0; // 옵션 인덱스 수 초기화
	opt_h = opt_c = opt_t = opt_p = opt_e = FALSE;
	opterr = 0; // option error 출력 off

	for(i = 0; i < argc; i++)
		strncpy(store[i],argv[i],strlen(argv[i]));

	for(i = 0; i < NUM; i++)
	{
		memset((char *)student[i].name,0,NAME_SIZE);
		for(j = 0; j < NUM; j++)
			memset((char *)student[i].problem[j].ans,0,NAME_SIZE);
	}

	/* 예외 처리 */

	if(argc <= 2)
		check = TRUE;
	else
	{
		if((dir_std = opendir(store[1])) == NULL || (dir_ans = opendir(store[2])) == NULL) // argv 1과 2값이 실제로 있는 디렉토리인가?
		{
			check = TRUE; // 아니면 check = TRUE

			if(strcmp(store[1],"-c"))
			{
				fprintf(stderr,"<%s OR %s> DIRECTORY ARE NOT FOUNDED.\n", store[1], store[2]);
				exit(1);
			}
		}
	}

	/* 옵션 처리 */

	while((opt = getopt(argc,argv,"hpc:t:e:")) != EOF)
	{
		switch(opt) 
		{
			case 'h' :
				opt_h = TRUE;
				break;
			case 'c' : // 가변 인자가 존재함
				opt_c = TRUE;
				oidx.idx_c = optind-1;
				break;
			case 't' : // 가변 인자가 존재함
				opt_t = TRUE;
				oidx.idx_t = optind-1;
				break;
			case 'p' :
				opt_p = TRUE;
				oidx.idx_p = TRUE;
				break;
			case 'e' : // 가변 인자가 존재함
				opt_e = TRUE;
				oidx.idx_e = optind-1;
				break;
			case '?' :
				fprintf(stderr,"잘 못 입력하셨습니다!\n");
				option_help();
				exit(1);
		}
	}

	maximum_arguments(oidx);

	if(opt_h) 
	{
		for(i = 0; i < count_arg; i++)
			free(store[i]);
		free(store);
		option_help();
	}

	if(check) // check = TRUE
	{
		if(opt_c) // c 옵션에 해당하는 결과를 출력하고 끝내야함.
			option_c(oidx);
		else 
			option_help();
	}
	else // check = FALSE 
	{
		grading_ready(oidx); // directory 체크
		if(opt_c)
			option_c(oidx); // c option
	}
	
	gettimeofday(&end,NULL); // 시간 측정 끝
	printf("\n");
	ssu_runtime(&begin,&end);
	printf("\n");

	return 0;
}
