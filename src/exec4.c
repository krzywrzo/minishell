/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec4.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjesione < sjesione@student.42warsaw.pl    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 15:17:11 by sjesione          #+#    #+#             */
/*   Updated: 2026/01/11 17:41:32 by sjesione         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/shell.h"

static void	expand_piped_cmd_args(char **cmd, t_env *env)
{
	char	*expanded;
	int		i;

	i = 0;
	while (cmd[i])
	{
		expanded = process_string_with_quotes(cmd[i], env);
		free(cmd[i]);
		cmd[i] = expanded;
		i++;
	}
}

static void	execute_piped_cmd(char **cmd, char **env_arr)
{
	char	*path;

	path = NULL;
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

static void	handle_piped_cmd(t_ast *node, t_env *env)
{
	char	**env_arr;
	char	**cmd;

	child_signals();
	env_arr = convert_list_to_arr(env);
	cmd = list_to_argv(node->argv);
	if (!env_arr || !cmd)
	{
		if (env_arr)
			free_ast_argv(env_arr);
		if (cmd)
			free(cmd);
		return ;
	}
	expand_piped_cmd_args(cmd, env);
	execute_piped_cmd(cmd, env_arr);
}

static void	handle_empty_piped(void)
{
	char	buffer[4096];
	ssize_t	bytes;

	bytes = read(STDIN_FILENO, buffer, sizeof(buffer));
	while (bytes > 0)
	{
		write(STDOUT_FILENO, buffer, bytes);
		bytes = read(STDIN_FILENO, buffer, sizeof(buffer));
	}
	exit(0);
}

int	exec_cmd(t_ast *node, t_env *env, int is_piped)
{
	int		ret;

	if (!node->argv || !node->argv->str)
	{
		if (!is_piped)
			return (0);
		handle_empty_piped();
	}
	if (is_builtin(node->argv->str) == 1)
	{
		ret = identify_builtins(node, env, is_piped);
		if (is_exit_signal(ret))
			return (ret);
		return (0);
	}
	if (is_piped)
		handle_piped_cmd(node, env);
	else
		fork_and_run(node, env);
	return (0);
}
