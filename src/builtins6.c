/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins6.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjesione < sjesione@student.42warsaw.pl    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/11 12:00:00 by sjesione          #+#    #+#             */
/*   Updated: 2026/01/11 17:41:32 by sjesione         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/shell.h"

int		process_export_arg(char *arg, t_env *env);
int		env_list_len(t_env *env);
void	sort_env_array(t_env **sorted, int len);
void	print_sorted_array(t_env **sorted, int len);

static void	print_export_list(t_env *env)
{
	t_env	**sorted;
	int		len;
	int		i;

	len = env_list_len(env);
	if (len == 0)
		return ;
	sorted = malloc(sizeof(t_env *) * len);
	if (!sorted)
		return ;
	i = 0;
	while (env)
	{
		sorted[i++] = env;
		env = env->next;
	}
	sort_env_array(sorted, len);
	print_sorted_array(sorted, len);
	free(sorted);
}

int	export_builtin(char **argv, t_env *env)
{
	int	i;
	int	status;

	i = 1;
	status = 0;
	if (!argv[1])
	{
		print_export_list(env);
		return (0);
	}
	while (argv[i])
	{
		if (process_export_arg(argv[i], env))
			status = 1;
		i++;
	}
	return (status);
}
