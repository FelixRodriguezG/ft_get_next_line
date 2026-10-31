/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: felixrod <felixrod@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/31 12:20:37 by felixrod          #+#    #+#             */
/*   Updated: 2026/10/31 12:21:01 by felixrod         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

static char	*ft_read_to_stash(int fd, char *stash)
{
	char	*buf;
	ssize_t	bytes;

	buf = malloc(sizeof(char) * BUFFER_SIZE + 1);
	if (!buf)
		return (ft_free(&stash));
	bytes = 1;
	while (bytes > 0 && !ft_strchr(stash, '\n'))
	{
		bytes = read(fd, buf, BUFFER_SIZE);
		if (bytes == -1)
		{
			free(buf);
			return (ft_free(&stash));
		}
		buf[bytes] = '\0';
		stash = ft_strjoin_free(stash, buf);
		if (!stash)
			break ;
	}
	free(buf);
	if (stash && !*stash)
		return (ft_free(&stash));
	return (stash);
}

static char	*ft_extract_line(char *stash)
{
	size_t	len;

	len = 0;
	while (stash[len] && stash[len] != '\n')
		len++;
	if (stash[len] == '\n')
		len++;
	return (ft_strndup(stash, len));
}

static char	*ft_save_rest(char *stash)
{
	char	*nl;
	char	*start_rest;
	char	*rest;
	size_t	rest_len;

	nl = ft_strchr(stash, '\n');
	if (!nl || !*(nl + 1))
		return (ft_free(&stash));
	start_rest = nl + 1;
	rest_len = ft_strlen(start_rest);
	rest = ft_strndup(start_rest, rest_len);
	free(stash);
	return (rest);
}

char	*get_next_line(int fd)
{
	static char	*stash[FD_MAX];
	char		*line;

	if (fd < 0 || fd >= FD_MAX || BUFFER_SIZE <= 0)
		return (NULL);
	stash[fd] = ft_read_to_stash(fd, stash[fd]);
	if (!stash[fd])
		return (NULL);
	line = ft_extract_line(stash[fd]);
	if (!line)
		return (ft_free(&stash[fd]));
	stash[fd] = ft_save_rest(stash[fd]);
	return (line);
}
