#include "42sh.h"

t_env *env_from_envp(char **envp) {
    t_env *env = NULL;
    for (int i = 0; envp[i]; i++) {
        char *eq = strchr(envp[i], '=');
        if (!eq) continue;
        char *name = strndup(envp[i], eq - envp[i]);
        char *val = strdup(eq + 1);
        env_set(&env, name, val, 1);
        free(name);
        free(val);
    }
    return env;
}

char *env_get(t_env *env, const char *name) {
    while (env) {
        if (!strcmp(env->name, name))
            return env->value;
        env = env->next;
    }
    return NULL;
}

void    env_set(t_env **env, const char *name, const char *value, int export_flag)
{
    t_env *e = *env;
    while (e) {
        if (!strcmp(e->name, name))
        {
            free(e->value);
            e->value = strdup(value);
            if (export_flag)
                e->exported = 1;
            return;
        }
        e = e->next;
    }
    t_env *new = malloc(sizeof(t_env));
    new->name = strdup(name);
    new->value = strdup(value);
    new->exported = export_flag;
    new->next = *env;
    *env = new;
}

void    env_unset(t_env **env, const char *name) {
    t_env *prev = NULL, *cur = *env;
    while (cur) {
        if (!strcmp(cur->name, name)) {
            if (prev)
                prev->next = cur->next;
            else
                *env = cur->next;
            free(cur->name);
            free(cur->value);
            free(cur);
            return ;
        }
        prev = cur;
        cur = cur->next;
    }
}

void env_print(t_env *env, int only_exported) {
    while (env) {
        if (!only_exported || env->exported)
            printf("%s=%s\n", env->name, env->value);
        env = env->next;
    }
}

void    env_free(t_env *env) {
    while (env) {
        t_env *next = env->next;
        free(env->name);
        free(env->value);
        free(env);
        env = next;
    }
}

char    **env_to_envp(t_env *env) {
    int count = 0;
    t_env *e = env;
    while (e) {
        if (e->exported) count++;
        e = e->next;
    }

    char **envp = malloc(sizeof(char *) * (count + 1));
    int i = 0;
    while (env) {
        if (env->exported) {
            size_t len = strlen(env->name) + strlen(env->value) + 2;
            envp[i] = malloc(len);
            snprintf(envp[i], len, "%s=%s", env->name, env->value);
            i++;
        }
        env = env->next;
    }
    envp[i] = NULL;
    return envp;
}