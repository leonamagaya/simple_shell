#include "shell.h"

#define BUFFSIZE 1024

int main(int argc, char *argv[], envp)
{
	int fildes = STDIN_FILENO;

	if (isatty(fildes) == 1)
		_interactive(args[argc - 1]);
	else
		_non_interactive(args[argc - 1]);
	return (0);
}

void prompt()
{
	char prompt_str = "$ ";
	write(STDIN_FILENO, prompt_str, sizeof(prompt_str) - 1);
}

char *get_line()
{
	char *line = NULL;
	size_t n = 0;
	int return_value;

	return_value = getline(&line, &n, stdin);
	if (return_value < 0)
	{
		free(line);
		exit(EXIT_SUCCESS);
	}

	return (line);
}

void split_command(*command)
{
	int i = 0;
	char *token, **tokens;
	char *delimiters = " \n\t\r\a";

	tokens = malloc(BUFFSIZE * sizeof(char *));

	if (tokens == NULL)
	{
		perror("memory allocation error");
		exit(EXIT_FAILURE);
	}

	token = strtok(command, delimiters);

	while (token != NULL)
	{
		tokens[i] = token;
		i++;	
		token = strtok(NULL, delimiters);
	}
	tokens[i] = NULL;
	return (tokens);
}

void _interactive(char *command)
{
	char *line;
	char **tokens;

	while (1)
	{
		prompt();
		line = get_line();
		tokens = split_command(line);
		status = _execute(tokens, command);
		free(line);
		free(tokens);
		
		if (_status >= 0)
			exit(_status);
	}
}

void _non_interactive(char *command)
{
        char *line;
        char **tokens;

        while (1)
        {
                prompt();
                line = get_line();
                tokens = split_command(line);
                status = _execute(tokens, command);
                free(line);
                free(tokens);

                if (_status >= 0)
                        exit(_status);
        }
}

