/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec4.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjesione < sjesione@student.42warsaw.pl    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 15:17:11 by sjesione          #+#    #+#             */
/*   Updated: 2026/01/09 18:37:05 by sjesione         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/shell.h"

int	exec_cmd(t_ast *node, t_env *env, int is_piped)
{
	char	**env_arr;
	char	**cmd;
	char	*path;
	char	*expanded_cmd;
	int		i;
	char	buffer[4096];
	ssize_t	bytes;

	if (!node->argv || !node->argv->str)
	if (!node->argv || !node->argv->str)
	{
		if (!is_piped)
			return (0);
		while ((bytes = read(STDIN_FILENO, buffer, sizeof(buffer))) > 0)
		{
			write(STDOUT_FILENO, buffer, bytes);
		}
		exit(0);
	}
	if (is_builtin(node->argv->str) == 1)
	{
		identify_builtins(node, env, is_piped);
		return (0);
	}
	else
	{
		if (is_piped)
		{
			child_signals();
			env_arr = convert_list_to_arr(env);
			cmd = list_to_argv(node->argv);
			if (!env_arr || !cmd)
			{
				if (env_arr)
					free_ast_argv(env_arr);
				if (cmd)
					free(cmd);
				return (0);
			}
			path = NULL;
			i = 0;
			while (cmd[i])
			{
				expanded_cmd = expand_variables(cmd[i], env);
				free(cmd[i]);
				cmd[i] = expanded_cmd;
				i++;
			}
			if (cmd[0] && !is_absolute_relative(cmd[0]))
				path = get_path(cmd[0], env_arr);
			if (cmd[0] && is_absolute_relative(cmd[0]))
				run_non_standard(cmd, env_arr);
			if (path)
				execve(path, cmd, env_arr);
			if (cmd[0])
				err_putstr(cmd[0]);
			if (path)
				free(path);
			free(cmd);
			free_ast_argv(env_arr);
			exit(127);
		}
		else
			fork_and_run(node, env);
	}
	return (0);
}

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strncmp(cmd, "echo", 5) == 0)
		return (1);
	else if (ft_strncmp(cmd, "cd", 3) == 0)
		return (1);
	else if (ft_strncmp(cmd, "pwd", 4) == 0)
		return (1);
	else if (ft_strncmp(cmd, "export", 7) == 0)
		return (1);
	else if (ft_strncmp(cmd, "unset", 6) == 0)
		return (1);
	else if (ft_strncmp(cmd, "env", 4) == 0)
		return (1);
	else if (ft_strncmp(cmd, "exit", 5) == 0)
		return (1);
	else
		return (0);
}

void	err_putstr(char *cmd)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": command not found\n", 2);
}

void	child_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}
