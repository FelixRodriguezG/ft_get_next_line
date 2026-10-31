/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: felixrod <felixrod@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/31 12:06:02 by felixrod          #+#    #+#             */
/*   Updated: 2026/10/31 12:06:10 by felixrod         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_BONUS_H
# define GET_NEXT_LINE_BONUS_H

# include <stdlib.h>
# include <unistd.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1024
# endif

# ifndef FD_MAX
#  define FD_MAX 1024
# endif

/*******************************************************************************
 *							MAIN FUNCTION GET_NEXT_LINE PROTOTYPE
 ******************************************************************************/

char	*get_next_line(int fd);

/*******************************************************************************
 *							HELPER FUNCTIONS PROTOTYPES
 ******************************************************************************/

size_t	ft_strlen(const char *s);
char	*ft_strchr(const char *s, int c);
char	*ft_strndup(const char *s, size_t n);
char	*ft_strjoin_free(char *stash, const char *buf);
char	*ft_free(char **ptr);

#endif
