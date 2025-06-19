#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

typedef struct json
{
	enum
	{
		MAP,
		INTEGER,
		STRING
	} type;
	union
	{
		struct
		{
			struct pair	*data;
			size_t		size;
		} map;
		int				integer;
		char			*string;
	};
}						json;

typedef struct pair
{
	char				*key;
	json				value;
}						pair;

void					free_json(json j);
int						argo(json *dst, FILE *stream);
json					parse_map(FILE *stream);
int						parse_error;

int	peek(FILE *stream)
{
	int	c;

	c = getc(stream);
	ungetc(c, stream);
	return (c);
}

void	unexpected(FILE *stream)
{
	if (peek(stream) != EOF)
		printf("unexpected token '%c'\n", peek(stream));
	else
		printf("unexpected end of input\n");
	parse_error = 1;
}

int	accept(FILE *stream, char c)
{
	if (peek(stream) == c)
	{
		(void)getc(stream);
		return (1);
	}
	return (0);
}

int	expect(FILE *stream, char c)
{
	if (accept(stream, c))
		return (1);
	unexpected(stream);
	return (0);
}

void	free_json(json j)
{
	switch (j.type)
	{
	case MAP:
		for (size_t i = 0; i < j.map.size; i++)
		{
			free(j.map.data[i].key);
			free_json(j.map.data[i].value);
		}
		free(j.map.data);
		break ;
	case STRING:
		free(j.string);
		break ;
	default:
		break ;
	}
}

void	serialize(json j)
{
	switch (j.type)
	{
	case INTEGER:
		printf("%d", j.integer);
		break ;
	case STRING:
		putchar('"');
		for (int i = 0; j.string[i]; i++)
		{
			if (j.string[i] == '\\' || j.string[i] == '"')
				putchar('\\');
			putchar(j.string[i]);
		}
		putchar('"');
		break ;
	case MAP:
		putchar('{');
		for (size_t i = 0; i < j.map.size; i++)
		{
			if (i != 0)
				putchar(',');
			serialize((json){.type = STRING, .string = j.map.data[i].key});
			putchar(':');
			serialize(j.map.data[i].value);
		}
		putchar('}');
		break ;
	}
}

json parse_int(FILE *stream)
{
	json j;
	j.type = INTEGER;
	j.integer = 0;
	while (isdigit(peek(stream)))
		j.integer = j.integer * 10 + (getc(stream) - '0');
	return j;
}

json parse_string(FILE *stream)
{
	json j;
	j.type = STRING;
	if (!expect(stream, '"'))
	{
		j.string = strdup("");
		return j;
	}
	char buffer[4096];
	int i = 0;
	while (1)
	{
		int c = getc(stream);
		if (c == EOF)
		{
			unexpected(stream);
			j.string = strdup("");
			return j;
		}
		if (c == '"')
			break;
		if (c == '\\')
		{
			c = getc(stream);
			if (c == EOF)
			{
				unexpected(stream);
				j.string = strdup("");
				return j;
			}
		}
		if (i < 4095)
			buffer[i++] = c;
	}
	buffer[i] = '\0';
	j.string = strdup(buffer);
	return j;
}

json parser(FILE *stream)
{
	if (isdigit(peek(stream)))
		return parse_int(stream);
	if (peek(stream) == '"')
		return parse_string(stream);
	if (peek(stream) == '{')
		return parse_map(stream);
	unexpected(stream);
	json j = {.type = STRING, .string = strdup("")};
	return j;
}

json parse_map(FILE *stream)
{
	json j;
	j.type = MAP;
	if (!expect(stream, '{'))
	{
		j.map.data = NULL;
		j.map.size = 0;
		return j;
	}
	pair *items = NULL;
	size_t size = 0, capacity = 0;

	if (accept(stream, '}'))
	{
		j.map.data = items;
		j.map.size = size;
		return j;
	}

	while (1)
	{
		if (size == capacity)
		{
			capacity = capacity == 0 ? 2 : capacity * 2;
			items = realloc(items, capacity * sizeof(pair));
		}
		json key = parse_string(stream);
		if (!expect(stream, ':'))
			break;
		json value = parser(stream);
		items[size].key = key.string;
		items[size].value = value;
		size++;
		if (accept(stream, '}'))
			break;
		if (!expect(stream, ','))
			break;
	}
	j.map.data = items;
	j.map.size = size;
	return j;
}

int argo(json *dst, FILE *stream)
{
	parse_error = 0;
	*dst = parser(stream);
	if (parse_error)
		return -1;
	if (peek(stream) != EOF)
		return (unexpected(stream), -1);
	return 1;
}

int	main(int argc, char **argv)
{
	char	*filename;
	FILE	*stream;
	json	file;

	if (argc != 2)
		return (1);
	filename = argv[1];
	stream = fopen(filename, "r");
	if (argo(&file, stream) != 1)
	{
		free_json(file);
		return (1);
	}
	serialize(file);
	printf("\n");
}
