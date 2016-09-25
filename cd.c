#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "minishell.h"

char *cdAbsolute(char *args)
{
	char *new_dir = NULL;
	if (args[0] == '/'){
		if (isDir(args)){
			new_dir = malloc(sizeof(*new_dir) * (strlen(args) + 1));
			if (new_dir == NULL)
				return NULL;
			new_dir = strcpy(new_dir, args);
		}
	}
	return new_dir;	
}

char *cdRelatif (char *args, t_env *env)
{
	char *next_dir = malloc(sizeof(*next_dir) * (strlen(env->current_directory) + 1)); // +1: Null byte
	next_dir = strcpy(next_dir, env->current_directory);
	next_dir = addSeparator(next_dir);
	next_dir[strlen(next_dir)] = '\0';
	if ((strcmp(args, "..") == 0) && args[0] == '.' && args[1] == '.') 
		next_dir = previousDir(next_dir);
	else{
		testDir(args, &next_dir);
	}
	return next_dir;
}

void noArgCd (t_env *env)
{
	if (checkVar(env->raw_env, "OLDPWD"))
			modifyVar(env, "OLDPWD", env->current_directory);
		else
			addNewVar(env, "OLDPWD", env->current_directory);
		modifyVar(env, "PWD", env->home);

		free(env->current_directory);
		env->current_directory = strdup(env->home);
		return;
}

char *cdOldPwd (char **env)
{
	if (checkVar(env, "OLDPWD") == false){
			printf("minishell: cd: OLDPWD not set\n");
			return NULL;
	}
	char *str = NULL;
	if (checkVar(env, "OLDPWD")){
		int i;
		for (i = 0; env[i] != NULL; i++){
			if (strncmp(env[i], "OLDPWD", 6) == 0){
				int size = strlen(env[i] + 7);
				str = malloc(sizeof(*str) * (size + 1));
				str = strcpy(str, env[i] + 7);
				break;
			}
		}
	}
	return str;	
}

void cd (char *args, t_env *env)
{
	char *new_dir = NULL;
	if (args == NULL){
		noArgCd(env);
		return;
	}
	new_dir = getNewDir(args, env);
	if (new_dir == NULL){
		printf("minishell: cd: %s: No such file or directory\n", args);
		return;
	}
	if (checkVar(env->raw_env, "OLDPWD"))
		modifyVar(env, "OLDPWD", env->current_directory);
	else
		addNewVar(env, "OLDPWD", env->current_directory);modifyVar(env, "OLDPWD", env->current_directory);
	modifyVar(env, "PWD", new_dir);
	free(env->current_directory);
	env->current_directory = new_dir;
}
