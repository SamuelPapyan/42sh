#include "42sh.h"

int builtin_exit(t_shell *shell, char **argv) {
    int code = argv[1] ? atoi(argv[1]) : shell->last_status;
    exit(code);
}