#include "shell.h"

int exit_func(char **args)
{
    /* exit with status */
    if (args[1])
    {
        return (atoi(args[1]));
    }
    /* exit success */
    else
    {
        return (0);
    }
}

int env_func(char **args)
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

int cd_func(char **args)
{
	const char *error_message = "expected argument to \"cd\"\n";
	if (args[1] == NULL)
	{
		const char *error_message = "expected argument to \"cd\"\n";
		write(STDERR_FILENO, error_message, strlen(error_message));
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
		&exit_func,
		&env_func,
		&cd_func
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
    int index;
    index = 0;
    /* check if there is a pipe in the command*/
    while (args[index] != NULL)
    {
        if (strcmp(args[index], "|") == 0)
        {
            return (pipe_func(args));
        }
               if ((strcmp(args[index], ";") == 0) || (strcmp(args[index], "&&") == 0))
                {
                        return (semiColon_func(args));
                }
        index++;
    }
    if (access(args[0], X_OK) == 0)
    {
        return (access_command(args[0], args));
    }
    strcpy(path, main_path);
    directory = strtok(path, ":");
    while (directory != NULL)
    {
        char full_path[256];
        strcpy(full_path, directory);
        strcat(full_path, "/");
        strcat(full_path, args[0]);
        if (access(full_path, X_OK) == 0)
        {
            return (access_command(full_path, args));
        }
        directory = strtok(NULL, ":");
    }
    perror(name);
    return (-1);
}
