/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwrzosek <kwrzosek@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/27 17:45:25 by kwrzosek          #+#    #+#             */
/*   Updated: 2025/12/27 22:20:32 by kwrzosek         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/shell.h"

int	order_66(t_ast *root)
{
	char	**cmd;
	char	*cmd_path;

	if (!root->argv)
		return (-1);
	cmd = list_to_argv(root->argv);
	cmd_path = get_path(cmd[0], cmd);
	printf("cmd: %s\n", cmd[0]);
	int i = 1;
	while (cmd[i])
		printf("cmd args: %s", cmd[i++]);
	printf("\ncmd path: %s\n", cmd_path);
	return(0);
}
