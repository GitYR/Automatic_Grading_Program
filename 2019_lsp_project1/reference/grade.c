#include "./score.h"

void grading_student(Optind opt)
{
	int a,i,j;
	int file_check;
	DIR * dir_std, * dir_ans; // 각각 학생 그리고 답안 디렉토리를 위한 변수
	char fname[NAME_SIZE*4];
	char path[NAME_SIZE*4];

	printf("grading score...\n");
	student_compile(opt); // 학생 컴파일 ..
	
	for(i = 2; i < count_ans; i++) // 답안 디렉토리 기준으로
	{
		memset((char *)fname,0,NAME_SIZE*4);
		memset((char *)path,0,NAME_SIZE*4);
		sprintf(fname,"%s/", fname_ans); // ./TRUESET
		sprintf(fname,"%s%s",fname,flist_ans[i]->d_name); // ./TRUESET/XX
		sprintf(path,"%s/",fname); // ./TRUESET/XX/

		if(strstr(fname,".csv") != NULL) // 점수 파일은 제외
			continue;

		if((dir_ans = opendir(fname)) == NULL) // 답안 디렉토리 안의 문제 디렉토리 열고
		{
			fprintf(stderr,"opendir error for %s\n", fname);
			exit(1);
		}
		else // 열리면
		{
			struct dirent * dir_member = readdir(dir_ans); // 열린 디렉토리 읽기

			while(1) // 현재 디렉토리와 부모 디렉토리 거르기
			{
				if(strstr(dir_member->d_name,".c") != NULL || strstr(dir_member->d_name,".txt") != NULL)
					break;
				else
					dir_member = readdir(dir_ans);
			}

			sprintf(fname,"%s/",fname);
			sprintf(fname,"%s%s",fname,dir_member->d_name); // ./TRUESET/XX/XX.txt or .c

			if(strstr(fname,".txt") != NULL) // 빈칸 문제
			{
				/* path :  ./TRUESET/XX/       */
				/* fname : ./TRUESET/XX/XX.txt*/
				blank_problem(path,fname,opt);
			}
			else if(strstr(fname,".c") != NULL) // 프로그램 문제
			{
				program_problem(path,fname,opt); // 문제하나씩 컴파일
				memset((char *)fname,0,NAME_SIZE*4);
				sprintf(fname,"%s", dir_member -> d_name);
				student_execute(fname,opt);
			}
		}
	}
	// .c 답안 비교
	
	i = 2;
	while(i < count_std) // 학생 폴더 검사
	{
		int count_id,count,std,ans;
		char problem[NAME_SIZE*4];
		char std_out[NAME_SIZE*4];
		char ans_out[NAME_SIZE*4];
		char std_path[NAME_SIZE*4];
		char ans_path[NAME_SIZE*4];
		struct dirent ** flist;
		struct dirent ** flist_ans_id;

		memset((char *)ans_out,0,NAME_SIZE*4);
		memset((char *)std_out,0,NAME_SIZE*4);
		memset((char *)ans_path,0,NAME_SIZE*4);
		memset((char *)std_path,0,NAME_SIZE*4);

		sprintf(std_path,"%s/", fname_std); // std_path : ./STUDENT/
		sprintf(std_path,"%s%s", std_path, flist_std[i]->d_name); // std_path : ./STUDENT/201900XX

		if((count_id = scandir(std_path,&flist,0,versionsort)) < 0) // ../201900XX의 파일 정렬 & 읽기
		{
			fprintf(stderr,"scandir error for %s\n", std_path);
			exit(1);
		}
		else
		{
			j = 2;
			while(j < count_ans) // 정답 파일 읽기
			{
				if(strstr(flist_ans[j]->d_name,".csv") != NULL) // .txt파일 + 채점파일은 거르자
				{
					j++;
					continue;
				}
				else
				{
					int isFile; // 파일이 있는가? 체크하는 변수
					char ans_buf[FILESIZE];
					char std_buf[FILESIZE];
					char moonjae[NAME_SIZE*4];

					memset((char *)problem,0,NAME_SIZE*4);
					memset((char *)ans_path,0,NAME_SIZE*4);
					memset((char *)std_path,0,NAME_SIZE*4);
					memset((char *)moonjae,0,NAME_SIZE*4);

					sprintf(ans_path,"%s/", fname_ans); // ./TRUESET/
					sprintf(ans_path,"%s%s", ans_path, flist_ans[j]->d_name); // ./TRUESET/XX

					if((count = scandir(ans_path,&flist_ans_id,0,versionsort)) < 0) // ./TRUESET/XX 폴더 열기 + 정렬
					{
						fprintf(stderr,"scandir error for %s\n", ans_path);
						exit(1);
					}
					else
					{
						int k = 2;

						while(k < count)
						{
							if(strstr(flist_ans_id[k]->d_name,".stdout") != NULL)
							{
								memset((char *)ans_path,0,NAME_SIZE*4);
								sprintf(problem,"%s", flist_ans_id[k]->d_name); // XX.stdout
								sprintf(ans_path,"%s/", fname_ans); // ./TRUESET/
								sprintf(ans_path,"%s%s", ans_path, flist_ans[j]->d_name);
								sprintf(ans_path,"%s/", ans_path);
								sprintf(ans_path,"%s%s", ans_path, flist_ans_id[k]->d_name); // ans_path : ./TRUESET/XX/XX.stdout
		
								sprintf(std_path,"%s/", fname_std); // ./STUDENT/
								sprintf(std_path,"%s%s", std_path, flist_std[i]->d_name); // ./STUDET/201900XX
								sprintf(std_path,"%s/", std_path); // ./STUDENT/201900XX/
								sprintf(std_path,"%s%s", std_path, problem); // ./STUDENT/201900XX/XX.stdout
								strncpy(moonjae,problem,strlen(problem)-7);
								sprintf(moonjae,"%s.c", moonjae); // moonjae = XX.c
		
								isFile = access(std_path,0);
		
								if(isFile == 0) // file 존재
								{
									int ans_len,std_len;
									bool correct = FALSE;

									if((ans = open(ans_path,O_RDONLY)) < 0)
									{
										fprintf(stderr,"open error for %s\n", ans_path);
										exit(1);
									}

									if((std = open(std_path,O_RDONLY)) < 0)
									{
										fprintf(stderr,"open error for %s\n", std_path);
										exit(1);
									}

									while(1)
									{
										memset((char *)ans_buf,0,FILESIZE);
										memset((char *)std_buf,0,FILESIZE);
									
										ans_len = read(ans,ans_buf,FILESIZE);
										std_len = read(std,std_buf,FILESIZE);
										string_processing(ans_buf,std_buf);
										
										if(!strcmp(ans_buf,std_buf))
										{
											if(*ans_buf != '\0' && *std_buf != '\0')
												correct = TRUE;
										}
										else
										{
											correct = FALSE;
											break;
										}
										if(ans_len <= 0 && std_len <= 0)
											break;
									}
									
									if(!correct) // 틀렸음
									{
										int l = 0;

										while(l < count_ans)
										{
											if(strstr(flist_ans[l]->d_name,".csv") != NULL)
											{
												l++;
												continue;
											}
											if(!strncmp(student[i-2].problem[l].ans,moonjae,strlen(moonjae)))
											{
												student[i-2].problem[l].score = WRONG;
												break;
											}
											l++;
										}
									}
								}
							}
							k++;
						}
					}
				}
				j++;
			}
		}
		printf("%s is finished...", flist_std[i]->d_name);
		if(opt.idx_p) // p option 존재
		{
			int k = 0;
			double sum = 0;

			while(*student[i-2].problem[k].ans != '\0')
			{
				sum += student[i-2].problem[k].score;
				k++;
			}
			student[i-2].sum = sum;
			printf("\tscore : %.2lf\n", sum);
		}
		else
			printf("\n");
		i++;
	}
	if(opt.idx_p)
	{
		double sum = 0;

		for(i = 0; *student[i].name != '\0'; i++)
			sum += student[i].sum;
		sum /= (count_std - 2);
		printf("Total average : %.2lf\n", sum);
	}

	/* 쓸모없는 파일 제거 */

	file_check = access("./POUT.txt",0); // 파일 체크
	if(file_check == 0)
		remove("./POUT.txt");

	file_check = access("./PERR.txt",0);
	if(file_check == 0)
		remove("./PERR.txt");

	file_check = access("./temp.txt",0);
	if(file_check == 0)
		remove("./temp.txt");
	
	file_check = access("./grade.o",0);
	if(file_check == 0)
		remove("./grade.o");

	file_check = access("./score.o",0);
	if(file_check == 0)
		remove("./score.o");

	file_check = access("./pro.o",0);
	if(file_check == 0)
		remove("./pro.o");
	
	file_check = access("./pro.o",0);
	if(file_check == 0)
		remove("./stack.o");
}

