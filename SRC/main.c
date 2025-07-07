/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maemran < maemran@student.42amman.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 14:12:17 by maemran           #+#    #+#             */
/*   Updated: 2025/07/07 12:51:56 by maemran          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

int     pipes_init(t_data *data)
{
    int i;

    i = 0;
    data->fd = malloc(sizeof(int *) * (data->cmds_num));
    if (!data->fd)
        return (failure("Error: Memory allocation failed.\n"));
    while (i < data->cmds_num - 1)
    {
        data->fd[i] = malloc(sizeof(int) * 2);/////
        if (pipe(data->fd[i]) == -1)
        {
            write(2, "Error: Pipe creation failed.\n", 30);
            return (FAILURE);
        }
        i++;
    }
    data->fd[i] = NULL;
    return (SUCCESS);
}

int prepare_path(char **env, t_data *data)// OK
{
    int i;
    char *path_env;

    i = 0;
    while (env[i])
    {
        if (ft_strncmp(env[i], "PATH=", 5) == 0)
        {
            path_env = env[i] + 5;
            data->paths = ft_split(path_env, ':');
            if (!data->paths)
                return (failure("Error: Memory allocation failed.\n"));
            return (SUCCESS);
        }
        i++;
    }
    return (SUCCESS);
}

int absolute_path_executer(t_data *data, int i)//OK
{
    int length;
    char *new_cmd;

    length = 0;
    while (data->paths[length])
    {
        new_cmd = path_cmd_join(data, i, data->paths[length]);
        if (!new_cmd)
            return (FAILURE);
        if (access(new_cmd, X_OK) == 0)
            execve(new_cmd, data->cmd_args[i], NULL);
        free(new_cmd);
        length++;
    }
    return (SUCCESS);
}

int relative_path_executer(t_data *data, int i)//OK
{
    if (ft_strncmp(data->cmds[i], "./", 2) == 0 || 
        ft_strncmp(data->cmds[i], "../", 3) == 0 || 
        ft_strncmp(data->cmds[i], "/", 1) == 0)
    {
        if (access(data->cmds[i], X_OK) == 0)
            execve(data->cmds[i], data->cmd_args[i], NULL);
    }
    return (SUCCESS);
}

void wait_process(t_data *data)//OK
{
    int i;

    i = 0;
    while (i < data->cmds_num)
    {
        wait(NULL);
        i++;
    }
}

int executer(t_data *data)
{
    int i;
    int id;
    
    i = 0;
    while (i < data->cmds_num)
    {
        id = fork();
        if (id == 0)
        {
            if (redirection(data, i))
                exit (1);
            relative_path_executer(data, i);
            if (absolute_path_executer(data, i))//free mem allocation
                exit (1);
        }
        if (id != 0)
        {
            if (i != 0)
                close(data->fd[i - 1][0]);
            if (i != data->cmds_num - 1)
                close(data->fd[i][1]);
        }
        i++;
    }
    close(data->infile);
    close(data->outfile);
    wait_process(data);
    return (SUCCESS);
}

int fill_args(int argc, char **argv, char **env, t_data *data)
{
    int i;

    i = 0;
    (void)env;
    data->cmd_args = malloc(sizeof(char **) * (data->cmds_num + 1));////
    data->cmds = malloc(sizeof(char *) * (data->cmds_num + 1));////
    while (i < data->cmds_num)// 0 executable 1 infile 2 cmd
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
    io_files(data, argv[1], argv[argc - 1]);
    return (SUCCESS);
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
    fill_args(argc, argv, env, data);
    prepare_path(env, data);
    pipes_init(data);
    executer(data);
    return (0);
}