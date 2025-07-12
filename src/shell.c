#include "42sh.h"

void init_shell(t_shell *shell, char **envp)
{
    shell->env = env_from_envp(envp);
    shell->last_status = 0;
}

void cleanup_shell(t_shell *shell) {
    if (!shell->env)
        return;
    env_free(shell->env);
}