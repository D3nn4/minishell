#include <stdio.h>
#include "minishell.h"

void echo (char *args)
{
	if (args == NULL){
		printf("\n");
		return;
	}
	printf("%s\n", args);
}