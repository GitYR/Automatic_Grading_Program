#include "./score.h"

void grading_ready(Optind opt)
{
	int i,j;
	FILE * fp;
	FILE * point;
	char choice = '0'; // score_table을 위한 변수

	/* 선언한 변수들의 초기화 작업 */

	control = FALSE;
	count_std = count_ans = 0;
	memset((char *)fname_std,0,NAME_SIZE);
	memset((char *)fname_ans,0,NAME_SIZE);
	memset((char *)csv_name,0,NAME_SIZE+20);
	memset((char *)point_name,0,NAME_SIZE+20);

	sprintf(fname_std,"./%s",store[1]); // 학생 디렉토리 경로 이름 복사
	sprintf(fname_ans,"./%s",store[2]); // 답안 디렉토리 경로 이름 복사
	sprintf(csv_name,"%s/score_table.csv", fname_ans); // 점수 테이블 경로 이름 복사
	sprintf(point_name,"./score.csv"); // 채점 테이블 경로 이름 복사

	if((fp = fopen(csv_name,"r")) == NULL) // csv 파일이 없을 때
	{
		fprintf(stderr,"score_table.csv file doesn't exist in %s directory!\n", fname_ans);
		printf("1. input blank question and program's score ex) 0.5 1\n"
			   "2. input all question's score ex) Input value of 1-1 : 0.1\n"
			   "select type >> ");
		scanf("%c", &choice);
	}
	else
	{
		control = TRUE;
		fclose(fp);
	}

	/* scan a directory : 이름 순 정렬 */

	if((count_std = scandir(fname_std,&flist_std,0,versionsort)) < 0) // 학생 디렉토리 : count_std - 2 = 학번의 갯수
	{
		fprintf(stderr,"scandir error for %s\n", fname_std);
		exit(1);
	}

	if((count_ans = scandir(fname_ans,&flist_ans,0,versionsort)) < 0) // 답안 디렉토리 - .csv 파일 2개 + . + ..
	{
		fprintf(stderr,"scandir error for %s\n", fname_ans);
		exit(1);
	}
	else
	{
		if(choice == '1') // 점수 테이블 만드는 방식
			score_table(choice);
		else if(choice == '2')
			score_table(choice);
	}
	FILE * fp_score;

	if((fp_score = fopen(csv_name,"r")) == NULL)
	{
		fprintf(stderr,"fopen error for %s\n", csv_name);
		exit(1);
	}
	else
	{
		double score;
		char temp[NAME_SIZE*4];
		memset((char *)temp,0,NAME_SIZE*4);

		for(i = 0; i < count_std-2; i++)
		{
			fseek(fp_score,0,SEEK_SET);
			memset((char *)temp,0,NAME_SIZE*4);
			strncpy(student[i].name,flist_std[i+2]->d_name,strlen(flist_std[i+2]->d_name));
		
			for(j = 0; j < count_ans-2; j++)
			{
				fscanf(fp_score,"%s", temp);
				fscanf(fp_score,"\t");
				fscanf(fp_score,"%lf", &score);
				fscanf(fp_score,"\n");
				
				if(strstr(flist_ans[j+2]->d_name,".csv") != NULL)
					continue;

				strncpy(student[i].problem[j].ans,flist_ans[j+2]->d_name,strlen(flist_ans[j+2]->d_name));
			
				if(strstr(flist_ans[j+2]->d_name,"-") != NULL)
				{
					sprintf(student[i].problem[j].ans,"%s.txt",student[i].problem[j].ans);
					if(!strncmp(temp,student[i].problem[j].ans,strlen(temp)))
						student[i].problem[j].score = score;
				}
				else
				{
					DIR * dir;
					char path[NAME_SIZE*4];

					memset((char *)path,0,NAME_SIZE*4);
					sprintf(path,"%s/", fname_ans); // ./TRUESET
					sprintf(path,"%s%s", path, flist_ans[j+2]->d_name); // ./TRUESET/XX

					if((dir = opendir(path)) == NULL)
					{
						fprintf(stderr,"opendir error for %s\n", path);
						exit(1);
					}
				
					struct dirent * dir_ent = readdir(dir);

					while(1)
					{
						if(strstr(dir_ent->d_name,".txt") != NULL || strstr(dir_ent->d_name,".c") != NULL)
							break;
						else
							dir_ent = readdir(dir);
					}
					if(strstr(dir_ent->d_name,".c") != NULL)
						sprintf(student[i].problem[j].ans,"%s.c",student[i].problem[j].ans);
					else
						sprintf(student[i].problem[j].ans,"%s.txt",student[i].problem[j].ans);
					
					if(!strncmp(temp,student[i].problem[j].ans,strlen(temp)))
						student[i].problem[j].score = score;
					closedir(dir);
				}
			}
		}
		fclose(fp_score);
	}
	
	if((fp = fopen(csv_name,"r")) == NULL) // 여기까지 왔다면 점수 파일은 있어야함
	{
		fprintf(stderr,"fopen error for %s\n", csv_name); // 그래도 예외처리
		exit(1);
	}
	else
	{
		grading_student(opt); // 채점 시작
		make_point_file();
		fclose(fp);
	}
}

