/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjesione < sjesione@student.42warsaw.pl    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 18:40:07 by sjesione          #+#    #+#             */
/*   Updated: 2024/12/29 18:06:41 by sjesione         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	counter(int n)
{
	int	count;

	count = 1;
	if (n < 0)
		count++;
	while (n / 10 != 0)
	{
		n = n / 10;
		count++;
	}
	return (count);
}

char	*ft_itoa(int n)
{
	char	*number;
	int		val;
	int		count;
	long	nbr;

	val = 1;
	nbr = n;
	if (nbr < 0)
		val = -1;
	count = counter(nbr);
	number = (char *)ft_calloc(count + 1, sizeof(char));
	if (!number)
		return (number);
	nbr = nbr * val;
	number[count] = '\0';
	while (count > 0 && nbr >= 0)
	{
		count--;
		number[count] = nbr % 10 + '0';
		nbr = nbr / 10;
	}
	if (val == -1)
		number[0] = '-';
	return (number);
}
