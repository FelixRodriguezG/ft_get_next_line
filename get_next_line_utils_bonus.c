/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: felixrod <felixrod@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/31 12:21:22 by felixrod          #+#    #+#             */
/*   Updated: 2026/10/31 12:21:29 by felixrod         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

char	*ft_strchr(const char *s, int c)
{
	size_t	i;

	if (!s)
		return (NULL);
	i = 0;
	while (s[i])
	{
		if ((char)s[i] == (char)c)
			return ((char *)&s[i]);
		i++;
	}
	if ((char)s[i] == (char)c)
		return ((char *)&s[i]);
	return (NULL);
}

size_t	ft_strlen(const char *s)
{
	size_t	len;

	if (!s)
		return (0);
	len = 0;
	while (s[len])
		len++;
	return (len);
}

char	*ft_free(char **ptr)
{
	free(*ptr);
	*ptr = NULL;
	return (NULL);
}

char	*ft_strndup(const char *s, size_t n)
{
	size_t	i;
	char	*dup_str;

	dup_str = (char *)malloc(sizeof(char) * (n + 1));
	if (!dup_str)
		return (NULL);
	i = 0;
	while (i < n && s[i])
	{
		dup_str[i] = s[i];
		i++;
	}
	dup_str[i] = '\0';
	return (dup_str);
}

char	*ft_strjoin_free(char *stash, const char *buf)
{
	size_t	new_len;
	char	*new_stash;
	size_t	i;
	size_t	j;

	new_len = ft_strlen(stash) + ft_strlen(buf);
	new_stash = (char *)malloc(sizeof(char) * (new_len + 1));
	if (!new_stash)
		return (ft_free(&stash));
	i = 0;
	while (stash && stash[i])
	{
		new_stash[i] = stash[i];
		i++;
	}
	j = 0;
	while (buf[j])
		new_stash[i++] = buf[j++];
	new_stash[i] = '\0';
	free(stash);
	return (new_stash);
}
