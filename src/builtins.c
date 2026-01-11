/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjesione < sjesione@student.42warsaw.pl    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 15:16:12 by sjesione          #+#    #+#             */
/*   Updated: 2026/01/11 17:41:32 by sjesione         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/shell.h"

static int	echo_builtin(char **argv)
{
	int	i;
	int	nl_flag;

	i = 1;
	nl_flag = 1;
	while (argv[i] && ft_strncmp(argv[i], "-n", 3) == 0)
	{
		nl_flag = 0;
		i++;
	}
	while (argv[i])
	{
		ft_putstr_fd(argv[i], 1);
		if (argv[i + 1])
			ft_putchar_fd(' ', 1);
		i++;
	}
	if (nl_flag)
		ft_putchar_fd('\n', 1);
	return (0);
}

static int	pwd_builtin(void)
{
	char	cwd[1024];

	if (getcwd(cwd, sizeof(cwd)))
	{
		ft_putstr_fd(cwd, 1);
		ft_putchar_fd('\n', 1);
		return (0);
	}
	perror("minishell: pwd");
	return (1);
}

static int	env_builtin(t_env *env)
{
	while (env)
	{
		if (env->val)
		{
			ft_putstr_fd(env->key, 1);
			ft_putchar_fd('=', 1);
			ft_putstr_fd(env->val, 1);
			ft_putchar_fd('\n', 1);
		}
		env = env->next;
	}
	return (0);
}

static void	exec_identified_builtin(char **cmd, t_ast *node, t_env *env,
		int is_piped)
{
	if (ft_strncmp(cmd[0], "echo", 5) == 0)
		g_exit_status = echo_builtin(cmd);
	else if (ft_strncmp(cmd[0], "cd", 3) == 0)
		g_exit_status = cd_builtin(cmd, env);
	else if (ft_strncmp(cmd[0], "pwd", 4) == 0)
		g_exit_status = pwd_builtin();
	else if (ft_strncmp(cmd[0], "export", 7) == 0)
		g_exit_status = export_builtin(cmd, env);
	else if (ft_strncmp(cmd[0], "unset", 6) == 0)
		g_exit_status = unset_builtin(cmd, env);
	else if (ft_strncmp(cmd[0], "env", 4) == 0)
		g_exit_status = env_builtin(env);
	else if (ft_strncmp(cmd[0], "exit", 5) == 0)
		g_exit_status = exit_builtin(cmd, env, is_piped);
	else
		g_exit_status = 0;
}

int	identify_builtins(t_ast *node, t_env *env, int is_piped)
{
	char	**cmd;
	int		i;
	char	*expanded;

	cmd = list_to_argv(node->argv);
	if (!cmd || !cmd[0])
		return (0);
	i = 0;
	while (cmd[i])
	{
		expanded = process_string_with_quotes(cmd[i], env);
		cmd[i] = expanded;
		i++;
	}
	exec_identified_builtin(cmd, node, env, is_piped);
	i = 0;
	while (cmd[i])
	{
		free(cmd[i]);
		i++;
	}
	free(cmd);
	return (g_exit_status);
}
