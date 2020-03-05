#include "calculator.h"

int main(void)
{
	char input[MAX_INPUT];

	init_calculator();
	memset((char *)input,0,MAX_INPUT);

	fgets(input,MAX_INPUT,stdin);
	input[strlen(input)-1] = 0;

	remove_space(input);
	convert_postfix(input);
	calculate();

	exit(EXIT_SUCCESS);
}
