/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjesione < sjesione@student.42warsaw.pl    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 13:08:45 by sjesione          #+#    #+#             */
/*   Updated: 2024/12/29 18:07:38 by sjesione         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char	*l;
	int		len;

	l = (char *)s;
	if ((char)c == '\0')
		return (l + ft_strlen(l));
	len = ft_strlen(l);
	while (len--)
	{
		if (l[len] == (char)c)
			return (&l[len]);
	}
	return (NULL);
}
