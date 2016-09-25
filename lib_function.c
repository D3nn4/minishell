#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include "minishell.h"

bool applyLibFunction (char *path_to_test, t_function *data, t_env *env)
{
	int return_value = 0;
	int status_wait;
	char **args = getArgsExecve(data);
	pid_t father;
	father = fork();
	if (father > 0){
		wait(&status_wait);
		if (WIFEXITED(status_wait))
			return_value = WEXITSTATUS(status_wait);
	}
	if (father == 0){
		return_value = execve (path_to_test, args, env->raw_env);
		exit(return_value);
	}
	int i;
	for (i = 0; args[i] != NULL; i++){
		free(args[i]);
	}
	free(args);
	if (return_value == 0)
		return true;
	return false;
}
bool noPathFunction (t_function *data, t_env *env)
{
	char *path_to_test = NULL;
	struct stat *buf = NULL;
	buf = malloc(sizeof(*buf));
	if (buf == NULL)
		return NULL;
	path_to_test = malloc(sizeof(*path_to_test) * (strlen(data->name) + 1));
	if (path_to_test == NULL)
		return NULL;
	path_to_test = strcpy(path_to_test, data->name);
	path_to_test[strlen(data->name)] = '\0';
	if(stat(path_to_test, buf) == 0) {
		applyLibFunction(path_to_test, data, env);
		free(path_to_test);
		free(buf);
		return true;
	}
	free(path_to_test);
	free(buf);
	return false;
}
bool testLibFunction (t_function *data, t_env *env)
{
	int i;
	char *path_to_test = NULL;
	struct stat *buf = malloc(sizeof(*buf));;
	if (buf == NULL)
		return NULL;
	for (i = 0; env->paths[i] != NULL; i++) {
			path_to_test = addPath (data->name, env->paths[i]);
			if(stat(path_to_test, buf) == 0) {
				applyLibFunction(path_to_test, data, env);
				free(path_to_test);
				free(buf);
				return true;
			}
			free(path_to_test);
	}
	free(buf);
	return false;
}

bool findLibFunction (t_function *data, t_env *env)
{
	struct stat *buf = malloc(sizeof(*buf));
	if (buf == NULL)
		return NULL;
	if(stat(data->name, buf) == 0) {
		applyLibFunction(data->name, data, env);
		free(buf);
		return true;
	}
	else if (env->paths == NULL || env->paths[0] == '\0'){
		free(buf);
		return noPathFunction(data, env);
	}
	else {
		free(buf);
		return testLibFunction(data, env);
	}
	free(buf);
	return false;	
}