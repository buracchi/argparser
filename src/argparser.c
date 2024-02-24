#include <buracchi/argparser/argparser.h>

#include <stdlib.h>
#include <string.h>

#include <buracchi/common/utilities/try.h>
#include <fcntl.h>
#include <unistd.h>

#include "struct_argparser.h"
#include "utils.h"

extern argparser_t argparser_init(int argc, const char *argv[argc + 1]) {
	struct {
		struct argparser argparser;
		char default_program_name[];
	} *mblock;

	struct argparser_argument *arguments;
	size_t default_program_name_size = (argc ? strlen(argv[0]) + 1 : 0);
	mblock = malloc(sizeof *mblock + default_program_name_size);
	if (mblock == nullptr) {
		return nullptr;
	}
	arguments = malloc(sizeof *arguments);
	if (arguments == nullptr) {
		free(mblock);
		return nullptr;
	}
	if (argc) {
		basename(argv[0], default_program_name_size, mblock->default_program_name);
	}
	mblock->argparser = (struct argparser){
		.argc = argc,
		.argv = argv,
		.default_program_name = argc ? mblock->default_program_name : "",
		.program_name = argc ? mblock->default_program_name : "",
		.usage = nullptr,
		.description = nullptr,
		.epilog = nullptr,
		.arguments_number = 1,
		.arguments = arguments,
		.add_help = true,
		.exit_on_error = true,
		.subparsers_number = 0,
		.subparsers = nullptr,
		.subparsers_options =
			(struct argparser_subparsers_options){
							      .title = nullptr,
							      .description = nullptr,
							      .prog = nullptr,
							      .required = false,
							      .help = nullptr,
							      .metavar = nullptr,
							      },
	};
	arguments[0] = (struct argparser_argument){
		.flag = "h",
		.long_flag = "help",
		.action = ARGPARSER_ACTION_HELP,
		.help = "show this help message and exit",
	};
	return &mblock->argparser;
}

extern void argparser_destroy(argparser_t argparser) {
	for (size_t i = 0; i < argparser->subparsers_number; i++) {
		argparser_destroy(argparser->subparsers[i].parser);
	}
	free(argparser->arguments);
	free(argparser);
}

extern void argparser_error(argparser_t argparser, const char *message) {
	bool is_sdterr_valid = !(fcntl(STDERR_FILENO, F_GETFD) == -1 && errno == EBADF);
	if (message != nullptr && is_sdterr_valid) {
		argparser_print_usage(argparser, stderr);
		fprintf(stderr, "%s: error: \n", argparser_get_program_name(argparser));
		argparser_exit_status_message(argparser, 2, message);
	}
}

extern void argparser_exit_status_message(argparser_t argparser, int status, const char *message) {
	bool is_sdterr_valid = !(fcntl(STDERR_FILENO, F_GETFD) == -1 && errno == EBADF);
	if (message != nullptr && is_sdterr_valid) {
		fprintf(stderr, "%s\n", message);
	}
	exit(status);
}

extern inline const char *argparser_get_program_name(argparser_t argparser) {
	return argparser->program_name;
}

extern inline void argparser_set_program_name(argparser_t argparser, const char *program_name) {
	argparser->program_name = (program_name == nullptr) ?
	                                  argparser->default_program_name :
	                                  program_name;
}

extern void argparser_set_usage(argparser_t argparser, const char *usage) {
	argparser->usage = usage;
}

extern void argparser_set_description(argparser_t argparser, const char *description) {
	argparser->description = description;
}
