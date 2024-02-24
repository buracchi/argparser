#pragma once

#include <buracchi/argparser/argparser.h>

struct subparser {
	const char **selection_result;
	const char *command_name;
	const char *help;
	argparser_t parser;
};

struct argparser {
	int argc;
	const char *const *argv;
	const char *default_program_name;
	const char *program_name; /* The name of the program (default:
	                             basename(argv[0])) */
	const char *usage; /* The string describing the program usage (default:
	                      generated from arguments added to parser) */
	const char *description; /* Text to display before the argument help
	                            (default: none) */
	const char *epilog; /* Text to display after the argument help (default: none) */
	bool add_help; /* Add a -h/--help option to the parser (default: True) */
	bool exit_on_error; /* Determines whether ArgumentParser exits with error
	                       info when an error occurs. (default: True) */
	size_t arguments_number;
	struct argparser_argument *arguments;
	size_t subparsers_number;
	struct subparser *subparsers;
	struct argparser_subparsers_options subparsers_options;
};