void make_point_file(void)
{
	int i;
	FILE * point_csv;
	char problem[NAME_SIZE*8];

	if((point_csv = fopen(point_name,"w")) == NULL)
	{
		fprintf(stderr,"fopen error for %s\n", point_name);
		exit(1);
	}

	i = 2;
	fprintf(point_csv,"\t");
	while(i < count_ans)
	{
		memset((char *)problem,0,NAME_SIZE*4);

		if(strstr(flist_ans[i]->d_name,".csv") != NULL)
		{
			i++;
			continue;
		}

		if(strstr(flist_ans[i]->d_name,"-") != NULL)
			sprintf(problem,"%s.txt",flist_ans[i]->d_name);
		else
		{
			DIR * dir;
			char path[NAME_SIZE*4];

			memset((char *)path,0,NAME_SIZE*4);
			sprintf(path,"%s/", fname_ans); // ./TRUESET
			sprintf(path,"%s%s", path, flist_ans[i]->d_name); // ./TRUESET/XX

			if((dir = opendir(path)) == NULL)
			{
				fprintf(stderr,"opendir error for %s\n", path);
				exit(1);
			}
		
			struct dirent * dir_ent = readdir(dir);

			while(1)
			{
				if(strstr(dir_ent->d_name,".txt") != NULL || strstr(dir_ent->d_name,".c") != NULL)
					break;
				else
					dir_ent = readdir(dir);
			}
			if(strstr(dir_ent -> d_name,".txt") != NULL)
				sprintf(problem,"%s.txt",flist_ans[i]->d_name);
			else
				sprintf(problem,"%s.c",flist_ans[i]->d_name);

		}

		fprintf(point_csv,"%s\t", problem);
		i++;
	}
	fprintf(point_csv,"sum\n");

	i = 0;
	while(i < count_std - 2)
	{
		int j = 0;
		double sum = 0;

		while(*student[i].problem[j].ans != '\0')
		{
			sum += student[i].problem[j].score;
			j++;
		}
		student[i].sum = sum;
		i++;
	}
	if(control == TRUE)
		count_ans--;

	i = 2;
	while(i < count_std)
	{
		int j = 2;
		fprintf(point_csv,"%s\t", student[i-2].name);
		while(j < count_ans)
		{
			fprintf(point_csv,"%.2lf\t", student[i-2].problem[j-2].score);
			j++;
		}
		fprintf(point_csv,"%.2lf\n", student[i-2].sum);
		i++;
	}
}

void option_help(void) // h 옵션의 경우 출력 후 종료
{
	fprintf(stdout,"Usage : ./ssu_score <STUDENTDIR> <TRUESET> [OPTION]\n"
			"Option :\n"
			"-e <DIRNAME>	 print error on 'DIRNAME/ID/qname_error.txt' file --> MUST HAVE A PARAMETER WHICH IS NAMED 'DIRNAME/ID/qname_error.txt'\n" 
			"-t <QNAMES> 	 compile QNAME.c with -lpthread option --> MUST HAVE A PARAMETER AT LEAST ONE\n"
			"-h          	 print usage\n"
			"-p          	 print student's score and total average\n"
			"-c <IDS>    	 print ID's score --> MUST HAVE A PARAMETER AT LEAST ONE\n");
	exit(0);
}

void option_c(Optind opt)
{
	int j;
	double score;
	FILE * fp_score;
	char * fname = "./score.csv";
	
	if((fp_score = fopen(fname,"r")) == NULL)
	{
		fprintf(stderr,"There is no score.csv\n");
		exit(1);
	}
	else
	{
		int i = 0;
		int index = opt.idx_c;
		int count_a = 0,count_s = 0;
		char temp[NAME_SIZE*4];
		char before[NAME_SIZE*4];

		memset((char *)temp,0,NAME_SIZE*4);
		memset((char *)before,0,NAME_SIZE*4);

		while(strstr(temp,"sum") == NULL)
		{
			fscanf(fp_score,"%s\t", temp);
			count_a++;
		}
		fscanf(fp_score,"\n");
		fscanf(fp_score,"%s\t", temp);
		while(strcmp(temp,before))
		{
			sprintf(before,"%s", temp);
			loop(fp_score,count_a);
			fscanf(fp_score,"%s", temp);
			count_s++;
		}
		fseek(fp_score,0,SEEK_SET);
		j = 0;
		while(index < opt.idx_c + 5 && index < count_arg && strstr(store[index],"-") == NULL)
		{
			bool flag = FALSE;
			memset((char *)temp,0,NAME_SIZE*4);
			while(strcmp(store[index],temp) != 0)
			{
				if(j > count_s)
				{
					flag = TRUE;
					break;
				}
				memset((char *)temp,0,NAME_SIZE*4);
				loop(fp_score,count_a);
				fscanf(fp_score,"%s\t", temp);
				j++;
			}
			i = 0;
			j = 0;
			if(!flag)
			{
				while(i < count_a)
				{
					fscanf(fp_score,"%lf\t", &score);
					i++;
				}
				printf("%s's score : %.2lf\n", store[index], score);
				fseek(fp_score,0,SEEK_SET);
			}
			else
				printf("%s is an invalid id\n", store[index]);
			index++;
		}
	}
}

