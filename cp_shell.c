#include "shell.h"

#define BUFFSIZE 1024

int main(int argc, char *args[])
{
    int fd = STDIN_FILENO;
    /* determines if input in interactive or from file */
    if (isatty(fd) == 1)
        interactive_mode(args[argc - 1]);
    else
        non_interactive_mode(args[argc - 1]);
    return (0);
}

void interactive_mode(char *name)
{
    char *line;
    char **args;
    int execute_status;
    while (1)
    {
        write(0, "$ ", 2);
        line = read_input();
        args = split_strings(line);
        execute_status = execute_command(args, name);
        free(line);
        free(args);
        if (execute_status >= 0)
        {
            exit(execute_status);
        } else
        {
            continue;
        }
    }
}

char *get_line()
{
	int i = 0, character;
	char *line;

	line =  malloc(sizeof(char) * BUFFSIZE);

	if (line == NULL)
	{
		perror("something is wrong");
		exit(EXIT_FAILURE);
	}

	while (1)
	{
		character = getchar();

		if (character == EOF)
		{
			free(line);
			exit(EXIT_SUCCESS);
		}
		else if (character == '\n')
		{
			line[i] = '\0';
			return(line);
		}
		else
			line[i] = character;
		i++;
		if (i >= BUFFSIZE)
		{
			BUFFSIZE += BUFFSIZE;
			line = realloc(line, BUFFSIZE);
			if (line == NULL)
			{
				perror("something is wrong");
				exit(EXIT_FAUILURE);
			}
		}
	}
}


