#include <buracchi/argparser/argparser.h>

#include <string.h>

#include <buracchi/common/utilities/try.h>
#include <buracchi/common/utilities/utilities.h>

#include "struct_argparser.h"

extern void argparser_set_subparsers_options(argparser_t argparser,
                                             struct argparser_subparsers_options options) {
	memcpy(&argparser->subparsers_options, &options, sizeof argparser->subparsers_options);
}

extern argparser_t argparser_add_subparser(argparser_t argparser,
                                           const char **selection_result,
                                           const char *command_name,
                                           const char *help) {
	struct subparser subparser;
	struct subparser *subparsers_reallocd;
	char *prog;
	subparser.selection_result = selection_result;
	subparser.command_name = command_name;
	subparser.help = help;
	try(subparser.parser = argparser_init(argparser->argc, argparser->argv), nullptr, fail);
	if (argparser->subparsers_options.prog) {
		asprintf(&prog, "%s %s", argparser->subparsers_options.prog, command_name);
	}
	else {
		char *old_prog;
		asprintf(&prog, "%s", argparser->program_name);
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
	    nullptr,
	    fail2);
	argparser->subparsers = subparsers_reallocd;
	argparser->subparsers_number++;
	memcpy(&(argparser->subparsers[argparser->subparsers_number - 1]), &subparser, sizeof subparser);
	return subparser.parser;
fail2:
	argparser_destroy(subparser.parser);
fail:
	return nullptr;
}
