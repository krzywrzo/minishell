/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec5.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjesione < sjesione@student.42warsaw.pl    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 14:42:49 by sjesione          #+#    #+#             */
/*   Updated: 2026/01/09 18:42:44 by sjesione         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/shell.h"

int	what_fd(t_ast *node)
{
	int	fd;

	fd = -1;
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

int	exec_redir(t_ast *node, t_env *env, int is_piped)
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
	if (!is_piped && node->left_node && node->left_node->node_type == NODE_CMD
		&& (!node->left_node->argv || !node->left_node->argv->str))
	{
		restore_fd(node, original_fd);
		close(original_fd);
		return (0);
	}
	order_66(node->left_node, env, is_piped);
	restore_fd(node, original_fd);
	close(original_fd);
	return (0);
}
