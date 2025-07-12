#include "42sh.h"

int g_exit_status = 0;

void sigint_handler(int signo) {
    (void)signo;
    write(1, "\n> ", 3);
}

void init_signals(void) {
    signal(SIGINT, sigint_handler);
    signal(SIGQUIT, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);
}

int main(int argc, char **argv, char **envp) {
    char *line;
    t_shell shell;

    (void)argc;
    (void)argv;

    init_signals();
    init_shell(&shell, envp);

    while (1) {
        line = readline("> ");
        if (!line)
            break;
        if (*line) {
            add_history(line);
            parse_and_execute(&shell, line);
        }
        free(line);
    }
    cleanup_shell(&shell);
    return (g_exit_status);
}