#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include "interpreter.h"

#define VERSION "0.0.1"

static void cli_usage(const char *name)
{
	printf("Usage: %s [-h|-v]\n\n", name);
	printf(" -h | --help			print this help text\n"
	       " -v | --version			print version release\n"
	       " -r | --run			run budget interpreter???\n");
}

static void cli_abort_usage(void) 
{
	printf("Please run \"string-parser --help\" for usage info\n");
	exit(1);
}

static void cli_version_opt_call(const char *name, const char *version)
{
	printf("The current version of %s is: %s\n", name, version);
	exit(0);
}

int main(int argc, char *argv[])
{
	int option_index, opt = 0;
	int operation_specified = 0;
	static const char optstring[] = "h:v:r:";
	static const struct option long_options[] = {
		{"help",	0,NULL, 'h'},
		{"version",	0,NULL, 'v'},
		{"run",		0,NULL, 'r'},
		{NULL,		0,NULL, 0},
	};

	
	while((opt = getopt_long(argc, argv, optstring,
			long_options, &option_index)) != EOF) {
		switch(opt) {
		case 'h':
			if(++operation_specified > 1) {
				fprintf(stderr, "More than one operation "
						"specified. Aborting.\n");
				cli_abort_usage();
			}
			cli_usage(argv[0]);
			break;
		case 'v':
			if(++operation_specified > 1) {
				fprintf(stderr, "More than one operation "
						"specified. Aborting.\n");
				cli_abort_usage();
			}
			cli_version_opt_call(argv[0],VERSION);
			break;
		case 'r':
			if(++operation_specified > 1) {
				fprintf(stderr, "More than one operation "
						"specified. Aborting.\n");
				cli_abort_usage();
			}
			while(run_interpreter()){
			}
			//fprintf(stderr, "Still getting implemented. Aborting.\n");
			//cli_abort_usage();
			break;
		default:
			cli_usage(argv[0]);
			break;
		}
	}
	return 0;	
}
