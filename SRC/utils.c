/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maemran < maemran@student.42amman.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 12:18:03 by maemran           #+#    #+#             */
/*   Updated: 2025/07/07 12:52:07 by maemran          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

int failure(char *msg)//OK
{
    ft_putstr_fd(msg, 2);
    return (FAILURE);
}

int close_fds(t_data *data)//OK
{
    int i;

    i = 0;
    while (i < data->cmds_num - 1)
    {
        close(data->fd[i][0]);
        close(data->fd[i][1]);
        i++;
    }
    close(data->infile);
    close(data->outfile);
    return (SUCCESS);
}

char    *path_cmd_join(t_data *data, int i, char *path)// OK
{
    char *str;
    char *new_cmd;

    str = ft_strjoin("/", data->cmds[i]);
    if (!str)
    {
        write(2, "Error: Memory allocation failed.\n", 34);
        return (NULL);
    }
    new_cmd = ft_strjoin(path, str);
    if (!new_cmd)
    {
        write(2, "Error: Memory allocation failed.\n", 34);
        free(str);
        return (NULL);
    }
    free(str);
    return (new_cmd);
}