void string_processing(char * ans_buf,char * std_buf)
{
	int i;
	char * apars, * spars;
	char abuf[FILESIZE];
	char sbuf[FILESIZE];

	memset((char *)abuf,0,FILESIZE);
	memset((char *)sbuf,0,FILESIZE);

	if(*ans_buf != '\0' && *std_buf != '\0') // 버퍼가 비어있을 때 Seg가 생기므로
	{
		/* 공백 제거 */
	
		apars = strtok(ans_buf," ");
		strcat(abuf,apars);
		while(apars = strtok(NULL," "))
			strcat(abuf,apars);
	
		spars = strtok(std_buf," ");
		strcat(sbuf,spars);
		while(spars = strtok(NULL," "))
			strcat(sbuf,spars);
	
		/* 대소문자 구분 제거 */

		i = 0;
		while(i < strlen(abuf))
		{
			if(abuf[i] >= 'A' && abuf[i] <= 'Z')
				abuf[i] += 32;
			i++;
		}
	
		i = 0;
		while(i < strlen(sbuf))
		{
			if(sbuf[i] >= 'A' && sbuf[i] <= 'Z')
				sbuf[i] += 32;
			i++;
		}
	
		/* 값 돌려주기 */

		memset((char *)ans_buf,0,FILESIZE);
		memset((char *)std_buf,0,FILESIZE);
		sprintf(ans_buf,"%s", abuf);
		sprintf(std_buf,"%s", sbuf);
	}
}

