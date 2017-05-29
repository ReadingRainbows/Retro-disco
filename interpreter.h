#ifndef _interpreter_h
#define _interpreter_h

typedef union {
	char* s;
	char  c;
	int   i;
	float f;
} args_t;

typedef struct {
	const char* name;
	void (*func)(args_t*);
	const char* args;
	const char* description;	
} entry_t;

int run_interpreter(void);
//void parse(char* cmd, int* arg);
int parse(char* cmd);
args_t* args_parse(const char *s);

void cmd_add(args_t* args);
void cmd_substract(args_t* args);
void cmd_set(args_t* args);
void cmd_exit(args_t* args);
void cmd_debug_database(args_t* args);

#endif
