#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "minishell.h"

char **getPaths (char *string)
{
	if (string[5] == '\0')//premier char de la premiere valeur)
		return NULL;
	int nb_path = 1;
	int i, j;
	for (i = 0; i < (int)strlen(string); i++){
		if (string[i] == ':')
			nb_path++;
	}
	char **paths = malloc(sizeof(*paths) * (nb_path + 1));
	if (paths == NULL)
		return NULL;
	int curseur = 5;
	for (i = 0; i < nb_path; i++){
		j = curseur;
		while (string[j] != ':' && string[j] != '\0')
			j++;
		paths[i] = malloc (sizeof(**paths) * (j - curseur) + 1);
		if (paths[i] == NULL) 
			return NULL;
		paths[i] = strncpy (paths[i], string + curseur, j - curseur);
		paths[i][j - curseur] = '\0';
		curseur = j + 1;
	}
	paths[nb_path] = NULL;
	return paths;
}

void getHome (char *string, t_env *env)
{
	int i;
	int lenght = 0;
	for (i = 5; string[i] ; i++)
		lenght++;
	env->home = malloc(sizeof(*env->home) * lenght + 1);
	if (env->home == NULL)
		return;
	env->home = strcpy(env->home, string + 5);
}

void getCurrent (char *string, t_env *env)
{
	int i;
	int lenght = 0;
	for (i = 4; string[i] ; i++)
		lenght++;
	env->current_directory = malloc(sizeof(*env->current_directory) * lenght + 1);
	if (env->current_directory == NULL)
		return;
	env->current_directory = strcpy (env->current_directory, string + 4);
}

void createEnv (t_env **struct_env)
{
	t_env *env = *struct_env;
	int size = SIZE_MAX;
	if (env->raw_env == NULL){
		env->raw_env = malloc(sizeof(*env->raw_env) * 1); 
		if (env->raw_env == NULL)
			return;
	}
	char *buf = malloc(sizeof(*buf) * size);
	if (buf == NULL)
		return;
	buf = getcwd(buf, size);
	if (checkVar(env->raw_env, "OLDPWD") == false)
		addNewVar (env, "OLDPWD", buf);
	else 
		modifyVar(env, "OLDPWD", buf);
	if (checkVar(env->raw_env, "HOME") == false)
		addNewVar (env, "HOME", buf);
	else
		modifyVar(env, "HOME", buf);
	if (checkVar(env->raw_env, "PATH") == false)
		addNewVar (env, "PATH", '\0');
	else
		modifyVar(env, "PATH", '\0');
	free(buf);
}

t_env *getEnv (char **env)
{
	int i;
	t_env *struct_env = malloc(sizeof(*struct_env));
	if (struct_env == NULL)
		return NULL;
	struct_env->paths = NULL;
	struct_env->current_directory = NULL;
	struct_env->home = NULL;
	struct_env->raw_env = copy2D(env);
	if (env == NULL || checkVar(env, "HOME") == false){
		createEnv(&struct_env);
	}	
	for (i = 0; struct_env->raw_env[i]; i++){
		if (strncmp(struct_env->raw_env[i], "PATH=", 5) == 0)
			struct_env->paths = getPaths (struct_env->raw_env[i]);
		if (strncmp (struct_env->raw_env[i], "HOME=", 5) == 0)
			getHome(struct_env->raw_env[i], struct_env);
		if (strncmp (struct_env->raw_env[i], "PWD=", 4) == 0)
			getCurrent(struct_env->raw_env[i], struct_env);	
	}
	chdir(struct_env->current_directory);
	return struct_env;
}