void blank_problem(char * path,char * fname,Optind opt) // 빈칸문제
{
	// path : ./TRUESET/XX/
    // fname : ./TRUESET/XX/XX.txt

	int i,j;
	int len_path;
	char blank[NAME_SIZE*4]; // XX.txt
	char std_path[NAME_SIZE*4]; // 학생 txt파일 경로
	int ans, std; // 답안 파일과 학생 파일 열기 위해

	memset((char *)blank,0,NAME_SIZE*4);
	len_path = strlen(path);

	i = len_path;
	while(i < strlen(fname))
	{
		blank[i-len_path] = fname[i];
		i++;
	} // XX.txt


	for(i = 2; i < count_std; i++)
	{
		int index,result;
		char * apars[16]; // ':'로 파싱
		char std_buf[BUFFER_SIZE*8];
		char ans_buf[BUFFER_SIZE*32];

		memset((char *)std_path,0,NAME_SIZE*4);
		memset((char *)std_buf,0,BUFFER_SIZE*8);
		memset((char *)ans_buf,0,BUFFER_SIZE*32);
		
		sprintf(std_path,"%s/", fname_std); // ./STUDENT/
		sprintf(std_path,"%s%s", std_path, flist_std[i]->d_name); // ./STUDENT/2019XXXX
		sprintf(std_path,"%s/", std_path); // ./STUDENT/2019XXXX/
		sprintf(std_path,"%s%s", std_path, blank); // ./STUDENT/2019XXXX/XX.txt

		if((std = open(std_path,O_RDONLY)) < 0) // 2019XXXX 학생의 XX.txt 열기
		{
			for(j = 0; j < count_ans; j++)
			{
				if(!strcmp(student[i-2].problem[j].ans,blank))
				{
					student[i-2].problem[j].score = 0;
					return;
				}
			}
		}
		if((ans = open(fname,O_RDONLY)) < 0) // ./TRUESET/XX/XX.txt 열기
		{
			fprintf(stderr,"open error for %s\n", fname);
			exit(1);
		}

		read(ans,ans_buf,BUFFER_SIZE*32); // 128 * 32 = 4096만큼 ans_buf에 읽어오기
		read(std,std_buf,BUFFER_SIZE*8);

		index = 0;
		apars[index++] = strtok(ans_buf,":"); // apars로 파싱
		while(apars[index] = strtok(NULL,":"))
			index++;
		string_clear(apars,index); // " "과 '\n' 제거

		if(*std_buf != '\0')
		{
			if(std_buf[strlen(std_buf)-1] == '\n')
				std_buf[strlen(std_buf)-1] = '\0';
		}

		for(j = 0; j < index; j++)
		{
			result = string_compare(apars[j],std_buf); // 문장 비교
			if(result)
				break;
		}
		if(!result) // 틀렸을 경우
		{
			for(j = 0; j < count_ans; j++)
			{
				if(!strcmp(student[i-2].problem[j].ans,blank))
					student[i-2].problem[j].score = 0;
			}
		}
		close(ans); // 계속 열려있으면 에러가 발생
		close(std); // ""
	}
}

