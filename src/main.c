/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjesione < sjesione@student.42warsaw.pl    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 22:26:55 by kwrzosek          #+#    #+#             */
/*   Updated: 2026/01/09 18:37:05 by sjesione         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/shell.h"

int	g_exit_status = 0;

void	print_env(t_env *envl)
{
	if (!envl)
		printf("List is empty\n");
	while (envl)
	{
		printf("KEY: %s VAL: %s\n", envl->key, envl->val);
		envl = envl->next;
	}
}

static int	handle_input(char *input, t_env *envl)
{
	t_token	*head;
	t_ast	*tree;

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
	if (order_66(tree, envl, 0) != 0)
	{
		free_ast(tree);
		return (1);
	}
	free_ast(tree);
	return (0);
}

/* src/main.c */

// Dodaj potrzebny nagłówek dla rl_clear_history, jeśli go nie ma w shell.h
// #include <readline/history.h> 

void	setup_signals(void);

int	main(int argc, char **argv, char **env)
{
	char	*input;
	t_env	*envl;

	(void)argc;
	(void)argv;
	setup_signals();
	envl = init_env(env);
	while (1)
	{
		input = readline("minishell$ ");
		if (!input)
		{
			ft_putstr_fd("exit\n", 1);
			break ;
		}
		if (handle_input(input, envl))
		{
			free_env(envl);
			rl_clear_history();
			return (-1);
		}
	}
	free_env(envl);
	rl_clear_history();
	return (0);
}
