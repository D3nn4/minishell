#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include "minishell.h"


char *addSeparator (char *string)
{
	int size = strlen (string);
	if ((string[size - 1] != '/')){
		string = realloc (string, sizeof(*string) * (size + 2));
		strcat(string, "/");
	}
	return string;
}

char **copy2D (char **raw_env)
{
	char **env = NULL;
	int i;
	int size = 0;
	for(i = 0; raw_env[i] != NULL; i++)
		size++;
	env = malloc(sizeof(*env) * (size + 1));
	if (env == NULL)
		return NULL;
	for (i = 0; raw_env[i] != NULL; i++) {
		size = strlen(raw_env[i]);
		env[i] = malloc(sizeof(**env) * (size + 1));
		if (env[i] == NULL)
			return NULL;
		env[i] = strcpy(env[i], raw_env[i]);
		env[i][size] = '\0';
	}
	env[i] = NULL;
	return env;
}

char *eraseDots (char *str)
{
	int i;
	char *new_str = malloc (sizeof(*new_str) * (strlen(str) + 1));
	if (new_str == NULL)
		return NULL;
	new_str[0] = '\0';
	for (i = 0; str[i] != '\0'; i++){
		while (str[i] == '.'){
			if (str[i + 1] == '.'){
				new_str = previousDir(new_str);
				i = i + 3; //from ../ next dir
				if (strlen(new_str) == 1 && str[i] == '\0')
					return new_str;
				int new_size = strlen(new_str) + strlen(str + i) + 1;
				new_str = realloc(new_str, new_size);
			}
			else if (str[i + 1] == '/' || str[i + 1] == '\0')
				i = i + 2; //from ./ next dir
			else
				i++;
		}
		strncat (new_str, str + i, 1);
	}
	new_str[strlen(new_str)] = '\0';
	//strncat (new_str, str + i, 1);
	free (str);
	return new_str;
}