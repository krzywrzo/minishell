/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjesione < sjesione@student.42warsaw.pl    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 16:30:00 by sjesione          #+#    #+#             */
/*   Updated: 2026/01/09 17:14:07 by sjesione         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/shell.h"
#include <sys/wait.h>

static char	*execute_command_substitution(char *cmd_str)
{
	FILE	*fp;
	char	*result;
	char	buffer[4096];
	int		result_len;

	// Create a pipe to capture command output
	fp = popen(cmd_str, "r");
	if (!fp)
		return (ft_strdup(""));
	
	result = malloc(4096);
	if (!result)
	{
		pclose(fp);
		return (ft_strdup(""));
	}
	
	result_len = 0;
	ft_memset(buffer, 0, sizeof(buffer));
	// Read command output into result buffer
	while (fgets(buffer, sizeof(buffer), fp) && result_len < 4090)
	{
		int len = ft_strlen(buffer);
		// Replace trailing newline with space
		if (len > 0 && buffer[len - 1] == '\n')
		{
			buffer[len - 1] = ' ';
		}
		ft_strlcpy(&result[result_len], buffer, 4096 - result_len);
		result_len += ft_strlen(buffer);
		ft_memset(buffer, 0, sizeof(buffer));
	}
	
	pclose(fp);
	
	// Remove trailing space if present
	if (result_len > 0 && result[result_len - 1] == ' ')
		result_len--;
	
	result[result_len] = '\0';
	return (result);
}

static char	*get_var_value(char *var_name, t_env *env)
{
	int	len;

	len = 0;
	while (var_name[len] && (ft_isalnum(var_name[len]) || var_name[len] == '_'))
		len++;
	while (env)
	{
		if (ft_strncmp(env->key, var_name, len) == 0 
			&& env->key[len] == '\0')
			return (env->val);
		env = env->next;
	}
	return (NULL);
}

static int	get_var_name_len(char *s)
{
	int	len;

	len = 0;
	while (s[len] && (ft_isalnum(s[len]) || s[len] == '_'))
		len++;
	return (len);
}

char	*expand_variables(char *str, t_env *env)
{
	char	*result;
	char	*var_val;
	char	*exit_code;
	char	*cmd_result;
	char	*cmd_str;
	int		i;
	int		var_len;
	int		result_len;
	int		cmd_start;
	int		j;
	int		paren_depth;

	if (!str)
		return (ft_strdup(""));
	
	// Debug: Check if we have $ in the string
	if (ft_strchr(str, '$'))
	{
		// String contains $, process it
	}
	else
	{
		// No $ found, just duplicate
		return (ft_strdup(str));
	}
	
	result = malloc(4096);
	if (!result)
		return (ft_strdup(str));
	result_len = 0;
	i = 0;
	while (str[i] && result_len < 4090)
	{
		// Handle command substitution $(...)
		if (str[i] == '$' && str[i + 1] == '(')
		{
			// Find the matching closing parenthesis
			cmd_start = i + 2;  // Start of actual command
			paren_depth = 1;
			j = i + 2;
			while (str[j] && paren_depth > 0)
			{
				if (str[j] == '(')
					paren_depth++;
				else if (str[j] == ')')
					paren_depth--;
				if (paren_depth > 0)
					j++;
			}
			
			if (paren_depth == 0)
			{
				// Extract command from cmd_start to j (exclusive)
				cmd_str = ft_substr(str, cmd_start, j - cmd_start);
				if (cmd_str && ft_strlen(cmd_str) > 0)
				{
					cmd_result = execute_command_substitution(cmd_str);
					if (cmd_result && ft_strlen(cmd_result) > 0)
					{
						ft_strlcpy(&result[result_len], cmd_result, 4096 - result_len);
						result_len += ft_strlen(cmd_result);
						free(cmd_result);
					}
					else if (cmd_result)
						free(cmd_result);
					free(cmd_str);
				}
				i = j + 1;  // Move past the closing parenthesis
			}
			else
			{
				// Unmatched parenthesis, just copy the character
				result[result_len++] = str[i++];
			}
		}
		// Handle $? (exit status)
		else if (str[i] == '$' && str[i + 1] == '?')
		{
			exit_code = ft_itoa(g_exit_status);
			if (exit_code)
			{
				ft_strlcpy(&result[result_len], exit_code, 4096 - result_len);
				result_len += ft_strlen(exit_code);
				free(exit_code);
			}
			i += 2;
		}
		// Handle $VAR (variable expansion)
		else if (str[i] == '$' && (ft_isalpha(str[i + 1]) || str[i + 1] == '_'))
		{
			i++;
			var_len = get_var_name_len(&str[i]);
			var_val = get_var_value(&str[i], env);
			if (var_val)
			{
				ft_strlcpy(&result[result_len], var_val, 4096 - result_len);
				result_len += ft_strlen(var_val);
			}
			i += var_len;
		}
		else
		{
			result[result_len++] = str[i++];
		}
	}
	result[result_len] = '\0';
	return (result);
}
