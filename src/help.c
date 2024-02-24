#include <buracchi/argparser/argparser.h>

#include <stdio.h>
#include <stdlib.h>

#include <buracchi/common/utilities/utilities.h>

#include "struct_argparser.h"

#include "arguments.h"

extern int argparser_print_help2(argparser_t argparser, FILE *file) {
	char *help_message;
	help_message = argparser_format_help(argparser);
	fprintf(file, "%s", help_message);
	free(help_message);
	return 0;
}

extern char *argparser_format_help(argparser_t argparser) {
	char *help_message;
	char *usage_message;
	char *optionals_description = nullptr;
	char *positionals_description = nullptr;
	char *subcommands_description = nullptr;
	usage_message = argparser_format_usage(argparser);
	for (size_t i = 0; i < argparser->arguments_number; i++) {
		struct argparser_argument *parg = &(argparser->arguments[i]);
		char *str_vararg = get_arg_string(parg);
		char *old_description;
		if (parg->name) {
			old_description = positionals_description;
			asprintf(&positionals_description,
			         "%s  %s\t\t\t%s\n",
			         positionals_description ? positionals_description : "",
			         parg->name,
			         parg->help ? parg->help : "");
		}
		else {
			old_description = optionals_description;
			asprintf(&optionals_description,
			         "%s  %s%s%s%s%s%s%s%s%s\t%s\n",
			         optionals_description ? optionals_description : "",
			         parg->flag ? "-" : "",
			         parg->flag ? parg->flag : "",
			         str_vararg ? " " : "",
			         str_vararg ? str_vararg : "",
			         parg->flag ? ", " : "",
			         parg->long_flag ? "--" : "",
			         parg->long_flag ? parg->long_flag : "",
			         str_vararg ? " " : "",
			         str_vararg ? str_vararg : "\t",
			         parg->help ? parg->help : "");
		}
		free(str_vararg);
		free(old_description);
	}
	for (size_t i = 0; i < argparser->subparsers_number; i++) {
		char *old_subcommands_description;
		old_subcommands_description = subcommands_description;
		asprintf(&subcommands_description,
		         "%s%s%s%s",
		         old_subcommands_description ? old_subcommands_description : "  {",
		         argparser->subparsers[i].command_name,
		         (i < argparser->subparsers_number - 1) ? "," : "",
		         (i == argparser->subparsers_number - 1) ? "}\n" : "");
		free(old_subcommands_description);
	}
	for (size_t i = 0; i < argparser->subparsers_number; i++) {
		if (argparser->subparsers[i].help) {
			char *old_subcommands_description;
			old_subcommands_description = subcommands_description;
			asprintf(&subcommands_description,
			         "%s    %s\t\t%s\n",
			         old_subcommands_description,
			         argparser->subparsers[i].command_name,
			         argparser->subparsers[i].help);
			free(old_subcommands_description);
		}
	}
	if (argparser->subparsers_options.title == nullptr &&
	    argparser->subparsers_options.description) {
		argparser->subparsers_options.title = "subcommands";
	}
	asprintf(&help_message,
	         "%s\n%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
	         usage_message,
	         argparser->description ? argparser->description : "",
	         argparser->description ? "\n" : "",
	         positionals_description ? "\npositional arguments:\n" : "",
	         positionals_description ? positionals_description : "",
	         subcommands_description && !argparser->subparsers_options.title ?
	                 subcommands_description :
	                 "",
	         optionals_description ? "\noptional arguments:\n" : "",
	         optionals_description ? optionals_description : "",
	         argparser->subparsers_options.title ? "\n" : "",
	         argparser->subparsers_options.title ?
	                 argparser->subparsers_options.title :
	                 "",
	         argparser->subparsers_options.title ? ":\n" : "",
	         argparser->subparsers_options.description ? "  " : "",
	         argparser->subparsers_options.description ?
	                 argparser->subparsers_options.description :
	                 "",
	         argparser->subparsers_options.description ? "\n\n" : "",
	         argparser->subparsers_options.title ? subcommands_description : "");
	free(usage_message);
	free(optionals_description);
	free(positionals_description);
	return help_message;
}
