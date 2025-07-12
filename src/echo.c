#include "42sh.h"

int builtin_echo(t_shell *shell, char **argv) {
    (void)shell;
    int i = 1;
    while (argv[i]) {
        printf("%s", argv[i]);
        if (argv[i + 1])
            printf(" ");
        i++;
    }
    printf("\n");
    return 0;
}