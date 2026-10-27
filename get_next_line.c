/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: felixrod <felixrod@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/26 22:41:05 by felixrod          #+#    #+#             */
/*   Updated: 2026/10/27 09:39:26 by felixrod         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*ft_strchr(const char *s, int c)
{
	size_t	i;

	if (!s)
		return (NULL);
	i = 0;
	while (s[i])
	{
		if ((char)s[i] == (unsigned char)c)
			return ((char *)&s[i]);
		i++;
	}
	if ((char)s[i] == (char)c)
		return ((char *)&s[i]);
	return (NULL);
}

static char	*ft_free(char **ptr)
{
	free(*ptr);
	*ptr = NULL;
	return (NULL);
}

static char *ft_read to_stash(int fd, char *stash)
{
	char	*buf;
	ssize_t	bytes;

	buf = malloc(sizeof(char) * BUFFER_SIZE + 1);
	if (!buf)
		return (ft_free(&stash));
	bytes = 1;
	while (bytes > 0 && !ft_strchr(stash, '\n'))
	{
		bytes =  read(fd, buf, BUFFER_SIZE);
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


char	*get_next_line(int fd)
{
	static char	*stash;
	ssize_t		bytes;

	if (ft_strchr(stash, '\n')
		bytes = read(fd, stash, BUFFER_SIZE);
}
