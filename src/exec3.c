/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjesione < sjesione@student.42warsaw.pl    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 13:33:27 by sjesione          #+#    #+#             */
/*   Updated: 2026/01/11 17:41:32 by sjesione         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/shell.h"

static void	expand_cmd_args(char **cmd, t_env *env)
{
	char	*expanded_cmd;
	int		i;

	i = 0;
	while (cmd[i])
	{
		expanded_cmd = process_string_with_quotes(cmd[i], env);
		free(cmd[i]);
		cmd[i] = expanded_cmd;
		i++;
	}
}

static void	child_process(t_ast *node, t_env *env, char **cmd, char **env_arr)
{
	char	*path;

	child_signals();
	expand_cmd_args(cmd, env);
	path = NULL;
	if (cmd[0] && !is_builtin(cmd[0]) && !is_absolute_relative(cmd[0]))
		path = get_path(cmd[0], env_arr);
	if (cmd[0] && is_builtin(cmd[0]))
	{
		identify_builtins(node, env, 0);
		exit(0);
	}
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
	free_env(env);
	exit(127);
}

static void	handle_status(pid_t pid)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		g_exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		g_exit_status = 128 + WTERMSIG(status);
		if (WTERMSIG(status) == SIGQUIT)
			ft_putstr_fd("Quit: 3\n", 1);
		else if (WTERMSIG(status) == SIGINT)
			ft_putstr_fd("\n", 1);
	}
	signal(SIGINT, SIG_DFL);
}

void	fork_and_run(t_ast *node, t_env *env)
{
	pid_t	pid;
	char	**cmd;
	char	**env_arr;

	if (!node->argv || !node->argv->str)
		return ;
	env_arr = convert_list_to_arr(env);
	cmd = list_to_argv(node->argv);
	if (!env_arr || !cmd)
	{
		free_ast_argv(env_arr);
		if (cmd)
			free(cmd);
		return ;
	}
	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid == -1)
		perror("minishell: fork");
	else if (pid == 0)
		child_process(node, env, cmd, env_arr);
	else
		handle_status(pid);
	free(cmd);
	free_ast_argv(env_arr);
}
