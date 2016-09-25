#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "minishell.h"

void printPath (t_env *env)
{
	int i;
	for (i = 0; env->paths[i] != NULL; i++) {
		printf("%s\n", env->paths[i]);
	}
}

char *addPath (char *name, char *path)
{
	char *str = malloc(sizeof(*str) * (strlen(name) + strlen(path) + 2)); // +2 => separator + \0
	if (str == NULL)
		return NULL;
	str = strcpy(str,path);
	strcat(str, "/");
	strcat(str, name);
	strcat(str, "\0");
	return str;
}