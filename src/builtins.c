/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwrzosek <kwrzosek@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/27 18:00:48 by kwrzosek          #+#    #+#             */
/*   Updated: 2026/01/05 15:10:14 by kwrzosek         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/shell.h"

// int	identify_builtins(char **cmd, char *cmd_path, char **env_arr)
int identify_builtins(t_ast *node, t_env *env)
{
    char    **cmd;
    int     exit_status;

    cmd = list_to_argv(node->argv);
    exit_status = 0;
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
    free_ast_argv(cmd);
    return (exit_status);
}

int echo_builtin(char **argv)
{
    int i;
    int nl_flag;

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

int pwd_builtin(void)
{
    char    cwd[1024];

    if (getcwd(cwd, sizeof(cwd)))
    {
        ft_putstr_fd(cwd, 1);
        ft_putchar_fd('\n', 1);
        return (0);
    }
    perror("minishell: pwd");
    return (1);
}

int env_builtin(t_env *env)
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

static void update_wd_env(t_env *env, char *key, char *new_val)
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

static char *get_env_val(t_env *env, char *key)
{
    while (env)
    {
        if (ft_strncmp(env->key, key, ft_strlen(key) + 1) == 0)
            return (env->val);
        env = env->next;
    }
    return (NULL);
}

int cd_builtin(char **argv, t_env *env)
{
    char    *path;
    char    cwd[1024];
    char    old_cwd[1024];

    if (!argv[1])
    {
        path = get_env_val(env, "HOME");
        if (!path)
        {
            ft_putstr_fd("minishell: cd: HOME not set\n", 2);
            return (1);
        }
    }
    else
        path = argv[1];
    if (!getcwd(old_cwd, sizeof(old_cwd)))
        perror("minishell: cd: getcwd failed");
    if (chdir(path) == -1)
    {
        ft_putstr_fd("minishell: cd: ", 2);
        ft_putstr_fd(path, 2);
        perror(" ");
        return (1);
    }
    if (getcwd(cwd, sizeof(cwd)))
    {
        update_wd_env(env, "OLDPWD", old_cwd);
        update_wd_env(env, "PWD", cwd);
    }
    return (0);
}

static int is_valid_key(char *key)
{
    int i;

    if (!key || (!ft_isalpha(key[0]) && key[0] != '_'))
        return (0);
    i = 1;
    while (key[i])
    {
        if (!ft_isalnum(key[i]) && key[i] != '_')
            return (0);
        i++;
    }
    return (1);
}

int export_builtin(char **argv, t_env *env)
{
    int     i;
    char    *key;
    char    *val;
    char    *equal_sign;

    i = 1;
    if (!argv[1])
        return (env_builtin(env));
    while (argv[i])
    {
        equal_sign = ft_strchr(argv[i], '=');
        if (equal_sign)
        {
            key = ft_substr(argv[i], 0, equal_sign - argv[i]);
            val = ft_strdup(equal_sign + 1);
        }
        else
        {
            key = ft_strdup(argv[i]);
            val = NULL;
        }
        if (!is_valid_key(key))
        {
            ft_putstr_fd("minishell: export: not a valid identifier\n", 2);
            free(key);
            if (val) free(val);
            return (1); 
        }
        add_or_update_env(&env, key, val);
        i++;
    }
    return (0);
}

int unset_builtin(char **argv, t_env *env)
{
    int i;
	int	list_len;

	list_len = list_size(env);
    i = 1;
    while (argv[i])
    {
		if (i < list_len)
			return (1);
		delete_node(&env, argv[i]);
        i++;
    }
    return (0);
}

static int is_numeric(char *str)
{
    int i = 0;
    if (str[i] == '-' || str[i] == '+')
        i++;
    while (str[i])
    {
        if (!ft_isdigit(str[i]))
            return (0);
        i++;
    }
    return (1);
}

int exit_builtin(char **argv, t_env *env)
{
    ft_putstr_fd("exit\n", 1);
    if (!argv[1])
    {
		free_env(env);
        exit(0);
    }
    if (is_numeric(argv[1]))
    {
        if (argv[2])
        {
            ft_putstr_fd("minishell: exit: too many arguments\n", 2);
            return (1);
        }
        int code = ft_atoi(argv[1]);
		free_env(env);
        exit(code % 256);
    }
    ft_putstr_fd("minishell: exit: numeric argument required\n", 2);
    free_env(env);
    exit(255);
}
