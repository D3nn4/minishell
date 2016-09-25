#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include "minishell.h"

char *previousDir (char *str)
{
	char *new_str = NULL;
	int size, i,j;
	size = strlen(str) - 1; //to be on last /
	i = size;
	while (str[i - 1] != '/')
		i--;
	new_str = malloc (sizeof(*new_str) * (i + 1)); // +1 = Null byte
	if (new_str == NULL)
		return NULL;
	for (j = 0; j < i+1; (new_str[j++] = '\0'));
	new_str = strncpy(new_str, str, i);
	new_str[i] = '\0';
	free (str);
	return new_str;
}

bool isDir(char *path)
{
	DIR *dir;
	dir = opendir(path);
	if (dir == NULL){
			closedir (dir);
			return false;
	}
	closedir(dir);
	return true;
}

char *getNewDir (char *args, t_env*env)
{
	char *new_dir = NULL;
	if (((strcmp(args, "..") == 0) && (strcmp(env->current_directory, "/") == 0))
		|| strcmp(args, ".") == 0) {
		int size = strlen(env->current_directory);
		new_dir = malloc(sizeof(*new_dir) * (size + 1));
		new_dir = strcpy(new_dir, env->current_directory);
		new_dir[size] = '\0';
	}
	else if (strcmp(args, "-") == 0)
		new_dir = cdOldPwd(env->raw_env);
	else{
		if (((new_dir = cdAbsolute(args)) == NULL))
			new_dir = cdRelatif(args, env);
	}
	return new_dir;
}

char *createNextDir (t_env *env)
{
	char *next_dir = malloc(sizeof(*next_dir) * (strlen(env->current_directory) + 1)); // +1: Null byte
	if (next_dir == NULL)
		return NULL;
	next_dir = strcpy(next_dir, env->current_directory);
	next_dir = addSeparator(next_dir);
	next_dir[strlen(next_dir)] = '\0';

	return next_dir;
}