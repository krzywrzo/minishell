/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwrzosek <kwrzosek@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/27 17:45:25 by kwrzosek          #+#    #+#             */
/*   Updated: 2026/01/02 15:28:46 by kwrzosek         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/shell.h"

int	order_66(t_ast *root, t_env *env)
{
	if (root->node_type == NODE_PIPE)
	{
		if(exec_pipe(root, env) == -1)	// TODO: exec_pipe
			return(handle_error());
	}
	else if (root->node_type == NODE_REDIR)
	{
		if (exec_redir() == -1)		// TODO: exec_redir
			return(handle_error());
	}
	else
	{
		if (exec_cmd(root, env, 0) == -1)
			return(handle_error());
	}
	return(0);
}

int exec_pipe(t_ast *node, t_env *env)
{
    int     fd[2];
    pid_t   pid_left;
    pid_t   pid_right;
    int     status;

    if (pipe(fd) == -1)
    {
        perror("pipe");
        return (1);
    }
    pid_left = fork();
    if (pid_left == -1)
    {
        perror("fork");
        close(fd[0]);
        close(fd[1]);
        return (1);
    }
    if (pid_left == 0)
    {
        close(fd[0]);
        dup2(fd[1], STDOUT_FILENO);
        close(fd[1]);
        exec_cmd(node->left_node, env, 1); 
        exit(g_exit_status);
    }
    pid_right = fork();
    if (pid_right == -1)
    {
        perror("fork");
        close(fd[0]);
        close(fd[1]);
        return (1);
    }
    if (pid_right == 0)
    {
        close(fd[1]);
        dup2(fd[0], STDIN_FILENO);
        close(fd[0]);
        exec_cmd(node->right_node, env, 1);
        exit(g_exit_status);
    }
    close(fd[0]);
    close(fd[1]);
    waitpid(pid_left, NULL, 0);
    waitpid(pid_right, &status, 0);
    if (WIFEXITED(status))
        g_exit_status = WEXITSTATUS(status);
    return (0);
}

int	exec_redir()
{
	return (0);
}
int	exec_cmd(t_ast *node, t_env *env, int is_piped)
{
	char	**env_arr;
	char	*cmd_path;
	char	**cmd;
	int		exit_code;

	env_arr = convert_list_to_arr(env);
	if (!env_arr)
		return(-1);
	cmd_path = get_path(node->argv->str, env_arr);
	cmd = list_to_argv(node->argv);
	if (is_builtin(cmd) == 1)
	{
		if (is_piped == 0 && is_state_changing(cmd[0]) == 1)
		{
			identify_builtins(cmd, cmd_path, env_arr);
			return (0);
		}
		else
		{
			identify_builtins(cmd, cmd_path, env_arr);
			return (0);
		}
	}
	fork_and_run(cmd, cmd_path, env_arr);
	return (0);
}
int	is_builtin(char **cmd)
{
		if (ft_strncmp(cmd[0], "echo", 4) == 0 && ft_strncmp(cmd[1], "-n", 2) == 0)
		return(1);
	else if (ft_strncmp(cmd[0], "cd", 2) == 0)
		return(1);
	else if (ft_strncmp(cmd[0], "pwd", 3) == 0)
		return (1);
	else if (ft_strncmp(cmd[0], "export", 6) == 0)
		return (1);
	else if (ft_strncmp(cmd[0], "unset", 5) == 0)
		return (1);
	else if (ft_strncmp(cmd[0], "env", 3) == 0)
		return (1);
	else if (ft_strncmp(cmd[0], "exit", 4) == 0)
		return (1);
	else
		return (0);
}

void    fork_and_run(char **cmd, char *cmd_path, char **env)
{
    pid_t   pid;
    int     status;

    pid = fork();
    if (pid == -1)
    {
        perror("minishell: fork");
        return ;
    }
    if (pid == 0)
    {
        if (is_builtin(cmd)) 
        {
            identify_builtins(cmd, cmd_path, env);
            exit(0);
        }
        if (is_absolute_relative(cmd[0]) == 1)
            run_non_standard(cmd, env);
        if (cmd_path)
            execve(cmd_path, cmd, env);
        ft_putstr_fd("minishell: ", 2);
        ft_putstr_fd(cmd[0], 2);
        ft_putstr_fd(": command not found\n", 2);
		exit(127);
    }
    else
    {
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
            g_exit_status = WEXITSTATUS(status);
        free_ast_argv(cmd);
    }
}

void    run_non_standard(char **argv, char **envp)
{
    char        *path;

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

int is_absolute_relative(char *cmd)
{
    if (!cmd || !cmd[0])
        return (0);
    if (ft_strchr(cmd, '/'))
        return (1);

    return (0);
}


int	is_state_changing(char *cmd)
{
	if (ft_strncmp(cmd, "cd", 2) == 0)
		return (1);
	else if (ft_strncmp(cmd, "export", 6) == 0)
		return (1);
	else if (ft_strncmp(cmd, "unset", 5) == 0)
		return (1);
	else if (ft_strncmp(cmd, "exit", 4) == 0)
		return (1);
	return (0);
}

int	handle_error()	//	TODO: handle execve error
{
	return(-1);
}

char *join_env_str(char *key, char *val)
{
    char    *tmp;
    char    *res;

    if (!key || !val)
        return (NULL);
    tmp = ft_strjoin(key, "=");
    if (!tmp)
        return (NULL);
    res = ft_strjoin(tmp, val);
    free(tmp);
    return (res);
}

char    **convert_list_to_arr(t_env *env)
{
    char    **env_arr;
    int     env_size;
    int     i;

    env_size = list_size(env);
    env_arr = malloc(sizeof(char *) * (env_size + 1));
    if (!env_arr)
        return (NULL);
    i = 0;
    while (env)
    {
        env_arr[i] = join_env_str(env->key, env->val);
        if (env_arr[i] == NULL) 
        {
            free_ast_argv(env_arr);
            return (NULL);
        }
        i++;
        env = env->next;
    }
    env_arr[i] = NULL;
    return (env_arr);
}


int	list_size(t_env *env)
{
	int	i;

	while (env)
	{
		i++;
		env = env->next;
	}
	return (i);
}
