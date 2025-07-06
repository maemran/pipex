/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maemran < maemran@student.42amman.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 14:12:17 by maemran           #+#    #+#             */
/*   Updated: 2025/07/06 21:41:16 by maemran          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int     pipes_init(t_data *data)
{
    int i;

    i = 0;
    data->fd = malloc(sizeof(int *) * (data->cmds_num));
    if (!data->fd)
    {
        write(2, "Error: Memory allocation failed.\n", 34);
        return (FAILURE);
    }
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

char    *path_cmd_join(t_data *data, int i, char *path)
{
    //ls // /usr/bin/ls  /usr/bin/   /bin/
    char *str;
    char *new_cmd;

    str = ft_strjoin("/", data->cmds[i]);
    if (!str)
    {
        write(2, "Error: Memory allocation failed.\n", 34);
        return (NULL);
    }
    new_cmd = ft_strjoin(path, str);
    free(str);
    return (new_cmd);
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
            path_env = env[i] + 5;
            data->paths = ft_split(path_env, ':');
            if (!data->paths)
            {
                write(2, "Error: Memory allocation failed.\n", 34);
                return (FAILURE);
            }
            return (SUCCESS);
        }
        i++;
    }
    return (SUCCESS);
}

int absolute_path_executer(t_data *data, int i)
{
    int length;
    char *new_cmd;

    length = 0;
    while (data->paths[length])
    {
        new_cmd = path_cmd_join(data, i, data->paths[length]);
        if (!new_cmd)
        {
            write(2, "Error: Memory allocation failed.\n", 34);
            return (FAILURE);
        }
        if (access(new_cmd, X_OK) == 0)
            execve(new_cmd, data->cmd_args[i], NULL);
        free(new_cmd);
        length++;
    }
    return (SUCCESS);
}

int relative_path_executer(t_data *data, int i)
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

int redirection(t_data *data, int i)
{
    if (i == 0)
       dup2 (data->infile, STDIN_FILENO);
    if (i == data->cmds_num - 1)
        dup2(data->outfile, STDOUT_FILENO);
    if (i != 0)
        dup2(data->fd[i - 1][0], STDIN_FILENO);
    if (i == data->cmds_num - 1)
        dup2(data->fd[i][1], STDOUT_FILENO);
    //close files 
    return (SUCCESS);
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
            redirection(data);
            relative_path_executer(data, i);
            absolute_path_executer(data, i);
        }
    }
    
}

int open_infile(t_data *data, char *infile)
{
    data->infile = open(infile, O_RDONLY);
    if (data->infile < 0)
    {
        write(2, "Error: Unable to open infile.\n", 30);
        return (FAILURE);
    }
    if (access(infile, R_OK) != 0)
    {
        close(data->infile);
        write(2, "Error: No read permission for infile.\n", 38);
        return (FAILURE);
    }
    if (access(infile, R_OK) != 0)
    {
        close(data->infile);
        write(2, "Error: No read permission for infile.\n", 38);
        return (FAILURE);
    }
    return (SUCCESS);
}

int open_outfile(t_data *data, char *outfile)
{
    data->outfile = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0777);
    if (data->outfile < 0)
    {
        write(2, "Error: Unable to open outfile.\n", 31);
        return (FAILURE);
    }
    if (access(outfile, W_OK) != 0)
    {
        close(data->outfile);
        write(2, "Error: No write permission for outfile.\n", 39);
        return (FAILURE);
    }
    return (SUCCESS);
}

int io_files(t_data *data, char *infile, char *outfile)
{
    if (open_infile(data, infile) == FAILURE)
        return (FAILURE);
    if (open_outfile(data, outfile) == FAILURE)
    {
        close(data->infile);
        return (FAILURE);
    }
    return (SUCCESS);
}

int fill_args(int argc, char **argv, char **env, t_data *data)
{
    int i;

    i = 0;
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
    fill_args(argc, argv, env, data);
    prepare_path(env, data);
    return (0);
}