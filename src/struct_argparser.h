#pragma once

#include <buracchi/argparser/argparser.h>

struct subparser {
    char const **selection_result;
    char const *command_name;
    char const *help;
    argparser_t parser;
};

struct argparser {
    int sys_argc;
    char *const *sys_argv;
    char const *program_name; /* The name of the program (default: basename(sys_argv[0])) */
    char const *usage; /* The string describing the program usage (default: generated from arguments added to parser) */
    char const *description; /* Text to display before the argument help (default: none) */
    char const *epilog; /* Text to display after the argument help (default: none) */
    bool add_help; /* Add a -h/--help option to the parser (default: True) */
    bool exit_on_error; /* Determines whether ArgumentParser exits with error info when an error occurs. (default: True) */
    size_t arguments_number;
    struct argparser_argument *arguments;
    size_t subparsers_number;
    struct subparser *subparsers;
    struct argparser_subparsers_options subparsers_options;
};
