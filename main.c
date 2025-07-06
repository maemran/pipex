/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maemran < maemran@student.42amman.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 14:12:17 by maemran           #+#    #+#             */
/*   Updated: 2025/07/06 15:19:58 by maemran          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int path_cmd_join(t_data *data, int i)
{
    ///
}

int prepare_path(char **env, t_data *data)
{
    int i;
    char *path_env;

    i = 0;
    while (env[i])
    {
        if (ft_strncmp(env[i], "PATH=", 5) == 0)
        {
            path_env = env[i];
            data->paths = ft_split(path_env, ':');
            if (!data->paths)
            {
                write(2, "Error: Memory allocation failed.\n", 34);
                return (1);
            }
            return (0);
        }
        i++;
    }
    return (0);
}

int fill_args(int argc, char **argv, char **env, t_data *data)
{
    int i;

    i = 0;
    data->cmd_args = malloc(sizeof(char **) * (data->cmds_num + 1));////
    data->cmds = malloc(sizeof(char *) * (data->cmds_num + 1));////
    while (i < data->cmd_args)// 0 executable 1 infile 2 cmd
    {
        //cmd ls -l
        data->cmd_args[i] = ft_split(argv[i + 2], ' ');
        if (!data->cmd_args[i])
        {
            write(2, "Error: Memory allocation failed.\n", 34);
            return (1);
        }
        data->cmds[i] = ft_strdup(data->cmd_args[i][0]);
        if (!data->cmds[i])
        {
            // Free previously allocated memory
            write(2, "Error: Memory allocation failed.\n", 34);
            return (1);
        }
        i++;
    }
    data->cmd_args[i] = NULL;
    data->cmds[i] = NULL;
    files();
    return (0);
}

int    main(int argc, char **argv, char **env)
{
    t_data *data;
    
    if (argc == 1)
    {
        write(2, "Error: No arguments provided.\n", 30);
        return (1);
    }
    data = malloc(sizeof(t_data));
    data->cmds_num = argc - 3;
    return (0);
}