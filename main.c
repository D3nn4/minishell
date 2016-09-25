#include "minishell.h"

int main (int argc, char **argv, char **env)
{
	(void)argv;
	(void)argc;
	minishell(env);
	return 0;
}