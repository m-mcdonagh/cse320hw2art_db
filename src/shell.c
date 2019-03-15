#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <getopt.h>
#define BOOLEAN char
#define FALSE 0
#define TRUE 1


#define MAXARGS 5
char** commandSplitter(char* commandLine){
	while (isspace(*commandLine)) commandLine++;
	if (*commandLine == '\0'){
		printf("ERROR: failed to parse command line\n");
		return NULL;
	}
	//commandLine = malloc(strlen(commandLine)*sizeof(char)+1);
	char** output = malloc(MAXARGS*sizeof(char*));
	int inputCheck = 0;
	int index = 0;
	output[index] = commandLine;
	
	while (*++commandLine != '\0' && index < MAXARGS){
		if (*commandLine == ' '){
			*commandLine = '\0';
			while (isspace(*++commandLine));
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
				while(isspace(*++commandLine));
			}
			else{
				output[++index] = commandLine;
			}
		}
	}
	return output;
}

void shell_loop(){
	char* commandLine;
	char** arguments;
	size_t bufsize = 256;
	BOOLEAN notExit = TRUE;
	
	while(notExit){
		printf("> ");
		getline(&commandLine, &bufsize, stdin);
		arguments = commandSplitter(commandLine);
		notExit = executeCommand(arguments);
		free(arguments);
	}
}
