/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjesione < sjesione@student.42warsaw.pl    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 17:02:22 by sjesione          #+#    #+#             */
/*   Updated: 2024/12/29 18:15:59 by sjesione         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*join;
	size_t	s1_len;
	size_t	s2_len;
	size_t	i;

	if (!s1 || !s2)
		return (NULL);
	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	join = ft_calloc((s1_len + s2_len + 1), sizeof(char));
	if (!join)
		return (NULL);
	i = -1;
	while (s1[++i])
	{
		join[i] = s1[i];
	}
	i = -1;
	while (s2[++i])
	{
		join[i + s1_len] = s2[i];
	}
	join[i + s1_len] = '\0';
	return (join);
}
