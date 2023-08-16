#include "shell.h"

#define BUFFSIZE 1024

int main(int argc, char *argv[], envp)
{
	int fd = STDIN_FILENO;

	if (isatty(fd) == 1)
		interactive_mode(args[argc - 1]);
	else
		non_interactive_mode(args[argc - 1]);
	return (0);
}

void prompt()
{
	char prompt_str = "cisfun$ ";
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

void interactive_mode(char *command)
{
	char *line;
	char **tokens;

	while (1)
	{
		prompt();
		line = get_line();
	



