#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "minishell.h"


void addNewVar(t_env *env, char *var, char *value)
{
	int size = 0;
	int i;
	int str_len = strlen(var);
	if (value)
		str_len = strlen(var) + strlen(value);
	for(i = 0; env->raw_env[i] != NULL; i++)
		size++;
	char **new_env = malloc(sizeof(*new_env) * (size + 2));// +2 => newvalue + NULL
	if (new_env == NULL)
		return;
	for (i = 0; env->raw_env[i] != NULL; i++) {
		new_env[i] = env->raw_env[i];
	}
	new_env[i+1] = NULL;
	new_env[i] = malloc(sizeof(**new_env) * (str_len + 2)); // +2 => = + '\0'
	if (new_env[i] == NULL) 
		return;
	new_env[i] = strcpy(new_env[i], var);
	strcat(new_env[i], "=");
	if (value != NULL)
		strcat(new_env[i], value);
	strcat(new_env[i], "\0");
	free(env->raw_env);
	env->raw_env = new_env;
}

void modifyVar (t_env *env, char *var, char *value)
{
	int size = strlen(var);
	int i = 0;
	char **list = env->raw_env;
	while (list[i] != NULL && strncmp(list[i], var, size) != 0) 
		i++;
	if (list[i] == NULL){
		addNewVar(env, var, value);
		return;
	}
	int value_len = 0;
	if (value)
		value_len = strlen(value);
	free(list[i]);
	list[i] = malloc(sizeof(*list[i]) * (value_len + size + 2)); // + 2 for = && \0
	if (list[i] == NULL)
		return;
	list[i] = strcpy(list[i], var);
	strcat(list[i], "=");
	if (value)
		strcat(list[i], value);
	else
		strcat(list[i], "\0");
}

bool checkVar (char **list, char *var)
{	
	int size = strlen(var);
	int i ;
	for (i = 0; list[i] != NULL; i++) {
		if(list[i] != NULL && strncmp(list[i], var, size) == 0)
			return true;
	}
	return false;
}

void mySetenv (char *data, t_env *env)
{
	if (data == NULL){
		printf("args null setenv\n");
		displayEnv(env->raw_env);
		return;
	}
	t_function *var_to_add = NULL;
	var_to_add = getFunction(data);
	if (checkVar(env->raw_env, var_to_add->name)) 
		modifyVar(env, var_to_add->name, var_to_add->args);
	else
		addNewVar(env, var_to_add->name, var_to_add->args);
	freeStructFunction (&var_to_add);
}