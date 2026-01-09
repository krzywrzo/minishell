/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjesione < sjesione@student.42warsaw.pl    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 22:26:55 by kwrzosek          #+#    #+#             */
/*   Updated: 2026/01/09 15:12:47 by sjesione         ###   ########.fr       */
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
	if (!tree)
		free_ast(tree);
	free_token(head);
	if (order_66(tree, envl) != 0)
	{
		free_ast(tree);
		return (1);
	}
	return (0);
}

int	main(int argc, char **argv, char **env)
{
	char	*input;
	t_env	*envl;

	(void)argc;
	(void)argv;
	envl = init_env(env);
	while (1)
	{
		input = readline("minishell$ ");
		if (!input)
			break ;
		if (handle_input(input, envl))
			return (-1);
	}
	return (0);
}
