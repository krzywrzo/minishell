/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjesione < sjesione@student.42warsaw.pl    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 13:08:08 by sjesione          #+#    #+#             */
/*   Updated: 2024/12/29 18:05:19 by sjesione         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	char	*c;
	char	*cd;
	int		i;

	i = 0;
	if (!dest && !src)
		return (NULL);
	c = (char *)src;
	cd = (char *)dest;
	while (n--)
	{
		cd[i] = c[i];
		i++;
	}
	return (cd);
}
