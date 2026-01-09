/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjesione < sjesione@student.42warsaw.pl    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 13:33:27 by sjesione          #+#    #+#             */
/*   Updated: 2026/01/09 13:42:52 by sjesione         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/shell.h"

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

void	fork_and_run(t_ast *node, t_env *env)
{
	pid_t	pid;
	int		status;
	char	**cmd;
	char	*cmd_path;
	char	**env_arr;

	//	TODO: signal handling
	if (!node->argv || !node->argv->str)
		return ;
	env_arr = convert_list_to_arr(env);
	if (!env_arr)
		return ;
	cmd = list_to_argv(node->argv);
	if (!cmd)
	{
		free_ast_argv(env_arr);
		return ;
	}
	cmd_path = NULL;
	if (cmd[0] && !is_builtin(cmd[0]) && !is_absolute_relative(cmd[0]))
		cmd_path = get_path(cmd[0], env_arr);
	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid == -1)
	{
		perror("minishell: fork");
		free_ast_argv(env_arr);
		free_ast_argv(cmd);
		if (cmd_path)
			free(cmd_path);
		return ;
	}
	if (pid == 0)
	{
		child_signals();
		if (cmd[0] && is_builtin(cmd[0]) == 1)
		{
			identify_builtins(node, env);
			exit(0);
		}
		if (cmd[0] && is_absolute_relative(cmd[0]) == 1)
			run_non_standard(cmd, env_arr);
		if (cmd_path)
			execve(cmd_path, cmd, env_arr);
		if (cmd[0])
			err_putstr(cmd[0]);
		exit(127);
	}
	else
	{
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
		signal(SIGINT, handle_sigint);
		free_ast_argv(cmd);
		free_ast_argv(env_arr);
		if (cmd_path)
			free(cmd_path);
	}
}

void	run_non_standard(char **argv, char **envp)
{
	char	*path;

	path = argv[0];
	if (access(path, F_OK) == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		exit(127);
	}
	if (access(path, X_OK) == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": Permission denied\n", 2);
		exit(126);
	}
	execve(path, argv, envp);
	perror("execve");
	exit(1);
}

int	is_absolute_relative(char *cmd)
{
	if (!cmd || !cmd[0])
		return (0);
	if (ft_strchr(cmd, '/'))
		return (1);
	return (0);
}
