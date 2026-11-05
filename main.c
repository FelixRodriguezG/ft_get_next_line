/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: felixrod <felixrod@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/31 12:03:57 by felixrod          #+#    #+#             */
/*   Updated: 2026/10/31 12:04:57 by felixrod         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdio.h>
#include "get_next_line.h"

/*
 * Uso:
 *   ./gnl              lee de stdin (Ctrl+D para terminar)
 *   ./gnl test.txt     lee del fichero
 *   ./gnl a.txt b.txt  lee varios ficheros, uno detrás de otro
 *
 * Cada línea se imprime numerada y entre comillas, con los '\n'
 * escapados para ver exactamente qué devuelve get_next_line.
 * Este main NO se entrega.
 */

static void	print_line(int n, const char *line)
{
	printf("[%d] \"", n);
	while (*line)
	{
		if (*line == '\n')
			printf("\\n");
		else if (*line == '\t')
			printf("\\t");
		else
			putchar(*line);
		line++;
	}
	printf("\"\n");
}

static void	read_all(int fd, const char *name)
{
	char	*line;
	int		n;

	printf("=== %s (BUFFER_SIZE=%d) ===\n", name, BUFFER_SIZE);
	n = 1;
	line = get_next_line(fd);
	while (line)
	{
		print_line(n++, line);
		free(line);
		line = get_next_line(fd);
	}
	printf("[%d] NULL\n\n", n);
}

static void	test_invalid_fds(void)
{
	char	*line;

	printf("=== fds inválidos (todos deben dar NULL) ===\n");
	line = get_next_line(-1);
	printf("fd -1  -> %s\n", line ? "NO NULL (mal)" : "NULL");
	free(line);
	line = get_next_line(42);
	printf("fd 42  -> %s\n", line ? "NO NULL (mal)" : "NULL");
	free(line);
}

int	main(int argc, char **argv)
{
	int	fd;
	int	i;

	if (argc == 1)
		read_all(0, "stdin");
	i = 1;
	while (i < argc)
	{
		fd = open(argv[i], O_RDONLY);
		if (fd == -1)
			perror(argv[i]);
		else
		{
			read_all(fd, argv[i]);
			close(fd);
		}
		i++;
	}
	test_invalid_fds();
	return (0);
}
