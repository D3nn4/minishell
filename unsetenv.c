#include <string.h>
#include <stdlib.h>
#include "minishell.h"

char **getListVar (char *data)  
{
	int i;
	int j = 0;
	int arg_nb = 1;
	for (i = 0; data[i] != '\0'; i++){
		if (data[i] == ' ')
			arg_nb++;
	}
	char **list_var = malloc(sizeof(*list_var) * (arg_nb + 1)); // +1 =>last pointeur null
	if (list_var == NULL) 
		return NULL;
	for (i = 0; i < arg_nb; i++){
		int size = 0;
		int curseur = j;
		while (data[j] != ' ' && data[j] != '\0') {
			size++;
			j++;
		}
		if (data[j] == ' ')
			j++;
		list_var[i] = strndup(data + curseur, size);
	}
	list_var[arg_nb] = NULL;
	return list_var;
}

void deleteVar (t_env *env, char *var)
{
	int size = 0;
	int i;
	int j = 0;
	char **new_env = NULL;
	for(i = 0; env->raw_env[i] != NULL; i++)
		size++;
	new_env = malloc(sizeof(*new_env) * size); 
	if (new_env == NULL)
		return;
	for (i = 0; env->raw_env[i] != NULL; i++) {
		if (strncmp(env->raw_env[i], var, strlen(var)) != 0){
			new_env[j] = env->raw_env[i];
			j++;
		}
		else
			free(env->raw_env[i]);
	}
	new_env[size - 1] = NULL;
	free(env->raw_env);
	env->raw_env = new_env;
}

void myUnsetenv (char *data, t_env *env)
{
	if (data == NULL){
		return;
	}
	char **list_var = NULL;
	int i;
	list_var = getListVar (data);
	for (i = 0; list_var[i] != NULL; i++) {
		if (checkVar(env->raw_env, list_var[i]))
			deleteVar(env, list_var[i]);
	}
	for (i = 0; list_var[i] != NULL; i++) {
		free(list_var[i]);
	}
	free(list_var);
	list_var = NULL;
	return;
}