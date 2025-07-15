#include "42sh.h"

char	*generate_tmpfile(void)
{
	char *tmp = strdup("/tmp/heredoc_XXXXXX");
	int fd = mkstemp(tmp);
	if (fd == -1)
	{
		perror("mkstemp");
		free(tmp);
		return NULL;
	}
	close(fd);
	return tmp;
}

char    *extract_from_newline(char *str) {
    char    *newstr = malloc((strlen(str) * sizeof(char)));
    int     i = 0;
    while (str[i] != '\n') {
        newstr[i] = str[i];
        ++i;
    }
    newstr[i] = '\0';
    return newstr;
}

void	process_heredocs(t_cmd *cmd)
{
	while (cmd)
	{
		t_redir *r = cmd->redirs;
		while (r)
		{
			if (r->type == TOKEN_HEREDOC)
			{
				char *eof = r->file;
				char *tmpfile = generate_tmpfile();
				FILE *f = fopen(tmpfile, "w");
				if (!f)
				{
					perror("fopen");
					break;
				}

				char *line = NULL;
				size_t len = 0;

				while (1)
				{
                    printf("heredoc> ");
					ssize_t nread = getline(&line, &len, stdin);
					char *nonewline = extract_from_newline(line);
                    if (nread == -1 || strcmp(nonewline, eof) == 0) {
						free(nonewline);
                        break;
                    }
                    free(nonewline);
					if (line[strlen(line) - 1] == '\n')
						fputs(line, f);
					else
					{
						fputs(line, f);
						fputc('\n', f);
					}
				}
				free(line);
				fclose(f);
				free(r->file);
				r->file = tmpfile;
                r->is_tempfile = 1;
				// FILE *check = fopen(tmpfile, "r");
				// char fuf[256];
				// while (fgets(fuf, sizeof(fuf), check))
				// 	printf("-> %s", fuf);
				// fclose(check);
			}
			r = r->next;
		}
		process_heredocs(cmd->pipe_next);
		cmd = cmd->next;
	}
}