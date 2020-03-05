#ifndef _CALCULATOR_FOR_STACK_
#define _CALCULATOR_FOR_STACK_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0
#define MAX_INPUT 128

/*
   IS_NUMBER(x) : x값에 따라 숫자인지 아닌지 판별
   IS_OPERATOR(x) : x값에 따라 연산자인지 아닌지 판별
*/
#define IS_NUMBER(x) ((x >= '0' && x <= '9') || x == '.') 
#define IS_OPERATOR(x) (x == '+' || x == '-' || x == '*' || x == '/' || x == '(' || x == ')')

typedef _Bool bool;

int num_top,op_top;
char op[MAX_INPUT]; // operators
char number[MAX_INPUT][MAX_INPUT]; // numbers & operators

void init_calculator(void); // 계산기 프로그램 초기화

void remove_space(char * input); // 공백문자(space) 제거

void convert_postfix(char * input); // 후위연산법으로 변환

void calculate(void); // 계산하는 함수

double return_result(double num1,double num2,char op); // 주어진 값들을 연산자에 따라 연산

void push(char op1); // push함수

char pop(void); // pop함수

bool require_pop(char op1,char op2); // pop을 할 상황인지 아닌지 판단하는 함수

int priority(char op1); // 연산자 우선순위 판별

void syntax_error(void); // 단순 에러 메시지 출력

#endif
