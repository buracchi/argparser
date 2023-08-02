#include <buracchi/argparser/argparser.h>

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include <buracchi/common/utilities/try.h>
#include <buracchi/common/utilities/utilities.h>

#include "struct_argparser.h"
#include "utils.h"

static struct argparser_argument help_arg = { .flag = "h",
	                                          .long_flag = "help",
	                                          .action = ARGPARSER_ACTION_HELP,
	                                          .help = "show this help "
	                                                  "message and exit" };

extern argparser_t argparser_init(int sys_argc, char *const *sys_argv) {
	argparser_t this;
	try(this = malloc(sizeof *this), NULL, fail);
	try(this->arguments = malloc(sizeof *this->arguments), NULL, fail2);
	memcpy(&(this->arguments[0]), &help_arg, sizeof *this->arguments);
	this->sys_argc = sys_argc;
	this->sys_argv = sys_argv;
	this->program_name = NULL;
	this->usage = NULL;
	this->description = NULL;
	this->epilog = NULL;
	this->arguments_number = 1;
	this->add_help = true;
	this->exit_on_error = true;
	this->subparsers_number = 0;
	this->subparsers = NULL;
	this->subparsers_options =
		(struct argparser_subparsers_options){ .title = NULL,
		                                           .description = NULL,
		                                           .prog = NULL,
		                                           .required = false,
		                                           .help = NULL,
		                                           .metavar = NULL };
	return this;
fail2:
	free(this);
fail:
	return NULL;
}

extern void argparser_destroy(argparser_t argparser) {
	for (size_t i = 0; i < argparser->subparsers_number; i++) {
        argparser_destroy(argparser->subparsers[i].parser);
	}
	free(argparser->arguments);
	free(argparser);
}

extern void argparser_set_program_name(argparser_t argparser, char const *program_name) {
	argparser->program_name = program_name;
}

extern void argparser_set_usage(argparser_t argparser, char const *usage) {
	argparser->usage = usage;
}

extern void argparser_set_description(argparser_t argparser, char const *description) {
	argparser->description = description;
}

extern int argparser_add_argument_action_store_cstr(argparser_t argparser,
                                                        char **result,
                                                        struct argparser_argument argument) {
	argparser->arguments_number++;
	argument.action = ARGPARSER_ACTION_STORE;
	argument.type = ARGPARSER_TYPE_CSTR;
	argument.result = (void **)result;
	struct argparser_argument *reallocated_address;
	try(reallocated_address =
	            realloc(argparser->arguments,
	                    sizeof *argparser->arguments * argparser->arguments_number),
	    NULL,
	    fail);
	argparser->arguments = reallocated_address;
	memcpy(&(argparser->arguments[argparser->arguments_number - 1]),
	       &argument,
	       sizeof *argparser->arguments);
	return 0;
fail:
	return 1;
}

extern int argparser_add_argument_action_store_ushort(argparser_t argparser,
                                                          unsigned short int *result,
                                                          struct argparser_argument argument) {
	argparser->arguments_number++;
	argument.action = ARGPARSER_ACTION_STORE;
	argument.type = ARGPARSER_TYPE_USHORT;
	argument.result = (void **)result;
	struct argparser_argument *reallocated_address;
	try(reallocated_address =
	            realloc(argparser->arguments,
	                    sizeof *argparser->arguments * argparser->arguments_number),
	    NULL,
	    fail);
	argparser->arguments = reallocated_address;
	memcpy(&(argparser->arguments[argparser->arguments_number - 1]),
	       &argument,
	       sizeof *argparser->arguments);
	return 0;
fail:
	return 1;
}

extern int argparser_add_argument_action_store_uint(argparser_t argparser,
                                                        unsigned int *result,
                                                        struct argparser_argument argument) {
	argparser->arguments_number++;
	argument.action = ARGPARSER_ACTION_STORE;
	argument.type = ARGPARSER_TYPE_UINT;
	argument.result = (void **)result;
	struct argparser_argument *reallocated_address;
	try(reallocated_address =
	            realloc(argparser->arguments,
	                    sizeof *argparser->arguments * argparser->arguments_number),
	    NULL,
	    fail);
	argparser->arguments = reallocated_address;
	memcpy(&(argparser->arguments[argparser->arguments_number - 1]),
	       &argument,
	       sizeof *argparser->arguments);
	return 0;
fail:
	return 1;
}