void loop(FILE * fp_score,int count)
{
	int i = 0;
	char temp[NAME_SIZE*4];

	memset((char *)temp,0,NAME_SIZE*4);

	while(i < count)
	{
		memset((char *)temp,0,NAME_SIZE*4);
		fscanf(fp_score,"%s\t", temp);
		i++;
	}
	fscanf(fp_score,"\n");
}

void score_table(char check)
{
	FILE * fp;

	if((fp = fopen(csv_name,"w")) == NULL) // 파일 열기
	{
		fprintf(stderr,"open error for %s\n", csv_name);
		exit(1);
	}
	else
	{
		int i;
		DIR * quest; // 답안 디렉토리에 접근하기위한 변수
		char temp[NAME_SIZE*4]; // 문자열 처리용
		double blank,program; // 점수 입력받기 위한 변수

		memset((char *)temp,0,NAME_SIZE*4); // temp 초기화

		if(check == '1') // 빈칸 & 프로그램 문제 점수 일괄 입력
		{
			printf("Input value of blank question : "); 
			scanf("%lf", &blank);
			printf("Input value of program question : "); 
			scanf("%lf", &program);
		}

		for(i = 2; i < count_ans; i++)
		{
			sprintf(temp,"%s/",fname_ans);
			sprintf(temp,"%s%s",temp,flist_ans[i]->d_name); // 디렉토리 이름 복사 붙여넣기

			if(strstr(temp,".csv") != NULL) // 점수파일까지 디렉토리로 열면 안되므로
				continue;

			if((quest = opendir(temp)) == NULL)
			{
				fprintf(stderr,"opendir error for %s\n", temp);
				exit(1);
			}
			else
			{
				int j;
				struct dirent * dir_member = readdir(quest);
			
				while(1)
				{
					if(!strncmp(dir_member->d_name,".",1) || !strncmp(dir_member->d_name,"..",2)) // 현재 디레토리와 부모 디렉토리 거르는 용도
						dir_member = readdir(quest);
					else if(strstr(dir_member->d_name,".exe") != NULL || strstr(dir_member->d_name,".stdout") != NULL)
						dir_member = readdir(quest);
					else
						break;
				}

				memset((char *)temp,0,NAME_SIZE*4);
				sprintf(temp,"%s",dir_member->d_name);
				
				if(check == '1') // 위 주석 참고
				{
					if(strstr(temp,".txt") != NULL) // temp는 빈칸 문제
						fprintf(fp,"%s\t%-.2lf\n",temp,blank);
					else if(strstr(temp,".c") != NULL) // temp는 프로그램 문제
						fprintf(fp,"%s\t%-.2lf\n",temp,program);
				}
				if(check == '2') // 문제마다 점수 입력하기
				{
					sprintf(temp,"%s",dir_member->d_name);

					printf("Input of %s: ",temp);
					scanf("%lf", &blank);
					fprintf(fp,"%s\t%-.2lf\n",temp,blank);
				}
			}
			memset((char *)temp,0,NAME_SIZE);
		}
		fclose(fp);
	}
}

void ssu_runtime(struct timeval * begin,struct timeval * end)
{
	end -> tv_sec -= begin -> tv_sec; // 먼저 초끼리 뺄셈

	if(end -> tv_usec < begin -> tv_usec) // 종료 마이크로 초가 시작 마이크로 초보다 작다면
	{
		end -> tv_sec--; // 뺄셈할 때 수가 작으면 앞 자리 수 가져오는 것처럼
		end -> tv_usec += SECOND_TO_MICRO; // 마이크로초에 더해주기
	}

	end -> tv_usec -= begin -> tv_usec; // 마이크로 초끼리 뺄셈
	printf("RunTime : %ld:%06ld(sec:usec)\n", end -> tv_sec, end -> tv_usec);
}

void maximum_arguments(Optind opt) // 가변인자 수가 5개 넘는 옵션 처리용
{
	int i;
	int count = 1;

	if(opt.idx_e) // e option
	{
		i = opt.idx_e;

		while(i < count_arg && strstr(store[i],"-") == NULL)
		{
			if(count > 1)
				printf("Maximum Number of Argument Exceeded. :: %s\n", store[i]);
			i++;
			count++;
		}
	}

	count = 1;

	if(opt.idx_t) // t option
	{
		i = opt.idx_t;

		while(i < count_arg && strstr(store[i],"-") == NULL)
		{
			if(count > 5)
				printf("Maximum Number of Argument Exceeded. :: %s\n", store[i]);
			i++;
			count++;
		}
	}

	count = 1;

	if(opt.idx_c) // c option
	{
		i = opt.idx_c;

		while(i < count_arg && strstr(store[i],"-") == NULL)
		{
			if(count > 5)
				printf("Maximum Number of Argument Exceeded. :: %s\n", store[i]);
			i++;
			count++;
		}
	}
}