bool string_compare(char * ans_buf,char * std_buf) // 문장 비교
{
	int i,j,len;

	if(!strcmp(ans_buf,std_buf)) // 맞았으면
		return TRUE;
	else
	{
		if(*std_buf == '\0') // 학생파일이 비어있는 경우는 판정할 필요가 없이
			return FALSE; // FALSE

		char temp[BUFFER_SIZE];

		/* 1. 전처리문인가? 2. 수식인가?(배정문,논리연산,계산) 3. 단순 변수 선언 및 함수 호출인가? */

		if(strstr(ans_buf,"#") != NULL)
		{
			bool check = FALSE;

			if(strstr(ans_buf,"#define") != NULL)
			{
				i = 0;
				len = strlen(ans_buf);
				while(i < len)
				{
					if(ans_buf[i] == ' ')
					{
						for(j = i; j < len; j++)
							ans_buf[j] = ans_buf[j+1];
					}
					i++;
				}
				if(strstr(std_buf,"#define") == NULL)
					return FALSE;
				i = 0;
				len = strlen(std_buf);
				while(i < len)
				{
					if(std_buf[i] == '#' &&std_buf[i+1] == 'd')
						break;
					i++;
				}
				if(std_buf[i+7] != ' ')
					return FALSE;

				i = 0;
				len = strlen(std_buf);
				while(i < len)
				{
					if(std_buf[i] == ' ')
					{
						for(j = i; j < len; j++)
							std_buf[j] = std_buf[j+1];
					}
					i++;
				}

				if(!strcmp(ans_buf,std_buf))
					return TRUE;
				else
					return FALSE;
			}

			if(strstr(ans_buf,"#include") != NULL) // 헤더 선언문이면
			{
				memset((char *)temp,0,BUFFER_SIZE);
	
				if(strstr(ans_buf,"sys") != NULL) // 시험해보니 sys/없이도 컴파일이 되는게 있음.
				{
					i = 8;
					len = strlen(ans_buf);
	
					while(i < len)
					{
						if(ans_buf[i] == '/')
						{
							i++;
							break;
						}
						i++;
					}
					for(j = i; ans_buf[j] != '>'; j++)
						temp[j-i] = ans_buf[j];
				}
	
				if(strstr(std_buf,"#include") != NULL) 
				{
					i = 0;
					len = strlen(ans_buf);
					while(i < len)
					{
						if(ans_buf[i] == ' ')
						{
							for(j = i; j < len; j++)
								ans_buf[j] = ans_buf[j+1];
						}
						i++;
					}
	
					i = 0;
					len = strlen(std_buf);
					while(i < len)
					{
						if(std_buf[i] == ' ')
						{
							for(j = i; j < len; j++)
								std_buf[j] = std_buf[j+1];
						}
						i++;
					}
					if(!strcmp(ans_buf,std_buf))
						return TRUE;
					else
					{
						if(strstr(temp,"wait.h") != NULL || strstr(temp,"time.h") != NULL)
						{
							if(strstr(std_buf,temp) != NULL)
								return TRUE;
							else
								return FALSE;
						}
						else
							return FALSE;
					}
				}
				else
					return FALSE;
			}
		}
		else if(EXPRESSION(ans_buf)) // 연산식만 따로 구분
		{
			stack_init();
			string_parsing(ans_buf,std_buf);
		}
		else
		{
			bool check = FALSE;
			char func[NAME_SIZE*4];
	
			i = 0;
			len = strlen(ans_buf);
			while(i < len) // 일단 검사
			{
				if(ans_buf[i] == ' ') // 공백 무시
				{
					i++;
					continue;
				}
				if(ISVALUE(ans_buf[i])) // 변수 조건에 부합하는가?
				{
					memset((char *)func,0,NAME_SIZE*4);

					j = i;
					while(ISVALUE(ans_buf[j]))
					{
						func[j-i] = ans_buf[j];
						j++;
					}
					if(ans_buf[j] == '(') // 이름 뒤 '('는 함수
					{
						check = TRUE;
						break;
					}
					else
						check = FALSE;
				}
				i++;
			}
			if(check)
			{
				if(strstr(std_buf,func) == NULL) // 함수명이 학생 답지에 없으면
					return FALSE;
				else
					return simple_compare(ans_buf,std_buf); // 함수명이 있으면 제대로 비교
			}
			else
				return simple_compare(ans_buf,std_buf); // 단순 선언문
		}
	}
}

void string_clear(char **pars,int index)
{
	int i,j,len;

	for(i = 0; i < index; i++)
	{
		len = strlen(pars[i]);
		char str[len+1];

		memset((char *)str,0,len+1);
		
		if(pars[i][len-1] == '\n' || pars[i][len-1] == ' ') // 문장 끝의 개행 제거
			pars[i][len-1] = '\0';
		
		if(pars[i][0] == ' ')
		{
			for(j = 1; j < len; j++)
				str[j-1] = pars[i][j];
			sprintf(pars[i],"%s", str);
		}
	}
}

bool simple_compare(char * ans_buf,char * std_buf)
{
	int i,j,index;
	int num_arg = 1;
	int k = 0;
	int len = strlen(ans_buf);

	for(i = 0; i < len; i++) // 단순 함수 빈칸문제면 공백문자가 없어도 됨
	{
		if(ans_buf[i] == ' ')
		{
			for(j = i; j < len; j++)
				ans_buf[j] = ans_buf[j+1];
		}
	}
	len = strlen(std_buf);

	for(i = 0; i < len; i++)
	{
		if(std_buf[i] == ' ')
		{
			for(j = i; j < len; j++)
				std_buf[j] = std_buf[j+1];
		}
	}
	if(!strcmp(ans_buf,std_buf))
		return TRUE;
	else
		return FALSE;
}

