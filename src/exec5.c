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

// Helper to count and collect all redirections in the chain
// Returns the actual command node and stores redirections in an array
static t_ast	*collect_redirs(t_ast *node, t_ast **redirs_arr, int *redir_count)
{
	if (!node)
		return (NULL);
	if (node->node_type != NODE_REDIR)
		return (node);
	
	// Recursively collect from left_node first
	t_ast *cmd = collect_redirs(node->left_node, redirs_arr, redir_count);
	
	// Add this redirection to our array (in order, left to right)
	redirs_arr[*redir_count] = node;
	(*redir_count)++;
	
	return (cmd);
}

// Helper to apply a single redirection and return the saved FD
static int	apply_single_redir(t_ast *redir_node)
{
	int fd;
	int original_fd;

	fd = what_fd(redir_node);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(redir_node->file);
		g_exit_status = 1;
		return (-1);
	}
	if (redir_node->redir_type == REDIR_IN)
	{
		original_fd = dup(STDIN_FILENO);
		dup2(fd, STDIN_FILENO);
	}
	else
	{
		original_fd = dup(STDOUT_FILENO);
		dup2(fd, STDOUT_FILENO);
	}
	close(fd);
	return (original_fd);
}

// Helper to restore a single redirection
static int	restore_single_redir(t_ast *redir_node, int original_fd)
{
	if (redir_node->redir_type == REDIR_IN)
		return (dup2(original_fd, STDIN_FILENO));
	else
		return (dup2(original_fd, STDOUT_FILENO));
}

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
	t_ast	*redirs_arr[100];
	int		redir_count;
	int		saved_fds[100];
	t_ast	*cmd;
	int		i;

	redir_count = 0;
	// Collect all redirections and get the actual command
	cmd = collect_redirs(node, redirs_arr, &redir_count);
	
	// Special case: empty command, no redirections
	if (!is_piped && cmd && cmd->node_type == NODE_CMD
		&& (!cmd->argv || !cmd->argv->str) && redir_count > 0)
	{
		// Apply all redirections
		for (i = 0; i < redir_count; i++)
		{
			saved_fds[i] = apply_single_redir(redirs_arr[i]);
			if (saved_fds[i] == -1)
				return (-1);
		}
		// Execute the command
		order_66(cmd, env, is_piped);
		// Restore in reverse order
		for (i = redir_count - 1; i >= 0; i--)
			restore_single_redir(redirs_arr[i], saved_fds[i]);
		for (i = 0; i < redir_count; i++)
			close(saved_fds[i]);
		return (0);
	}
	
	// Normal case
	// Apply all redirections in order
	for (i = 0; i < redir_count; i++)
	{
		saved_fds[i] = apply_single_redir(redirs_arr[i]);
		if (saved_fds[i] == -1)
			return (-1);
	}
	// Execute the command
	order_66(cmd, env, is_piped);
	// Restore in reverse order
	for (i = redir_count - 1; i >= 0; i--)
		restore_single_redir(redirs_arr[i], saved_fds[i]);
	for (i = 0; i < redir_count; i++)
		close(saved_fds[i]);
	return (0);
}
