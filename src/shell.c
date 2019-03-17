#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <getopt.h>
#define BOOLEAN char
#define FALSE 0
#define TRUE 1


/*
 * commandSplitter()
 * splits a command into its commponents, by whitespace, while keeping words within quotes (i.e. "hello there") as one argument
 *
 * Params
 * 	commandLine
 * 	pointer to a string. It's contents are changed and referenced by the return value, so it shouldn't be altered outside of this function for the return values's duration of use (done so each *output need not be free()'d)
 *
 * Return
 * 	Pointer to pointers of strings, up to 5
 */
#define MAXARGS 5 //value to determine how many arguments are passed into the function
char** commandSplitter(char* commandLine){
	while (isspace(*commandLine)) commandLine++;
	if (*commandLine == '\0'){
		printf("ERROR: not a valid command, type \"help\" for a list of commands.\n");
		return NULL;
	}
	char** output = calloc(MAXARGS+1 ,sizeof(char *));
	int index = 0;
	output[index] = commandLine;
	
	while (*++commandLine != '\0'){
		if (isspace(*commandLine)){
			if (*commandLine == '\n'){
				*commandLine = '\0';
				return output;
			}
			*commandLine = '\0';
			if (index >= MAXARGS-1)
				return output;
			while (isspace(*++commandLine));
			if (*commandLine == '\0')
				return output;
			if (*commandLine == '\"'){
				output[++index] = ++commandLine;
				while (*commandLine != '\"'){
					if (*commandLine == '\0'){
						printf("ERROR: failed to parse command line\n");
						return NULL;
					}
					commandLine++;
				}
				*commandLine = '\0';
				commandLine++;
			}
			output[++index] = commandLine;
		}
	}
	return output;
}

/*
 * executeCommand()
 * must be defined separately for each program
 *
 * Params:
 * 	arguments
 * 	list of string pointers for each argument
 *
 * Return:
 * 	BOOLEAN (char)
 * 	TRUE if shell should keep executing
 * 	FALSE if shell should terminate
 */
BOOLEAN executeCommand(char** arguments);

/* for testing purposes on executeCommand()
{
	for(int i=0; arguments[i]; i++)
		printf("%d) %s\t", i+1, arguments[i]);
	printf("\n");
	if (!strcmp(*arguments, "exit")){
		return FALSE;
	}
	else return TRUE;
}*/

/*
 * shell_loop()
 * runs the loop of the main program to ask for input from the user and execute accordingly
 * ends when executeCommand returns false
 *
 * Params
 * 	void
 *
 * Return
 * 	void
 */
void shell_loop(){
	char* commandLine;
	char** arguments;
	size_t bufsize = 256;
	BOOLEAN notExit = TRUE;
	
	while(notExit){
		printf("> ");
		getline(&commandLine, &bufsize, stdin);
		arguments = commandSplitter(commandLine);
		if (arguments){
			notExit = executeCommand(arguments);
			free(arguments);
		}
	}
}
