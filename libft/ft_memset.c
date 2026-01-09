/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjesione < sjesione@student.42warsaw.pl    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 13:08:16 by sjesione          #+#    #+#             */
/*   Updated: 2024/12/29 18:07:28 by sjesione         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *ptr, int val, size_t n)
{
	char	*c;
	int		i;

	i = 0;
	c = (char *)ptr;
	while (n--)
	{
		c[i] = val;
		i++;
	}
	return (c);
}
