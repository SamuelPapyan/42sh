#ifndef FTSH_H
# define FTSH_H

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>

#include <readline/readline.h>
#include <readline/history.h>

#include "lexer.h"
#include "parser.h"
#include "env.h"

typedef struct s_shell {
    t_env *env;
    int last_status;
} t_shell;

void init_shell(t_shell *shell, char **envp);
void cleanup_shell(t_shell *shell);

void parse_and_execute(t_shell *shell, char *input);

void execute_cmd(t_shell *shell, t_cmd *cmd);

int is_builtin(const char *cmd);
int exec_builtin(t_shell *shell, char **argv);

int builtin_cd(t_shell *shell, char **argv);
int builtin_exit(t_shell *shell, char **argv);
int builtin_echo(t_shell *shell, char **argv);
int builtin_type(t_shell *shell, char **argv);

int builtin_export(t_shell *sh, char **argv);
int builtin_unset(t_shell *sh, char **argv);
int builtin_set(t_shell *sh, char **argv);

#endif