void program_problem(char * path,char * fname,Optind opt)
{
	char c; // 임시용
	int i,length;
	bool check = FALSE;
	double pro_score; // 문제 점수

	char buf[BUFFER_SIZE]; 
	char point_c[NAME_SIZE*4]; // .c파일 저장 경로
	char point_exe[NAME_SIZE*4]; // .exe 파일 저장 경로

	char dir_err[NAME_SIZE*4]; // error 파일 이름
	char path_std[NAME_SIZE*4]; // 학생 파일 경로
	char std_id[NAME_SIZE*4]; // 학생 학번
	char std_err[NAME_SIZE*4]; // 학생 error 파일 이름
	char std_out[NAME_SIZE*4]; // 학생 파일 실행 결과 저장
	char ans_out[NAME_SIZE*4]; // 답안 파일 실행 결과 저장
	char compile[NAME_SIZE*4]; // 컴파일 문자열
	char execute[NAME_SIZE*4]; // 실행 문자열

	DIR * dir_std;
	FILE * fp_score; // 점수 테이블
	int std_result; // 학생 실행 결과 저장
	int ans_result; // 답안 실행 결과 저장

	/* 초기화 단계 */

	memset((char *)point_c,0,NAME_SIZE*4);
	memset((char *)point_exe,0,NAME_SIZE*4);
	memset((char *)compile,0,NAME_SIZE*4);
	memset((char *)execute,0,NAME_SIZE*4);
	memset((char *)ans_out,0,NAME_SIZE*4);

	sprintf(point_c,"./");
	sprintf(point_c,"%s%s", point_c, fname_ans);
	sprintf(point_c,"%s/score_table.csv", point_c);

	/* 점수 테이블 얻기 */

	if((fp_score = fopen(point_c,"r")) == NULL)
	{
		fprintf(stderr,"fopen error for %s\n", point_c);
		exit(1);
	}

	memset((char *)point_c,0,NAME_SIZE*4);
	sprintf(point_c,"./score.csv");

	/* 정답 프로그램 먼저 컴파일 후 저장 */
	// path : ./TRUESET/XX/ : 13
	// fname : ./TRUESET/XX/XX.c : 17

	memset((char *)point_c,0,NAME_SIZE*4);
	strncpy(point_c,fname,NAME_SIZE*4); // .c파일 저장 경로
	strncpy(point_exe,fname,strlen(path)+(strlen(fname)-strlen(path)-2)); // 실행파일 저장 경로

	/* 답안 실행결과.stdout */

	sprintf(point_exe,"%s.exe",point_exe);
	strncpy(ans_out,point_c,strlen(point_c)-2); // ./TRUESET/XX/XX
	sprintf(ans_out,"%s.stdout",ans_out); // ./TRUESET/XX/XX.stdout

	sprintf(compile,"gcc -o %s %s -lpthread",point_exe, point_c);
	system(compile);

	/* 정답 프로그램의 실행 결과를 저장 */

	sprintf(execute,"./%s", point_exe);
	
	FILE * fre_ans = freopen(ans_out,"w",stdout);
	system(execute);
	fre_ans = freopen("/dev/tty","a",stdout);
}

