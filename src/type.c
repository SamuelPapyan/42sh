#include "42sh.h"

int builtin_type(t_shell *shell, char **argv) {
    (void)shell;
    if (!argv[1])
        return 1;
    for (int i = 1; argv[i]; i++) {
        if (is_builtin(argv[i]))
            printf("%s is a shell builtin\n", argv[i]);
        else
            printf("%s is %s\n", argv[i], access(argv[i], X_OK) == 0 ? argv[i] : "not found");
    }
    return 0;
}