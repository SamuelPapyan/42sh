#include "42sh.h"

int is_builtin(const char *cmd) {
    return (!strcmp(cmd, "cd") ||
            !strcmp(cmd, "exit") ||
            !strcmp(cmd, "echo") ||
            !strcmp(cmd, "export") ||
            !strcmp(cmd, "set") ||
            !strcmp(cmd, "unset") ||
            !strcmp(cmd, "type"));
}

int exec_builtin(t_shell *shell, char **argv) {
    if (!argv || !argv[0]) return 1;

    if (!strcmp(argv[0], "cd"))
        return builtin_cd(shell, argv);
    if (!strcmp(argv[0], "exit"))
        return builtin_exit(shell, argv);
    if (!strcmp(argv[0], "echo"))
        return builtin_echo(shell, argv);
    if (!strcmp(argv[0], "type"))
        return builtin_type(shell, argv);
    if (!strcmp(argv[0], "export"))
        return builtin_export(shell, argv);
    if (!strcmp(argv[0], "unset"))
        return builtin_unset(shell, argv);
    if (!strcmp(argv[0], "set"))
        return builtin_set(shell, argv);
    return 1;
}