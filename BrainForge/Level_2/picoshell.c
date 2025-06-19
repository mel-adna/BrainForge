#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int	picoshell(char **cmds[])
{
	int		i = 0, prev = -1, pipefd[2], status, ret = 0;
	pid_t	pid;

	while (cmds[i])
	{
		if (cmds[i + 1] && pipe(pipefd) == -1)
			return (1);
		pid = fork();
		if (pid == 0)
		{
			if (prev != -1)
			{
				dup2(prev, 0);
				close(prev);
			}
			if (cmds[i + 1])
			{
				close(pipefd[0]);
				dup2(pipefd[1], 1);
				close(pipefd[1]);
			}
			execvp(cmds[i][0], cmds[i]);
			_exit(1);
		}
		if (prev != -1)
			close(prev);
		if (cmds[i + 1])
		{
			close(pipefd[1]);
			prev = pipefd[0];
		}
		else
			prev = -1;
		i++;
	}
	while (wait(&status) > 0)
		if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
			ret = 1;
	return (ret);
}

// int main()
// {
// 	char *cmd1[] = {"ls", "-l", NULL};
// 	char *cmd2[] = {"grep", ".c", NULL};
// 	char *cmd3[] = {"sleep", "800", NULL};

// 	char **cmds[] = {cmd1, cmd2, cmd3, NULL};

// 	int result = picoshell(cmds);

// 	if (result == 0)
// 		printf("Commands succeded\n");
// 	else
// 		printf("One or more commands failed\n");
// 	return (result);
// }
