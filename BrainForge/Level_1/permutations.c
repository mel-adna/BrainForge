#include <stdlib.h>
#include <unistd.h>

void	permute(char *s, char *used, char *result, int len, int depth)
{
	if (depth == len)
		return (write(1, result, len), write(1, "\n", 1), (void)0);
	for (int i = 0; i < len; i++)
		if (!used[i])
		{
			used[i] = 1;
			result[depth] = s[i];
			permute(s, used, result, len, depth + 1);
			used[i] = 0;
		}
}

int	main(int ac, char **av)
{
	if (ac == 2)
	{
		int len = 0;
		while (av[1][len])
			len++;
		for (int i = 0; i < len - 1; i++)
			for (int j = i + 1; j < len; j++)
				if (av[1][i] > av[1][j])
				{
					char tmp = av[1][i];
					av[1][i] = av[1][j];
					av[1][j] = tmp;
				}
		char *used = calloc(len, 1);
		char *result = malloc(len);
		if (!used || !result)
			return (1);
		permute(av[1], used, result, len, 0);
		free(used);
		free(result);
	}
	return (0);
}
