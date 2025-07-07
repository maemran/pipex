/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maemran < maemran@student.42amman.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 14:13:07 by maemran           #+#    #+#             */
/*   Updated: 2025/07/07 12:49:09 by maemran          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "../libft/libft.h"

#define SUCCESS 0
#define FAILURE 1

typedef struct s_data
{
    int     **fd;
    int     cmds_num;
    char   **env;
    char    *in;
    char    *out;
    int     infile;
    int     outfile;
    char    ***cmd_args;
    char    **cmds;
    char    **paths;
}           t_data;

int redirection(t_data *data, int i);
int io_files(t_data *data, char *infile, char *outfile);
int close_fds(t_data *data);
int failure(char *msg);
char    *path_cmd_join(t_data *data, int i, char *path);
int infile_check(t_data *data);
int outfile_check(t_data *data);

#endif