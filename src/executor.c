#include "42sh.h"

// Обработка редиректов (>, >>, <)
static void	apply_redirs(t_redir *r)
{
	while (r)
	{
		int fd = -1;
		if (r->type == TOKEN_REDIR_OUT)
			fd = open(r->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else if (r->type == TOKEN_REDIR_APPEND)
			fd = open(r->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else if (r->type == TOKEN_REDIR_IN)
			fd = open(r->file, O_RDONLY);

		if (fd == -1)
		{
			perror("open");
		}
		else if (r->type == TOKEN_REDIR_IN)
			dup2(fd, STDIN_FILENO);
		else
			dup2(fd, STDOUT_FILENO);
		close(fd);
		r = r->next;
	}
}

// Выполнение пайпов: a | b | c
void	run_pipeline(t_cmd *cmd, t_shell *shell)
{
	int		prev_fd = -1;
	int		fd[2];
	pid_t	pids[256];
	int		pid_count = 0;

	while (cmd)
	{
		if (cmd->pipe_next && pipe(fd) == -1)
		{
			perror("pipe");
			return;
		}

		pid_t pid = fork();
		if (pid == -1)
		{
			perror("fork");
			return;
		}
		else if (pid == 0)
		{
			if (prev_fd != -1)
			{
				dup2(prev_fd, STDIN_FILENO);
				close(prev_fd);
			}
			if (cmd->pipe_next)
			{
				close(fd[0]); // не читаем
				dup2(fd[1], STDOUT_FILENO);
				close(fd[1]);
			}
			apply_redirs(cmd->redirs);

			if (is_builtin(cmd->argv[0]))
				exit(exec_builtin(shell, cmd->argv));

			// char **envp = env_to_envp(shell->env);
			execvp(cmd->argv[0], cmd->argv);
			perror("exec");
			exit(1);
		}
		else
		{
			pids[pid_count++] = pid;
			if (prev_fd != -1)
				close(prev_fd);
			if (cmd->pipe_next)
			{
				close(fd[1]); // не пишем
				prev_fd = fd[0];
			}
		}
		cmd = cmd->pipe_next;
	}

	for (int i = 0; i < pid_count; i++)
	{
		int status;
		waitpid(pids[i], &status, 0);
		if (i == pid_count - 1)
			shell->last_status = WEXITSTATUS(status);
	}
}

// Выполнение одиночной команды без пайпа
static void	run_single(t_cmd *cmd, t_shell *shell)
{
	if (!cmd || !cmd->argv || !cmd->argv[0])
		return;

	if (is_builtin(cmd->argv[0]))
	{
		shell->last_status = exec_builtin(shell, cmd->argv);
		return;
	}

	pid_t pid = fork();
	if (pid == 0)
	{
		apply_redirs(cmd->redirs);

		// char **envp = env_to_envp(shell->env);
		execvp(cmd->argv[0], cmd->argv);
		perror("exec");
		exit(1);
	}
	else
	{
		int status;
		waitpid(pid, &status, 0);
		shell->last_status = WEXITSTATUS(status);
	}
}

// Главный обработчик команд
void	execute_cmd(t_shell *shell, t_cmd *cmd)
{
	while (cmd)
	{
		if (cmd->pipe_next)
			run_pipeline(cmd, shell);
		else
			run_single(cmd, shell);

		if (cmd->next_type == TOKEN_AND && shell->last_status != 0)
			break;
		if (cmd->next_type == TOKEN_OR && shell->last_status == 0)
			break;

		cmd = cmd->next;
	}
}