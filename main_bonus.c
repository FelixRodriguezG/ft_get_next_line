/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: felixrod <felixrod@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/31 13:00:00 by felixrod          #+#    #+#             */
/*   Updated: 2026/10/31 13:00:00 by felixrod         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdio.h>
#include "get_next_line_bonus.h"

/*
 * Uso:
 *   ./gnl_bonus                    usa bonus_a.txt y bonus_b.txt
 *   ./gnl_bonus a.txt b.txt c.txt  alterna entre los ficheros que pases
 *
 * Lee UNA línea de cada fichero por turnos (A, B, C, A, B, C...)
 * hasta que todos devuelven NULL. Si los stashes se mezclan, se ve
 * enseguida: aparecen trozos de un fichero en las líneas de otro.
 * Este main NO se entrega.
 */

#define MAX_FILES 10

static void	print_line(const char *name, const char *line)
{
	printf("%-12s -> ", name);
	if (!line)
	{
		printf("NULL\n");
		return ;
	}
	putchar('"');
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

static int	open_all(int n, char **names, int *fds)
{
	int	i;

	i = 0;
	while (i < n)
	{
		fds[i] = open(names[i], O_RDONLY);
		if (fds[i] == -1)
		{
			perror(names[i]);
			while (--i >= 0)
				close(fds[i]);
			return (0);
		}
		i++;
	}
	return (1);
}

/* Una vuelta: una línea de cada fichero que aún no haya terminado.
 * Devuelve cuántos ficheros siguen vivos. */
static int	one_round(int n, char **names, int *fds, int *done)
{
	char	*line;
	int		alive;
	int		i;

	alive = 0;
	i = 0;
	while (i < n)
	{
		if (!done[i])
		{
			line = get_next_line(fds[i]);
			print_line(names[i], line);
			if (line)
				alive++;
			else
				done[i] = 1;
			free(line);
		}
		i++;
	}
	return (alive);
}

static void	test_invalid_fds(void)
{
	char	*line;

	printf("\n=== fds inválidos (todos deben dar NULL) ===\n");
	line = get_next_line(-1);
	printf("fd -1          -> %s\n", line ? "NO NULL (mal)" : "NULL");
	free(line);
	line = get_next_line(FD_MAX);
	printf("fd FD_MAX (%d) -> %s\n", FD_MAX, line ? "NO NULL (mal)" : "NULL");
	free(line);
	line = get_next_line(FD_MAX + 100);
	printf("fd FD_MAX+100  -> %s\n", line ? "NO NULL (mal)" : "NULL");
	free(line);
}

int	main(int argc, char **argv)
{
	char	*defaults[] = {"test_2.txt", "test_3.txt"};
	char	**names;
	int		fds[MAX_FILES];
	int		done[MAX_FILES] = {0};
	int		n;

	names = defaults;
	n = 2;
	if (argc > 1)
	{
		names = argv + 1;
		n = argc - 1;
	}
	if (n > MAX_FILES)
		return (printf("Máximo %d ficheros\n", MAX_FILES), 1);
	if (!open_all(n, names, fds))
		return (1);
	printf("=== %d ficheros alternados (BUFFER_SIZE=%d) ===\n", n, BUFFER_SIZE);
	while (one_round(n, names, fds, done))
		;
	while (n-- > 0)
		close(fds[n]);
	test_invalid_fds();
	return (0);
}
