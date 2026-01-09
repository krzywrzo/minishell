/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjesione < sjesione@student.42warsaw.pl    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 20:15:05 by sjesione          #+#    #+#             */
/*   Updated: 2024/12/29 18:06:31 by sjesione         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	trimmer(char c, char const *set)
{
	while (*set)
	{
		if (c == *set)
			return (1);
		set++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*trimmed;
	size_t	start;
	size_t	end;

	start = 0;
	if (!s1 || !set)
		return (NULL);
	end = ft_strlen((char *)s1);
	while (trimmer(s1[start], set) && s1[start])
		start++;
	while (trimmer(s1[end - 1], set) && end > start)
		end--;
	if (start >= end)
		return (ft_calloc(1, sizeof(char)));
	trimmed = (ft_substr(s1, start, end - start));
	return (trimmed);
}