void student_compile(Optind opt) // 학생 코드 컴파일 및 에러처리 + 일부 채점
{
	char c;
	int isFile;
	int i,length;
	DIR * dir_std;
	int std_result; // 학생 실행 결과 저장
	bool check = FALSE;
	double pro_score; // 문제 점수
	double std_score; // 학생 점수

	char compare_pro[BUFFER_SIZE]; // 비교용
	char buf[BUFFER_SIZE]; 
	char point_c[NAME_SIZE*4]; // .c파일 저장 경로
	char point_exe[NAME_SIZE*4]; // .exe 파일 저장 경로

	char dir_err[NAME_SIZE*4]; // error 파일 이름
	char path_std[NAME_SIZE*4]; // 학생 파일 경로
	char std_id[NAME_SIZE*4]; // 학생 학번
	char std_err[NAME_SIZE*4]; // 학생 error 파일 이름
	char std_out[NAME_SIZE*4]; // 학생 파일 실행 결과 저장
	char ans_out[NAME_SIZE*4]; // 답안 파일 실행 결과 저장
	char compile[NAME_SIZE*4]; // 컴파일 문자열
	char execute[NAME_SIZE*4]; // 실행 문자열

	memset((char *)compare_pro,0,BUFFER_SIZE);
	memset((char *)dir_err,0,NAME_SIZE*4); // e 옵션 대비용 초기화
	memset((char *)execute,0,NAME_SIZE*4); // 실행파일용 배열 초기화
	
	if(opt.idx_e != 0) // e option이 있다면
	{
		sprintf(dir_err,"./");
		sprintf(dir_err,"%s%s", dir_err, store[opt.idx_e]); // dir_err : ./ERRORa
		isFile = access(dir_err,F_OK);
		if(isFile == 0)
		{
			sprintf(execute,"rm -r %s", dir_err);
			system(execute);
		}
		mkdir(dir_err,0766);
	}

	for(i = 2; i < count_std; i++) // 학번 개수만큼 돌기
	{
		std_score = 0;
		memset((char *)buf,0,NAME_SIZE*4);
		memset((char *)std_id,0,NAME_SIZE*4); // 학생 학번 초기화
		memset((char *)dir_err,0,NAME_SIZE*4);  // 에러파일 경로 초기화
		memset((char *)path_std,0,NAME_SIZE*4); // 학생 경로 저장용 배열 초기화
		memset((char *)point_c,0,NAME_SIZE*4); // .c 이름을 갖는 배열 초기화
		memset((char *)point_exe,0,NAME_SIZE*4); // .exe 이름을 갖는 배열 초기화
		memset((char *)std_out,0,NAME_SIZE*4); // 학생 실행결과용 배열 초기화
		memset((char *)ans_out,0,NAME_SIZE*4); // 정답 실행결과용 배열 초기화
		memset((char *)compile,0,NAME_SIZE*4); // 컴파일용 배열 초기화
		memset((char *)execute,0,NAME_SIZE*4); // 실행파일용 배열 초기화

		sprintf(std_id,"%s",flist_std[i]->d_name); // '2019XXXX'으로 이름 초기화
		sprintf(path_std,"%s/", fname_std); // './STUDENT/'으로 이름 초기화
		sprintf(path_std,"%s%s", path_std, flist_std[i]->d_name); // ./STUDENT/2019XXXX로 초기화
	
		if((dir_std = opendir(path_std)) == NULL) // ./STUDENT/2019XXXX 디렉토리 열기
		{
			fprintf(stderr,"opendir error for %s\n", path_std);
			exit(1);
		}
		else // ./STUDENT/2019XXXX가 열리면
		{
			int status; // 실행한 프로세스 상태 점검용
			int j,count_std_id;
			struct dirent ** flist_std_id; // 파일 리스트 정렬 및 추출용도

			if((count_std_id = scandir(path_std,&flist_std_id,0,versionsort)) < 0) // 정렬
			{
				fprintf(stderr,"scandir error for %s\n", path_std);
				exit(1);
			}
			else // scandir이 성공하면
			{
				if(opt.idx_e) // e option이 존재한다면
				{
					sprintf(dir_err,"./"); // ./
					sprintf(dir_err,"%s%s", dir_err, store[opt.idx_e]); // ./ERROR
					sprintf(dir_err,"%s/", dir_err); // ./ERROR/
					sprintf(dir_err,"%s%s", dir_err, std_id); // ./ERROR/2019XXXX
					mkdir(dir_err,0766); // ./ERROR/2019XXXX 만들기
				}
				for(j = 2; j < count_std_id; j++) // 학번 안에 있는 파일 개수만큼
				{
					memset((char *)dir_err,0,NAME_SIZE*4);
					memset((char *)buf,0,NAME_SIZE*4);
					memset((char *)point_c,0,NAME_SIZE*4);
					memset((char *)point_exe,0,NAME_SIZE*4);
					memset((char *)execute,0,NAME_SIZE*4);
					memset((char *)compile,0,NAME_SIZE*4);
				
					if(strstr(flist_std_id[j]->d_name,".c") != NULL) // .c파일 대상
					{
						std_score = 0;
						sprintf(point_c,"%s/", path_std); // ./STUDENT/2019XXXX/
						strncpy(compare_pro,flist_std_id[j]->d_name,strlen(flist_std_id[j]->d_name)); // 문제번호.c
						sprintf(point_c,"%s%s", point_c, flist_std_id[j] -> d_name); // ./STUDENT/2019XXXX/XX.c
						strncpy(point_exe,point_c,strlen(point_c)-2); // .c부분만 제외하고 복사
						sprintf(point_exe,"%s.stdexe",point_exe); // ./STUDENT/2019XXX/XX.stdexe
						
						if(opt.idx_e) // e option이 존재한다면
						{
							sprintf(dir_err,"./");
							sprintf(dir_err,"%s%s", dir_err, store[opt.idx_e]);
							sprintf(dir_err,"%s/", dir_err);
							sprintf(dir_err,"%s%s", dir_err, std_id);
							sprintf(dir_err,"%s/",dir_err);
							strncpy(buf,flist_std_id[j]->d_name,strlen(flist_std_id[j]->d_name)-2); // .c 부분을 제외한 '문제번호'만 복사
							sprintf(dir_err,"%s%s",dir_err,buf); // ./ERROR/2019XXXX/문제번호
							sprintf(dir_err,"%s_error.txt",dir_err); // ./ERROR/2019XXXX/XX_error.txt
						}

						check = FALSE;

						if(opt.idx_t) // t option이 존재하면
						{
							int count = 1;
							int index = opt.idx_t;

							while(index < count_arg && strstr(store[index],"-") == NULL) // 옵션 검사
							{
								count++;
								if(strstr(point_c,store[index]) != NULL && count < 6)
								{
									check = TRUE; // thead option이 필요한가?
									sprintf(compile,"gcc -o %s %s -lpthread",point_exe, point_c);
								}
								if(count > 5)
									break;
								index++;
							}
							if(!check) // thread 옵션이 필요 없던 .c 파일들
								sprintf(compile,"gcc -o %s %s",point_exe,point_c);
							else // thread 옵션이 필요한 .c 파일들
								sprintf(compile,"gcc -o %s %s -lpthread", point_exe,point_c);
							
						}
						else // t 옵션이 없다면
							sprintf(compile,"gcc -o %s %s",point_exe, point_c);

						check = FALSE;
					
						if(opt.idx_e) // e option이 존재하면
						{
							FILE * fre_std_compile = freopen(dir_err,"w",stderr);
							status = system(compile);

							if(status == 0) // error가 발생하지 않음 
							{
								int temp_fd; 
								int index = 0;
								
								if((temp_fd = open(dir_err,O_RDONLY)) < 0)
								{
									fprintf(stderr,"open error for %s\n", dir_err);
									exit(1);
								}

								check = TRUE;
								memset((char *)buf,0,BUFFER_SIZE);

								while((length = read(temp_fd,&c,1)) > 0)
								{
									buf[index++] = c;
									
									if(c == '\n')
									{
										if(strstr(buf,"warning:") != NULL)
										{
											std_score -= WARNING; // std_score += 0.1
											memset((char *)buf,0,BUFFER_SIZE);
										}
										index = 0;
									}
								}
								close(temp_fd);
							}
							if(status == 256) // error ?
							{
								check = FALSE;
								std_score = WRONG; // 0점
							}
							fre_std_compile = freopen("/dev/tty","w",stderr);
							
							if(check) // check = TRUE - error가 없는 경우
							{
								sprintf(execute,"rm %s",dir_err);
								system(execute);
								memset((char *)execute,0,NAME_SIZE*4);
							}
						}
						else // e 옵션이 없다면
						{
							memset((char *)dir_err,0,NAME_SIZE*4);
							sprintf(dir_err,"./"); // dir_err : ./
							sprintf(dir_err,"%s%s", dir_err, "temp.txt"); // dir_err : ./temp.txt
							FILE * fre_std_compile = freopen(dir_err,"w",stderr);
							status = system(compile);
							
							if(status == 0) // error가 발생하지 않음 
							{
								int index = 0;
								int temp_fd; 
								
								if((temp_fd = open(dir_err,O_RDONLY)) < 0)
								{
									fprintf(stderr,"open error for %s\n", dir_err);
									exit(1);
								}

								check = TRUE;
								memset((char *)buf,0,BUFFER_SIZE);

								while((length = read(temp_fd,&c,1)) > 0)
								{
									buf[index++] = c;
									
									if(c == '\n')
									{
										if(strstr(buf,"warning:") != NULL)
										{
											std_score -= WARNING; // std_score += 0.1
											memset((char *)buf,0,BUFFER_SIZE);
										}
										index = 0;
									}
								}
								close(temp_fd);
							}
							if(status == 256) // error ?
							{
								check = FALSE;
								std_score = WRONG; // 0점
							}
							fre_std_compile = freopen("/dev/tty","w",stderr);
							
							if(check)
							{
								sprintf(execute,"rm %s", dir_err);
								system(execute);
								memset((char *)execute,0,NAME_SIZE*4);
							}
						} // e 옵션이 없을 때의 else의 끝

						if(strstr(flist_std_id[j]->d_name,".c") != NULL)
						{
							int k;
							
							// std_id : 2019XXXX , compare_pro : XX.c

							if(check) // error가 아니라면
							{
								for(k = 0; k < count_std - 2; k++)
								{
									if(!strncmp(student[k].name,std_id,strlen(std_id))) // id == student[k].name
									{
										int l;
										for(l = 0; l < count_ans - 2; l++)
										{
											if(!strncmp(student[k].problem[l].ans,compare_pro,strlen(compare_pro)))
											{
												if(student[k].problem[l].score <= -(std_score)) //!!!
													student[k].problem[l].score = WRONG;
												else
													student[k].problem[l].score += std_score;
												break;
											}
										}
										break;
									}
								}
							}
							else
							{
								for(k = 0; k < count_std - 2; k++)
								{
									if(!strncmp(student[k].name,std_id,strlen(std_id)))
									{
										int l;
										for(l = 0; l < count_ans - 2; l++)
										{
											if(!strncmp(student[k].problem[l].ans,compare_pro,strlen(compare_pro)))
											{
												student[k].problem[l].score = WRONG;
												break;
											}
										}
									}
								}
							}
						}
					} // if strstr의 끝
				} // end of for loop of j
			} // scandir이 성공할 때의 else의 끝
		} // else의 끝 - 학번 파일이 열림
	} // end of for loop of i 
}

