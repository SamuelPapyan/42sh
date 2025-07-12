#include "42sh.h"

int builtin_cd(t_shell *shell, char **argv) {
    (void)shell;
    const char *target = argv[1] ? argv[1] : getenv("HOME");

    if (!target) {
        fprintf(stderr, "cd: HOME not set\n");
        return 1;
    }
    if (chdir(target) != 0) {
        perror("cd");
        return 1;
    }
    return 0;
}