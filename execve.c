#include <string.h>
#include <stdlib.h>
#include "minishell.h"

int sizeToMalloc (char *str)
{
	int nb = 1; // au moins un arg
	int i;
	for (i = 0; str[i] != '\0'; i++){
		if (str[i] == ' ' || str[i] == '\t'){
			while (str[i + 1] == ' ' || str[i + 1] == '\t')
				i++;
			if(str[i+1] != '\0')
				nb++;
		}	
	}
	return nb;
}

int sizeOfArg (char *str)
{
	int i;
	int size = 0;
	for (i = 0; str[i] != ' ' && str[i] != '\t' && str[i] != '\0'; i++)
		size++;
	return size;
}

char **noArgs (t_function *data)
{
	char **args = malloc (sizeof(*args) * 2); // data->name + null
	if (args == NULL)
		return NULL;
	args[0] = strdup(data->name);
	args[1] = NULL;
	return args;
}
char **getArgsExecve(t_function *data)
{
	if (data->args == NULL)
		return noArgs(data);
	char **args = NULL;
	int nb_args = 2 + sizeToMalloc(data->args); //  +2 => null + nom fonction
	int i;
	int curseur = 0;
	args = malloc(sizeof(*args) * (nb_args)); 
	if (args == NULL)
		return NULL;
	args[nb_args - 1] = NULL;
	args[0] = strdup(data->name);
	for (i = 1; i < nb_args - 1; i++) {
		int size = sizeOfArg(data->args + curseur);
		args[i] = strndup (data->args + curseur, size);
		curseur = curseur + size;
		while (data->args[curseur] == ' ' || data->args[curseur] == '\t')
			curseur++;
	}
	return args;
}