/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maemran < maemran@student.42amman.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 12:13:46 by maemran           #+#    #+#             */
/*   Updated: 2025/07/07 12:52:03 by maemran          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

int infile_check(t_data *data)//OK
{
    if (data->infile < 0)
    {
        write(2, "Error: Infile does not exist.\n", 31);
        return (FAILURE);
    }
    if (access(data->in, F_OK) != 0)
    {
        close(data->infile);
        write(2, "Error: Infile does not exist.\n", 31);
        return (FAILURE);
    }
    if (access(data->in, R_OK) != 0)
    {
        close(data->infile);
        write(2, "Error: No read permission for infile.\n", 38);
        return (FAILURE);
    }
    return (SUCCESS);
}

int outfile_check(t_data *data)//OK
{
    if (data->outfile < 0)
    {
        write(2, "Error: Unable to open outfile.\n", 31);
        return (FAILURE);
    }
    if (access(data->out, W_OK) != 0)
    {
        close(data->outfile);
        write(2, "Error: No write permission for outfile.\n", 39);
        return (FAILURE);
    }
    return (SUCCESS);
}

int io_files(t_data *data, char *infile, char *outfile)//OK
{
    data->in = infile;
    data->out = outfile;
    data->outfile = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0777);
    data->infile = open(infile, O_RDONLY);
    return (SUCCESS);
}

int redirection(t_data *data, int i)//OK
{
    if (i == 0)
    {
        if (infile_check(data))
            return (FAILURE);
       dup2 (data->infile, STDIN_FILENO);
    }
    
    if (i == data->cmds_num - 1)
    {
        if (outfile_check(data))
            return (FAILURE);
        dup2(data->outfile, STDOUT_FILENO);
    }
    if (i != 0)
        dup2(data->fd[i - 1][0], STDIN_FILENO);
    if (i != data->cmds_num - 1)
        dup2(data->fd[i][1], STDOUT_FILENO);
    close_fds(data);
    return (SUCCESS);
}