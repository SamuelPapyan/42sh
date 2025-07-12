#include "42sh.h"

int	builtin_export(t_shell *sh, char **argv)
{
	for (int i = 1; argv[i]; i++)
	{
		char *eq = strchr(argv[i], '=');
        printf("eq: %s\n", eq);
		if (eq)
		{
			*eq = 0;
			env_set(&sh->env, argv[i], eq + 1, 1);
			*eq = '=';
		}
		else
			env_set(&sh->env, argv[i], "", 1);
	}
	return 0;
}

int	builtin_unset(t_shell *sh, char **argv)
{
	for (int i = 1; argv[i]; i++)
		env_unset(&sh->env, argv[i]);
	return 0;
}

int	builtin_set(t_shell *sh, char **argv)
{
	(void)argv;
	env_print(sh->env, 0);
	return 0;
}