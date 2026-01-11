/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins4.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjesione < sjesione@student.42warsaw.pl    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/11 12:00:00 by sjesione          #+#    #+#             */
/*   Updated: 2026/01/11 17:41:32 by sjesione         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/shell.h"

int		is_numeric(char *str);

static int	exit_code_signal(int code)
{
	return (256 + (code % 256));
}

int	exit_builtin(char **argv, t_env *env, int is_piped)
{
	int	code;
	int	is_interactive;

	(void)env;
	(void)is_piped;
	is_interactive = isatty(STDIN_FILENO);
	if (is_interactive)
		ft_putstr_fd("exit\n", 1);
	if (!argv[1])
		return (exit_code_signal(0));
	if (is_numeric(argv[1]))
	{
		if (argv[2])
		{
			ft_putstr_fd("minishell: exit: too many arguments\n", 2);
			return (1);
		}
		code = ft_atoi(argv[1]);
		return (exit_code_signal(code));
	}
	ft_putstr_fd("minishell: exit: numeric argument required\n", 2);
	return (exit_code_signal(255));
}

int	is_exit_signal(int status)
{
	return (status >= 256);
}

int	get_exit_code(int status)
{
	return (status - 256);
}
