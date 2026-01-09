/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjesione < sjesione@student.42warsaw.pl    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 15:16:12 by sjesione          #+#    #+#             */
/*   Updated: 2026/01/09 17:14:07 by sjesione         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/shell.h"

// int	identify_builtins(char **cmd, char *cmd_path, char **env_arr)
int	identify_builtins(t_ast *node, t_env *env)
{
	char	**cmd;
	int		exit_status;
	int		i;
	char	*expanded;

	cmd = list_to_argv(node->argv);
	exit_status = 0;
	i = 0;
	while (cmd[i])
	{
		expanded = expand_variables(cmd[i], env);
		cmd[i] = expanded;
		i++;
	}
	if (ft_strncmp(cmd[0], "echo", 5) == 0)
		exit_status = echo_builtin(cmd);
	else if (ft_strncmp(cmd[0], "cd", 3) == 0)
		exit_status = cd_builtin(cmd, env);
	else if (ft_strncmp(cmd[0], "pwd", 4) == 0)
		exit_status = pwd_builtin();
	else if (ft_strncmp(cmd[0], "export", 7) == 0)
		exit_status = export_builtin(cmd, env);
	else if (ft_strncmp(cmd[0], "unset", 6) == 0)
		exit_status = unset_builtin(cmd, env);
	else if (ft_strncmp(cmd[0], "env", 4) == 0)
		exit_status = env_builtin(env);
	else if (ft_strncmp(cmd[0], "exit", 5) == 0)
		exit_status = exit_builtin(cmd, env);
	i = 0;
	while (cmd[i])
	{
		free(cmd[i]);
		i++;
	}
	free(cmd);
	return (exit_status);
}

int	echo_builtin(char **argv)
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

int	pwd_builtin(void)
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

int	env_builtin(t_env *env)
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

void	update_wd_env(t_env *env, char *key, char *new_val)
{
	while (env)
	{
		if (ft_strncmp(env->key, key, ft_strlen(key) + 1) == 0)
		{
			if (env->val)
				free(env->val);
			env->val = ft_strdup(new_val);
			return ;
		}
		env = env->next;
	}
}
