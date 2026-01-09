/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjesione < sjesione@student.42warsaw.pl    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 13:08:02 by sjesione          #+#    #+#             */
/*   Updated: 2024/12/16 15:27:33 by sjesione         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t			l;
	unsigned char	*l1;
	unsigned char	*l2;

	l1 = (unsigned char *)s1;
	l2 = (unsigned char *)s2;
	l = 0;
	while (l < n)
	{
		if (l1[l] != l2[l])
			return (l1[l] - l2[l]);
		l++;
	}
	return (0);
}
