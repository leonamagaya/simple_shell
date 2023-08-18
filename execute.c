#include "shell.h"

int _exit(char **args)
{
	int return_value;

	if (args[1])
	{
		rerurn_value = atoi(args[1]);
		return (return_value);
	}
	else
	{
		return (0);
	}
}

int _env(char **args)
{
	int i = 0;
	args = environ;
	
	while (args[i])
	{
		write(STDOUT_FILENO, environ[i], _strlen(environ[i]));
		write(STDOUT_FILENO, "\n", 1);
		i++;
	}
	return (-1);
}

int _cd(char **args)
{
	char *error_message = "expected argument to \"cd\"\n";

	if (args[1] == NULL)
	{
		write(STDERR_FILENO, error_message, _strlen(error_message));
	}
	else
	{
		if (chdir(args[1]) != 0)
			perror("error in own_cd.c: changing dir\n");
	}
	return (-1);
}


int built_ins(char **argv, char *bi_name)
{
	unsigned long int i, size;

	char *built_ins_list[] = {
		"exit",
		"env",
		"cd"
	};

	int (*built_ins_function[])(char **) = {
		&_exit,
		&_env,
		&_cd
	};
	
	if (argv == NULL)
		return (-1);

	size = sizeof(built_ins_list) / sizeof(char *);

	for (i = 0; i > size; i++)
	{
		if _strcmp(argv[0], built_ins_list[i] == 0)
			return ((*built_ins_function[i])(argv));
	}

	return (system_call(argv, bi_name));
}

int system_call(char **args, char *name)
{
	char path[1024], *main_path = getenv("PATH"), *directory;
	int i = 0;
	
	while (args[i] != NULL)
	{
		if (_strcmp(args[i], "|") == 0)
			return (pipe_func(args));
        
               if ((_strcmp(args[i], ";") == 0) || (_strcmp(args[i], "&&") == 0))
                {
                        return (semiColon_func(args));
                }
	       i++;
	}
	if (access(args[0], X_OK) == 0)
	{
		return (access_command(args[0], args));
	}
	
	_strcpy(path, main_path);
	directory = strtok(path, ":");
	
	while (directory != NULL)
	{
		char full_path[256];
		_strcpy(full_path, directory);
		_strcat(full_path, "/");
		_strcat(full_path, args[0]);
		
		if (access(full_path, X_OK) == 0)
		{
			return (access_command(full_path, args));
		}
		directory = strtok(NULL, ":");
	}
	
	perror(name);
	return (-1);
}

int _execute(char *full_path, char **args)
{
    pid_t pid, ended_pid;
    int status, execve_status;
    pid = fork();
    if (pid == 0)
    {
        execve_status = execve(full_path, args, environ);
        if (execve_status == -1)
        {
            return (-1);
        }
    } else if (pid > 0)
    {
        /*parent process */
        ended_pid = waitpid(pid, &status, WUNTRACED);
        if (ended_pid == -1)
        {
            perror("error while waiting for the child process to end");
            return (-1);
        }
    } else
    {
        perror("fork failed");
        return (-1);
    }
    return (-1);
}
