#include <stdio.h>
#include <stdlib.h>

void	find(int *set, int size, int *subset, int subsize, int i, int sum,
		int target)
{
    if (sum == target)
	{
		for (int j = 0; j < subsize; j++)
			printf("%d%c", subset[j], j == subsize - 1 ? '\n' : ' ');
		return ;
	}
	for (int j = i; j < size; j++)
	{
		subset[subsize] = set[j];
		find(set, size, subset, subsize + 1, j + 1, sum + set[j], target);
	}
}

int	main(int ac, char **av)
{
	if (ac > 4)
	{
		int	size = ac - 2;
		int	*set = malloc(sizeof(int) * size);
		int	*subset = malloc(sizeof(int) * size);
		if (!set || !subset)
			return (1);
		for (int i = 0; i < size; i++)
			set[i] = atoi(av[i + 2]);
		for (int i = 0; i < size - 1; i++)
			for (int j = 0; j < size - i - 1; j++)
				if (set[j] > set[j + 1])
				{
					int	tmp = set[j];
					set[j] = set[j + 1];
					set[j + 1] = tmp;
				}
		find(set, size, subset, 0, 0, 0, atoi(av[1]));
		free(set);
		free(subset);
		return (0);
	}
	return (1);
}
