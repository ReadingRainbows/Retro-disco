#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "interpreter.h"

#define ENTRY_TABLE_SIZE 4 

const char* delimiters = " \n(,);";

entry_t tab[] = {
	{"add", cmd_add, "i", "Add money to database."},
	{"substract", cmd_substract, "i", "Substract money to database."},
	{"set", cmd_set, "i", "Set money amount to database."},
	{"exit", cmd_exit, " ", "Exit the program."},
};

void cmd_add(args_t* args)
{
	fprintf(stdout, "CMD_ADD -> arg : %d", args->i);
}

void cmd_substract(args_t* args)
{
	fprintf(stdout, "CMD_ADD -> arg : %d", args->i);
}


void cmd_set(args_t* args)
{
	fprintf(stdout, "CMD_SET -> arg : %d", args->i);
}

void cmd_exit(args_t* args)
{
	free(args);
	exit(0);
}

int run_interpreter(void) {
	char cmd[200];
	fprintf(stdout, "\n>");
	return parse(fgets(cmd, 200, stdin));
}

#define EXIT {free(args); fprintf(stdout,"Bad argument(s)"); return NULL;}
args_t* args_parse(const char *s)
{
	int argc = strlen(s);
	args_t* args = malloc(sizeof(args_t)*argc);
	int i = 0;
	for(i = 0; i < argc; ++i) {
		char* tok;
		switch(s[i]){
			case 's':
				args[i].s = strtok(NULL, delimiters);
				if(!args[i].s)
					EXIT
				break;
			case 'c':
				tok = strtok(NULL, delimiters);
				if(!tok)
					EXIT
				args[i].c = tok[0];
				if(!islower(args[i].c))
					EXIT
				break;
			case 'i':
				tok = strtok(NULL, delimiters);
				if(!tok)
					EXIT
				args[i].i = atoi(tok);
				break;
			case 'f':
				tok = strtok(NULL, delimiters);
				if(sscanf(tok,"%f", &args[i].f) != 1)
					EXIT
				break;
			case ' ':	
				free(args);
				args = NULL;
				break;
		}
	}
	return args;
}
#undef EXIT

int parse(char* cmd)
{
	const char* tok = strtok(cmd,delimiters);
	int i = ENTRY_TABLE_SIZE;
	if(!tok) {
		fprintf(stdout, "PARSE(): strtok returned NULL.\n");
		return 0;
	}

	while(i--) {
		if(strcmp(tok,tab[i].name) == 0){
			fprintf(stdout, "tok = %s\n", tok);
			args_t* args = args_parse(tab[i].args);
			if(args == NULL && strlen(tab[i].args))
				return 0;
			tab[i].func(args);
			free(args);
		}
	}
	return 1;
}