void student_execute(char * fname,Optind opt) // 학생 파일 실행
{
	bool check;
	int i,j,count;
	char path[NAME_SIZE*4];
	char name[NAME_SIZE*4];
	char path_out[NAME_SIZE*4];

	memset((char *)name,0,NAME_SIZE*4);
	memset((char *)path,0,NAME_SIZE*4);

	if(strstr(fname,".c") != NULL)
	{
		strncpy(name,fname,strlen(fname)-2); // name -> XX(문제이름)
		int status;
		struct dirent ** flist; // 학생의 학번 별 폴더 정렬

		for(i = 2; i < count_std; i++) // 학번의 수만큼 loop
		{
			memset((char *)path,0,NAME_SIZE*4);
			memset((char *)path_out,0,NAME_SIZE*4);

			sprintf(path,"%s/", fname_std); // ./STUDENT
			sprintf(path,"%s%s", path, flist_std[i]->d_name); // ./STUDENT/2019XXXX

			if((count = scandir(path,&flist,0,versionsort)) < 0) // 정렬해서 ./STUDENT/2019XXXX 열기
			{
				fprintf(stderr,"scandir error\n");
				exit(1);
			}
			
			for(j = 2; j < count; j++) // ./STUDENT/2019XXXX 안에 있는 파일의 수만큼
			{
				if(strstr(flist[j]->d_name,name) != NULL) // flist[j]->d_name이 XX(문제번호)를 포함해야함
				{
					if(strstr(flist[j]->d_name,".stdexe") != NULL) // flist[j]->d_name이 XX.stdexe이어야함
					{
						int l = 2;
						int k = 0;
						char status[NAME_SIZE*4];
						char kill[NAME_SIZE*8];
						char execute[NAME_SIZE*4];
						char point_c[NAME_SIZE*4];
				
						memset((char *)kill,0,NAME_SIZE*8);
						memset((char *)execute,0,NAME_SIZE*4);
						memset((char *)point_c,0,NAME_SIZE*4);
						memset((char *)status,0,NAME_SIZE*4);

						sprintf(kill,"killall %s", flist[j]->d_name); // killall XX.stdexe
						sprintf(path_out,"%s/", path); // ./STUDENT/2019XXXX/
						sprintf(execute,"%s", path); // execute : ./STUDENT/2019XXXX
						sprintf(execute,"%s/",execute);
						sprintf(execute,"%s%s", execute, flist[j]->d_name); // ./STUDENT/2019XXXX/XX.stdexe
						sprintf(path_out,"%s%s", path_out, name); // path_out : ./STUDENT/2019XXXX/XX
						sprintf(point_c,"%s", path_out); // point_c : ./STUDENT/2019XXXX/XX
						sprintf(point_c,"%s.c", point_c); // point_c : ./STUDENT/2019XXXX/XX.c
						sprintf(path_out,"%s.stdout", path_out); // path_out : ./STUDENT/2019XXXX/XX.stdout
						sprintf(status,"%s", execute);
						sprintf(execute,"./%s", status); // execute : ././STUDENT/2019XXXX/XX.stdexe
					/*	
						while(l < strlen(execute))
						{
							status[l-2] = execute[l];
							l++;
						}
					*/
						pthread_create(&t_id,NULL,execute_process,(void *)execute);
					
						if(!check_timeout(flist[j]->d_name,TIMEOUT)) // return 0일 때
						{
							k = 0;
							while(k < count_std - 2)
							{
								if(strstr(path,student[k].name) != NULL) // 2019XXXX
								{
									int l = 0;
									while(l < count_ans)
									{
										if(strstr(point_c,student[k].problem[l].ans) != NULL)
										{
											student[k].problem[l].score = WRONG;
											break;
										}
										l++;
									}
									break;
								}
								k++;
							}
							system(kill); // 무한루프 프로세스 강제종료
						}
						pthread_join(t_id, NULL);
					}
				}
			}
		}

	}
}

