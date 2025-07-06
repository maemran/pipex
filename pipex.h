/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maemran < maemran@student.42amman.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 14:13:07 by maemran           #+#    #+#             */
/*   Updated: 2025/07/06 15:11:17 by maemran          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

#include <unistd.h>
#include <stdlib.h>
#include "./libft/libft.h"

typedef struct s_data
{
    int     **fd;
    int     cmds_num;
    int     infile;
    int     outfile;
    char    ***cmd_args;
    char    **cmds;
    char    **paths;
}           t_data;

#endif