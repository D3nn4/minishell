#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "minishell.h"
char *getFunctionName (t_function *data, char *buffer, int i)
{
	data->name = malloc(sizeof(*data->name) * (i + 1));
	if (data->name == NULL)
		return NULL;
	data->name = strncpy(data->name, buffer, i - 1);
	data->name[i - 1] = '\0';	
	return data->name;
}

t_function *getFunction (char *buffer)
{	
	int curseur = 0;
	while (buffer[curseur] == ' ' || buffer[curseur] == '\t')
		curseur++;
	int i = getSizeVar(buffer + curseur);	
	if (i == 0)
		return NULL;
	t_function *data = malloc(sizeof(*data));
	if (data == NULL)
		return NULL;
	data->name = getFunctionName(data, buffer + curseur, i);
	i = i + curseur;
	while (buffer[i] == ' ' || buffer[i] == '\t')
		i++;
	if (buffer[i] == '\0' || buffer[i] == '\n')
		data->args = NULL;
	else {
		data->args = malloc (sizeof(*data->args) * strlen(buffer + i) + 1 );
		if (data->args == NULL)
			return NULL;
		data->args = strcpy(data->args, buffer + i);
		const int end_of_args = strlen (data->args);
		if (end_of_args > 1 && data->args[end_of_args - 1] == '\n')
			data->args[end_of_args - 1] = '\0';
	}
	return data;
}


bool FindBuiltInFunction (t_function *data, t_env *env)
{
	if (strcmp(data->name, "echo") == 0)
		echo(data->args);
	else if (strcmp (data->name, "cd") == 0){
		cd (data->args, env);
		chdir(env->current_directory);
	}
	else if (strcmp (data->name, "setenv") == 0)
		mySetenv (data->args, env);
	else if (strcmp (data->name, "unsetenv") == 0)
		myUnsetenv (data->args, env);
	else if (strcmp (data->name, "env") == 0)
		displayEnv (env->raw_env);
	else
		return false;
	return true;
}

bool applyFunction (char *buffer, int ret, t_env *env)
{
	(void)ret;
	t_function *data = getFunction(buffer);
	if (data == NULL)
		return true;
	if (strcmp(data->name, "exit") == 0
		|| strcmp(data->name, "quit") == 0){
		freeStructFunction(&data);
		return false;
	}
	if (FindBuiltInFunction(data, env) == false)
		if(findLibFunction(data, env) == false) 
			printf("%s: command not found\n", data->name);
	freeStructFunction(&data);
	return true;
}


void minishell (char **env)
{	
	size_t size_max = SIZE_MAX;
	t_env *struct_env = NULL;
	struct_env = getEnv(env);
	bool status = true;
	while (status){
		int ret = 0;
		char *buff = NULL;
		printf("%s$>", struct_env->current_directory);
		ret = getline(&buff, &size_max, stdin);
		if (ret <= 0)
			return;
		buff[ret] = '\0';
		if (ret > 1 && buff[0] != '\n')
			status = applyFunction (buff,ret, struct_env);
		if (buff[ret-1] != '\n')
			printf("\n");
		free(buff);
	}
	freeStructEnv (&struct_env);
	struct_env = NULL;
}


