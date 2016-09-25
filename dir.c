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
/*
char *isDir2 (char *args, t_env *env)
{	
	int temp_size = 0;
	char *next_dir = malloc(sizeof(*next_dir) * (strlen(env->current_directory) + 1)); // +1: Null byte
	next_dir = strcpy(next_dir, env->current_directory);
	DIR *dir = NULL;
	next_dir = addSeparator(next_dir);
	next_dir[strlen(next_dir)] = '\0';
	if ((strcmp(args, "..") == 0) && args[0] == '.' && args[1] == '.') {
		next_dir = previousDir(next_dir);
		return next_dir;
	}
	if (args[0] == '/')
		args = args + 1;
	if (args == NULL)
		temp_size = strlen(next_dir);
	else
		temp_size = strlen(next_dir) + strlen(args);
	//next_dir = realloc (next_dir, sizeof(*next_dir) * (temp_size + 1)); // +1: Null byte
	char *temp_dir = malloc(sizeof(*next_dir) * (temp_size + 1));
	temp_dir = strcpy (temp_dir, next_dir);
	free (next_dir);
	next_dir = temp_dir;
		if (next_dir == NULL){
		printf("error realloc isDir\n");
		return NULL;
	}
	if (args != NULL)
		strcat (next_dir, args);
	next_dir[temp_size] = '\0';
	next_dir = eraseDots (next_dir);
	if (strcmp(next_dir, "/") != 0) {
		if(isDir(next_dir))
			return next_dir;
	}
	return NULL;
}
*/