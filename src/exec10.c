/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec10.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjesione < sjesione@student.42warsaw.pl    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/11 12:00:00 by sjesione          #+#    #+#             */
/*   Updated: 2026/01/11 17:41:59 by sjesione         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/shell.h"

int	apply_single_redir(t_ast *redir_node, t_heredoc *hd);
int	restore_single_redir(t_ast *redir_node, int original_fd);

static t_ast	*collect_redirs(t_ast *node, t_ast **arr, int *count)
{
	t_ast	*cmd;

	if (!node)
		return (NULL);
	if (node->node_type != NODE_REDIR)
		return (node);
	cmd = collect_redirs(node->left_node, arr, count);
	arr[*count] = node;
	(*count)++;
	return (cmd);
}

static int	apply_redirs(t_ast **arr, int count, int *saved_fds, t_heredoc *hd)
{
	int	i;

	i = 0;
	while (i < count)
	{
		saved_fds[i] = apply_single_redir(arr[i], hd);
		if (saved_fds[i] == -1)
			return (-1);
		i++;
	}
	return (0);
}

static void	restore_redirs(t_ast **arr, int count, int *saved_fds)
{
	int	i;

	i = count - 1;
	while (i >= 0)
	{
		restore_single_redir(arr[i], saved_fds[i]);
		close(saved_fds[i]);
		i--;
	}
}

int	exec_redir(t_ast *node, t_env *env, int is_piped, t_heredoc *hd)
{
	t_ast	*redirs_arr[100];
	int		redir_count;
	int		saved_fds[100];
	t_ast	*cmd;
	int		ret;

	redir_count = 0;
	cmd = collect_redirs(node, redirs_arr, &redir_count);
	if (apply_redirs(redirs_arr, redir_count, saved_fds, hd) != 0)
		return (-1);
	ret = order_66(cmd, env, is_piped, hd);
	restore_redirs(redirs_arr, redir_count, saved_fds);
	return (ret);
}
