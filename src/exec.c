/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjesione < sjesione@student.42warsaw.pl    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/27 17:45:25 by kwrzosek          #+#    #+#             */
/*   Updated: 2026/01/09 13:34:14 by sjesione         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/shell.h"

int	order_66(t_ast *root, t_env *env)
{
	if (!root)
		return (0);
	if (root->node_type == NODE_PIPE)
	{
		if (exec_pipe(root, env) == -1)
			return (handle_error());
	}
	else if (root->node_type == NODE_REDIR)
	{
		if (exec_redir(root, env) == -1)
			return (handle_error());
	}
	else
	{
		if (exec_cmd(root, env, 0) == -1)
			return (handle_error());
	}
	return (0);
}

int	exec_pipe(t_ast *node, t_env *env)
{
	int		fd[2];
	pid_t	pid_left;
	pid_t	pid_right;
	int		status;

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
		// order_66(node->left_node, env);
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
		// order_66(node->right_node, env);
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

int	what_fd(t_ast *node)
{
	int	fd;

	if (node->redir_type == REDIR_IN)
		fd = open(node->file, O_RDONLY);
	else if (node->redir_type == REDIR_OUT)
		fd = open(node->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (node->redir_type == REDIR_APPEND)
		fd = open(node->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	return (fd);
}

int	restore_fd(t_ast *node, int original_fd)
{
	if (node->redir_type == REDIR_IN)
		return (dup2(original_fd, STDIN_FILENO));
	else
		return (dup2(original_fd, STDOUT_FILENO));
}

int	exec_redir(t_ast *node, t_env *env)
{
	int	fd;
	int	original_fd;

	fd = what_fd(node);
	if (fd == -1)
	{
		perror("minishell");
		g_exit_status = 1;
		return (-1);
	}
	if (node->redir_type == REDIR_IN)
		original_fd = dup(STDIN_FILENO);
	else
		original_fd = dup(STDOUT_FILENO);
	if (node->redir_type == REDIR_IN)
		dup2(fd, STDIN_FILENO);
	else
		dup2(fd, STDOUT_FILENO);
	close(fd);
	order_66(node->left_node, env);
	restore_fd(node, original_fd);
	close(original_fd);
	return (0);
}