extern int argparser_add_argument_action_store_int(argparser_t argparser,
                                                       int *result,
                                                       struct argparser_argument argument) {
	argparser->arguments_number++;
	argument.action = ARGPARSER_ACTION_STORE;
	argument.type = ARGPARSER_TYPE_INT;
	argument.result = (void **)result;
	struct argparser_argument *reallocated_address;
	try(reallocated_address =
	            realloc(argparser->arguments,
	                    sizeof *argparser->arguments * argparser->arguments_number),
	    NULL,
	    fail);
	argparser->arguments = reallocated_address;
	memcpy(&(argparser->arguments[argparser->arguments_number - 1]),
	       &argument,
	       sizeof *argparser->arguments);
	return 0;
fail:
	return 1;
}

extern int argparser_add_argument_action_store_long(argparser_t argparser,
                                                        long int *result,
                                                        struct argparser_argument argument) {
	argparser->arguments_number++;
	argument.action = ARGPARSER_ACTION_STORE;
	argument.type = ARGPARSER_TYPE_LONG;
	argument.result = (void **)result;
	struct argparser_argument *reallocated_address;
	try(reallocated_address =
	            realloc(argparser->arguments,
	                    sizeof *argparser->arguments * argparser->arguments_number),
	    NULL,
	    fail);
	argparser->arguments = reallocated_address;
	memcpy(&(argparser->arguments[argparser->arguments_number - 1]),
	       &argument,
	       sizeof *argparser->arguments);
	return 0;
fail:
	return 1;
}

extern int argparser_add_argument_action_store_true(argparser_t argparser,
                                                        bool *result,
                                                        struct argparser_argument argument) {
	argparser->arguments_number++;
	argument.action = ARGPARSER_ACTION_STORE_CONST;
	argument.type = ARGPARSER_TYPE_BOOL;
	argument.result = (void **)result;
	argument.const_value = (void *)true;
	*result = false;
	struct argparser_argument *reallocated_address;
	try(reallocated_address =
	            realloc(argparser->arguments,
	                    sizeof *argparser->arguments * argparser->arguments_number),
	    NULL,
	    fail);
	argparser->arguments = reallocated_address;
	memcpy(&(argparser->arguments[argparser->arguments_number - 1]),
	       &argument,
	       sizeof *argparser->arguments);
	return 0;
fail:
	return 1;
}

extern int argparser_add_argument_action_store_false(argparser_t argparser,
                                                         bool *result,
                                                         struct argparser_argument argument) {
	argparser->arguments_number++;
	argument.action = ARGPARSER_ACTION_STORE_CONST;
	argument.type = ARGPARSER_TYPE_BOOL;
	argument.result = (void **)result;
	argument.const_value = (void *)false;
	*result = true;
	struct argparser_argument *reallocated_address;
	try(reallocated_address =
	            realloc(argparser->arguments,
	                    sizeof *argparser->arguments * argparser->arguments_number),
	    NULL,
	    fail);
	argparser->arguments = reallocated_address;
	memcpy(&(argparser->arguments[argparser->arguments_number - 1]),
	       &argument,
	       sizeof *argparser->arguments);
	return 0;
fail:
	return 1;
}

extern void argparser_set_subparsers_options(argparser_t argparser,
                                                 struct argparser_subparsers_options options) {
	memcpy(&argparser->subparsers_options, &options, sizeof argparser->subparsers_options);
}

extern argparser_t argparser_add_subparser(argparser_t argparser,
                                               char const **selection_result,
                                               char const *command_name,
                                               char const *help) {
	struct subparser subparser;
	struct subparser *subparsers_reallocd;
	char *prog;
	subparser.selection_result = selection_result;
	subparser.command_name = command_name;
	subparser.help = help;
	try(subparser.parser = argparser_init(argparser->sys_argc, argparser->sys_argv),
	    NULL,
	    fail);
	if (argparser->subparsers_options.prog) {
		asprintf(&prog, "%s %s", argparser->subparsers_options.prog, command_name);
	}
	else {
		char *old_prog;
		asprintf(&prog, "%s", basename(argparser->sys_argv[0]));
		for (size_t i = 0; i < argparser->arguments_number; i++) {
			if (argparser->arguments[i].name) {
				old_prog = prog;
				asprintf(&prog,
				         "%s %s",
				         prog,
				         argparser->arguments[i].name);
				free(old_prog);
			}
		}
		old_prog = prog;
		asprintf(&prog, "%s %s", prog, command_name);
		free(old_prog);
	}
	// TODO: handle prog memory leak
	subparser.parser->program_name = prog;
	try(subparsers_reallocd = realloc(argparser->subparsers,
	                                  sizeof *(argparser->subparsers) *
	                                          (argparser->subparsers_number + 1)),
	    NULL,
	    fail2);
	argparser->subparsers = subparsers_reallocd;
	argparser->subparsers_number++;
	memcpy(&(argparser->subparsers[argparser->subparsers_number - 1]), &subparser, sizeof subparser);
	return subparser.parser;
fail2:
    argparser_destroy(subparser.parser);
fail:
	return NULL;
}
