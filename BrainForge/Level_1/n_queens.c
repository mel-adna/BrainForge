#include <stdio.h>
#include <stdlib.h>

void	solve(int *b, int n, int r)
{
	if (r == n)
	{
		for (int i = 0; i < n; i++)
		{
			fprintf(stdout, "%d", b[i]);
			if (i < n - 1)
				fprintf(stdout, " ");
			else
				fprintf(stdout, "\n");
		}
	}
	for (int c = 0; c < n; c++)
	{
		int ok = 1;
		for (int i = 0; i < r; i++)
		{
			if (b[i] == c || b[i] - i == c - r || b[i] + i == c + r)
				ok = 0;
		}
		if (ok)
		{
			b[r] = c;
			solve(b, n, r + 1);
		}
	}
}

int	main(int ac, char **av)
{
	int n;
	int *b;

	if (ac == 2)
	{
		n = atoi(av[1]);
		if (n <= 0)
			return (0);
		b = malloc(sizeof(int) * n);
		if (!b)
			return (1);
		solve(b, n, 0);
		free(b);
	}
	return (0);
}
