#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "minishell.h"


int main (int argc, char **argv, char**env)
{
	

	//if (father > 0){
		//sleep(5);
		printf("I'm your father\n");
		//printf("%d\n", father);
	//}
	pid_t father;
	father = fork();
	int return_value = 0;
	int status_exit;
	int status_wait;

	if (father > 0){
		wait(&status_wait);
		if (WIFEXITED(status_wait)){
		//	printf("in wifexited\n");
			return_value = WEXITSTATUS(status_wait);
		}

	}
	if (father == 0){

		printf("I'm your son\n");
		return_value = execve ("/binls", argv, env);
		exit(return_value);
	}
	
	printf("%d\n", return_value);
	printf("toto\n");
	return 0;
}