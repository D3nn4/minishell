#ifndef __HEADER_H__
#define __HEADER_H__
#include <stdbool.h>
#define SIZE_MAX  4096

typedef struct  s_env {
	char **raw_env;
	char **paths;
	char *home;
	char *current_directory;
}				t_env;

typedef struct 	s_function {
	char *name;
	char *args;

}				t_function;

void minishell(char **env);
void printPath (t_env *env);
void freeStructEnv (t_env **struct_env);
void getHome (char *string, t_env *env);
void createEnv (t_env **struct_env);
void freeStructFunction (t_function **function);
void echo (char *args);
void cd (char *args, t_env *env);
void displayEnv (char **env);
void mySetenv (char *data, t_env *env);
void addNewVar(t_env *env, char *var, char *value);
void modifyVar (t_env *env, char *var, char *value);
void myUnsetenv (char *data, t_env *env);
void deleteVar (t_env *env, char *var);
void noArgCd (t_env *env);
char *cdOldPwd (char **env);
char *getNewDir (char *args, t_env*env);
char *addSeparator (char *string);
char *eraseDots (char *str);
char * previousDir (char *str);
char *addPath (char *name, char *path);
char *cdAbsolute(char *args);
char *cdRelatif (char *args, t_env *env);
char **getPaths (char *string);
char **getListVar (char *data);
char **copy2D (char **env);
bool isDir (char *args);
bool checkVar (char **list, char *var);
bool applyLibFunction (char *path_to_test, t_function *data, t_env *env);
bool noPathFunction (t_function *data, t_env *env);
bool testLibFunction (t_function *data, t_env *env);
bool applyFunction (char *buffer, int ret, t_env *env);
bool FindBuiltInFunction (t_function *data, t_env *env);
bool findLibFunction (t_function *data, t_env *env);
t_function *getFunction (char *buffer);
t_env *getEnv (char **env);
int getSizeVar (char *buffer);




#endif