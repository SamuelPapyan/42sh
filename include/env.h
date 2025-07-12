#ifndef ENV_H
# define ENV_H

typedef struct s_env {
    char            *name;
    char            *value;
    int             exported;
    struct s_env    *next;
} t_env;

char    *env_get(t_env *env, const char *name);
void    env_set(t_env **env, const char *name, const char *value, int export_flag);
void    env_unset(t_env **env, const char *name);
void    env_print(t_env *env, int only_exported);
void    env_free(t_env *env);
t_env   *env_from_envp(char **envp);

char    **env_to_envp(t_env *env);
void    expand_variables(char **tokens, t_env *env, int last_status);

#endif