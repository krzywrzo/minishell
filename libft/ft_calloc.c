/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjesione < sjesione@student.42warsaw.pl    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 17:35:52 by sjesione          #+#    #+#             */
/*   Updated: 2024/12/30 17:02:13 by sjesione         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t num, size_t size)
{
	size_t	allsize;
	void	*pointer;

	if (num != 0 && size > SIZE_MAX / num)
		return (NULL);
	allsize = num * size;
	pointer = malloc(allsize);
	if (pointer == NULL)
		return (NULL);
	ft_bzero(pointer, allsize);
	return (pointer);
}