void * execute_process(void * arg) // thread용 함수
{
	int i = 0;
	char buf[NAME_SIZE*4];
	char path_out[NAME_SIZE*4];
	
	memset((char *)buf,0,NAME_SIZE*4);
	memset((char *)path_out,0,NAME_SIZE*4);

	sprintf(buf,"%s", (char *)arg);

	i = 2;
	while(i < strlen(buf) - 6)
	{
		path_out[i-2] = buf[i];
		i++;
	}
	sprintf(path_out,"%sstdout",path_out);

	if(arg == NULL)
		return (void *)0;

	sprintf(buf,"%s &", buf);

	FILE * fre_out = freopen(path_out,"w",stdout);
	system(buf);
	fre_out = freopen("/dev/tty","a",stdout);
}

int check_timeout(char *status,int wait) // 시간초과 잡는 함수
{
	int i,j,fd;
	char buf[NAME_SIZE*4];
	char temp[NAME_SIZE*4];
	char use[NAME_SIZE*4];

	for(i = 0; i < wait; i++)
	{
		memset((char *)use,0,NAME_SIZE*4);
		memset((char *)buf,0,NAME_SIZE*4);
		memset((char *)temp,0,NAME_SIZE*4);
		
		sprintf(use,"ps -e | grep ");
		sprintf(use,"%s%s", use, status);
		
		FILE * fre_out = freopen("./POUT.txt","w",stdout);
		FILE * fre_err = freopen("/dev/null","w",stderr);
		
		system(use);
		fflush(stdout);
		fflush(stderr);
		
		if((fd = open("./POUT.txt",O_RDONLY)) < 0)
			exit(1);
		
		read(fd,buf,NAME_SIZE*4);
		
		if(*buf == '\0')
		{
			fre_out = freopen("/dev/tty","a",stdout);
			fre_err = freopen("/dev/tty","a",stderr);
			return 1;
		}

		if(strstr(buf,status) == NULL)
		{
			fre_out = freopen("/dev/tty","a",stdout);
			fre_err = freopen("/dev/tty","a",stderr);
			return 1;
		}
	
		fre_out = freopen("/dev/tty","a",stdout);
		fre_err = freopen("/dev/tty","a",stderr);
		//printf("status : %s\n", status);
		//printf("buf : %s\n", buf);
		//printf("%s is still running!\n", status);
		sleep(1);
	}
	return 0;
}
