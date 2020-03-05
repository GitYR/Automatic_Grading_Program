#include "calculator.h"

void init_calculator(void)
{
	num_top = op_top = -1;
	memset((char *)op,0,MAX_INPUT);

	for(int i = 0; i < MAX_INPUT; i++)
		memset((char *)number[i],0,MAX_INPUT);
}

void remove_space(char * input)
{
	int i,j,len = strlen(input);

	for(i = 0; input[i] != '\0'; i++)
	{
		if(input[i] == ' ')
		{
			for(j = i; input[j] != '\0'; j++)
				input[j] = input[j+1];
			i--;
		}
	}
}

// 후위 연산으로 변환
void convert_postfix(char * input)
{
	int i,j,len = strlen(input);

	if(num_top >= MAX_INPUT)
		syntax_error();

	i = 0;
	while(i < len && input[i] != '\0')
	{
		if(IS_NUMBER(input[i]))
		{
			num_top++;
			for(j = i; IS_NUMBER(input[j]); j++)
				number[num_top][j-i] = input[j];
			i = j;
			continue;
		}
		else if(IS_OPERATOR(input[i]))
		{
			if(op_top == -1)
				push(input[i]);
			else
			{
				if(require_pop(op[op_top],input[i])) // true
				{
					number[++num_top][0] = pop();
					push(input[i]);
				}
				else // false
				{
					if(input[i] == ')' && op[op_top] == '(')
					{
						pop();
						i++;
						continue;
					}
					if(input[i] == ')')
					{
						for(j = op_top; op[j] != '(' && j > -1; j--)
							number[++num_top][0] = pop();
						if(j != -1)
							if(pop() != '(')
								syntax_error();
						i++;
						continue;
					}
					push(input[i]);
				}
			}
		}
		else
			syntax_error();
		i++;
	}

	i = op_top;
	while(i > -1)
	{
		number[++num_top][0] = pop();
		i--;
	}
}

void calculate(void)
{
	int i,j;
	char temp[MAX_INPUT];
	double num1,num2,result;

	for(i = 0; number[i][0] != '\0'; i++)
	{
		if(IS_OPERATOR(number[i][0]))
		{
			if(i < 2)
				syntax_error();
			memset((char *)temp,0,MAX_INPUT);
			num1 = atof(number[i-2]);
			num2 = atof(number[i-1]);
			result = return_result(num1,num2,number[i][0]);
			sprintf(temp,"%lf", result);
			temp[strlen(temp)] = 0;
			strcpy(number[i-2],temp);
			
			for(j = i; number[j][0] != '\0'; j++)
				strcpy(number[j],number[j+1]);
			for(j = i-1; number[j][0] != '\0'; j++)
				strcpy(number[j],number[j+1]);
			i = 0;
			continue;
		}
	}

	printf("result : %s\n", number[0]);
}

double return_result(double num1,double num2,char op)
{
	if(op == '+')
		return num1 + num2;
	if(op == '-')
		return num1 - num2;
	if(op == '*')
		return num1 * num2;
	if(op == '/')
		return num1 / num2;
}

void push(char op1)
{
	op[++op_top] = op1;
}

char pop(void)
{
	return op[op_top--];
}

bool require_pop(char op1,char op2)
{
	if(priority(op1) > priority(op2))
	{
		if(op1 == '(')
			return FALSE;
		return TRUE;
	}
	else
		return FALSE;
}

int priority(char op1)
{
	if(op1 == '(' || op1 == ')')
		return 5;
	else if(op1 == '*' || op1 == '/')
		return 3;
	else if(op1 == '+' || op1 == '-')
		return 1;
}

void syntax_error(void)
{
	fprintf(stderr,"SYNTAX ERROR\n");
	exit(EXIT_FAILURE);
}
