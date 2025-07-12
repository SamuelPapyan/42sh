#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    char *commands[][4] = {
        {"echo", "Hello World!", NULL},
        {"wc", "-c", NULL}
    };
    int num_cmds = 2;

    int pipes[num_cmds - 1][2];
    pid_t pids[num_cmds];

    // Создание пайпов
    for (int i = 0; i < num_cmds - 1; i++) {
        if (pipe(pipes[i]) == -1) {
            perror("pipe");
            exit(1);
        }
    }

    // Создание процессов
    for (int i = 0; i < num_cmds; i++) {
        pids[i] = fork();
        if (pids[i] == -1) {
            perror("fork");
            exit(1);
        }

        if (pids[i] == 0) {
            // Дочерний процесс

            if (i != 0) {
                dup2(pipes[i - 1][0], STDIN_FILENO);
            }
            if (i != num_cmds - 1) {
                dup2(pipes[i][1], STDOUT_FILENO);
            }

            for (int j = 0; j < num_cmds - 1; j++) {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }

            execvp(commands[i][0], commands[i]);
            perror("execvp");
            exit(1);
        }
    }

    // Родитель закрывает пайпы
    for (int i = 0; i < num_cmds - 1; i++) {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }

    // Ожидание завершения и проверка статуса
    int status;
    for (int i = 0; i < num_cmds; i++) {
        pid_t wpid = waitpid(pids[i], &status, 0);
        if (wpid == -1) {
            perror("waitpid");
            exit(1);
        }

        if (WIFEXITED(status)) {
            int code = WEXITSTATUS(status);
            printf("Команда #%d (pid %d) завершилась с кодом %d\n", i, wpid, code);
        } else {
            printf("Команда #%d (pid %d) завершилась аварийно\n", i, wpid);
        }
    }

    return 0;
}
