#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int	ft_popen(const char *file, char *const argv[], char type)
{
	int		pipefd[2];
	pid_t	pid;

	if ((type != 'r' && type != 'w') || pipe(pipefd) == -1)
		return (-1);
	pid = fork();
	if (pid == -1)
		return (close(pipefd[0]), close(pipefd[1]), -1);
	if (pid == 0)
	{
		if (type == 'r')
		{
			close(pipefd[0]);
			if (dup2(pipefd[1], 1) == -1)
				exit(1);
			close(pipefd[1]);
		}
		else
		{
			close(pipefd[1]);
			if (dup2(pipefd[0], 0) == -1)
				exit(1);
			close(pipefd[0]);
		}
		execvp((char *)file, argv);
		exit(1);
	}
	if (type == 'r')
		return (close(pipefd[1]), pipefd[0]);
	else
		return (close(pipefd[0]), pipefd[1]);
}

// int	main(void)
// {
// 	int fd = ft_popen("echo", (char *const[]){"echo", "Hello", NULL}, 'r');
// 	if (fd == -1)
// 	{
// 		perror("ft_popen");
// 		return (1);
// 	}

// 	char buffer[1024];
// 	ssize_t n;
// 	while ((n = read(fd, buffer, sizeof(buffer) - 1)) > 0)
// 	{
// 		buffer[n] = '\0';
// 		write(1, buffer, n);
// 	}
// 	close(fd);
// 	return (0);
// }