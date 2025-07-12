#include "42sh.h"

static char *expand_var(const char *token, t_env *env, int last_status)
{
	char *res = malloc(4096);
	res[0] = 0;

	for (int i = 0; token[i]; )
	{
		if (token[i] == '$')
		{
			i++;
			if (token[i] == '?')
			{
				char buf[12];
				snprintf(buf, sizeof(buf), "%d", last_status);
				strcat(res, buf);
				i++;
			}
			else
			{
				char var[256]; int j = 0;
				while (token[i] && (isalnum(token[i]) || token[i] == '_'))
					var[j++] = token[i++];
				var[j] = 0;
				char *val = env_get(env, var);
				if (val) strcat(res, val);
			}
		}
		else
		{
			int len = strlen(res);
			res[len] = token[i++];
			res[len+1] = 0;
		}
	}
	return res;
}

void expand_variables(char **tokens, t_env *env, int last_status)
{
	for (int i = 0; tokens[i]; i++)
	{
		if (strchr(tokens[i], '$'))
		{
			char *new = expand_var(tokens[i], env, last_status);
			free(tokens[i]);
			tokens[i] = new;
		}
	}
}