/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjesione < sjesione@student.42warsaw.pl    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 22:26:55 by kwrzosek          #+#    #+#             */
/*   Updated: 2026/01/11 17:41:32 by sjesione         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/shell.h"

int	g_exit_status = 0;

void	handle_sigint(int sig)
{
	(void)sig;
	ft_putchar_fd('\n', 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	g_exit_status = 1;
}

void	setup_signals(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
}

static int	handle_input(char *input, t_env *envl)
{
	t_token		*head;
	t_ast		*tree;
	t_heredoc	hd;
	int			ret;

	if (ft_strlen(input) > 0)
		add_history(input);
	head = read_input(input);
	free(input);
	if (!head)
		return (0);
	tree = parse_token(head);
	free_token(head);
	if (!tree)
		return (0);
	preread_heredocs(tree, envl, &hd);
	ret = order_66(tree, envl, 0, &hd);
	cleanup_heredocs(&hd);
	free_ast(tree);
	return (ret);
}

static int	main_loop(t_env *envl, int is_tty)
{
	char	*input;
	int		ret;

	while (1)
	{
		input = get_input(is_tty);
		if (!input)
		{
			if (is_tty)
				ft_putstr_fd("exit\n", 1);
			return (g_exit_status);
		}
		ret = handle_input(input, envl);
		if (is_exit_signal(ret))
			return (get_exit_code(ret));
	}
}

int	main(int argc, char **argv, char **env)
{
	t_env	*envl;
	int		ret;

	(void)argc;
	(void)argv;
	setup_signals();
	envl = init_env(env);
	ret = main_loop(envl, isatty(STDIN_FILENO));
	free_env(envl);
	rl_clear_history();
	return (ret);
}
