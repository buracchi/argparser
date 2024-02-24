#include <buracchi/argparser/argparser.h>

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <buracchi/common/utilities/utilities.h>

#include "struct_argparser.h"

#include "arguments.h"

#define OPTION_START_STR "["
#define OPTION_END_STR   "]"
#define ARG_SEPARATOR    " "
#define SUBCOMMAND_START "{"
#define SUBCOMMAND_END   "}"
#define SUBCOMMAND_SEP   ","

extern int argparser_print_usage2(argparser_t argparser, FILE *file) {
	char *usage_message;
	usage_message = argparser_format_usage(argparser);
	fprintf(file, "%s\n", usage_message);
	free(usage_message);
	return 0;
}

extern size_t argparser_usage_size(argparser_t argparser) {
	size_t result = 0;
	size_t optionals_usage_len = 0;
	size_t positionals_usage_len = 0;
	size_t subcommands_usage_size = 0;
	const char *program_name = argparser_get_program_name(argparser);
	for (size_t i = 0; i < argparser->arguments_number; i++) {
		const struct argparser_argument *parg = &(argparser->arguments[i]);
		size_t str_vararg_len = get_arg_string_len(parg);
		if (parg->name) {
			positionals_usage_len += str_vararg_len + strlen(ARG_SEPARATOR);
		}
		else {
			optionals_usage_len +=
				strlen(!parg->is_required ? OPTION_START_STR : "");
			optionals_usage_len += strlen(parg->flag ? "-" : "--");
			optionals_usage_len +=
				strlen(parg->flag ? parg->flag : parg->long_flag);
			optionals_usage_len +=
				strlen(str_vararg_len == 0 ? ARG_SEPARATOR : "");
			optionals_usage_len += str_vararg_len;
			optionals_usage_len +=
				strlen(!parg->is_required ? OPTION_END_STR : "");
			optionals_usage_len += strlen(ARG_SEPARATOR);
		}
	}
	for (size_t i = 0; i < argparser->subparsers_number; i++) {
		subcommands_usage_size += strlen(i == 0 ? SUBCOMMAND_START : "");
		subcommands_usage_size += strlen(argparser->subparsers[i].command_name);
		subcommands_usage_size += strlen(
			(i < argparser->subparsers_number - 1) ? SUBCOMMAND_SEP : "");
		subcommands_usage_size +=
			strlen((i == argparser->subparsers_number - 1) ?
		                       SUBCOMMAND_END ARG_SEPARATOR "..." :
		                       "");
		subcommands_usage_size += strlen(ARG_SEPARATOR);
	}
	result += strlen("usage: ");
	result += strlen(program_name);
	result += strlen(strlen(program_name) != 0 ? ARG_SEPARATOR : "");
	result += optionals_usage_len;
	result += positionals_usage_len;
	result += subcommands_usage_size;
	return result;
}

extern int argparser_nformat_usage(argparser_t argparser, char *buffer, size_t buffer_size) {
	char *buffer_current = buffer;
	size_t buffer_size_remaining = buffer_size;
	const char *program_name = argparser_get_program_name(argparser);
	{ // write usage
		int written = snprintf(buffer_current,
		                       buffer_size_remaining,
		                       "usage: %s%s",
		                       program_name,
		                       strlen(program_name) != 0 ? ARG_SEPARATOR : "");
		if (written < 0 || (unsigned long)written >= buffer_size_remaining) {
			return written;
		}
		buffer_current += written;
		buffer_size_remaining -= written;
	}
	{ // write optionals
		for (size_t i = 0; i < argparser->arguments_number; i++) {
			const struct argparser_argument *parg =
				&(argparser->arguments[i]);
			char *str_vararg = get_arg_string(parg);
			if (parg->name) {
				continue;
			}
			int written =
				snprintf(buffer_current,
			                 buffer_size_remaining,
			                 "%s%s%s%s%s%s ",
			                 !parg->is_required ? OPTION_START_STR : "",
			                 parg->flag ? "-" : "--",
			                 parg->flag ? parg->flag : parg->long_flag,
			                 str_vararg ? ARG_SEPARATOR : "",
			                 str_vararg ? str_vararg : "",
			                 !parg->is_required ? OPTION_END_STR : "");
			if (written < 0 || (unsigned long)written >= buffer_size_remaining) {
				return written;
			}
			buffer_current += written;
			buffer_size_remaining -= written;
			free(str_vararg);
		}
	}
	{ // write positionals
		for (size_t i = 0; i < argparser->arguments_number; i++) {
			const struct argparser_argument *parg =
				&(argparser->arguments[i]);
			char *str_vararg = get_arg_string(parg);
			if (!parg->name) {
				continue;
			}
			int written = snprintf(buffer_current, buffer_size_remaining, "%s ", str_vararg);
			if (written < 0 || (unsigned long)written >= buffer_size_remaining) {
				return written;
			}
			buffer_current += written;
			buffer_size_remaining -= written;
			free(str_vararg);
		}
	}
	{ // write subcommands
		for (size_t i = 0; i < argparser->subparsers_number; i++) {
			int written = snprintf(
				buffer_current,
				buffer_size_remaining,
				"%s%s%s%s",
				i == 0 ? SUBCOMMAND_START : "",
				argparser->subparsers[i].command_name,
				(i < argparser->subparsers_number - 1) ? SUBCOMMAND_SEP : "",
				(i == argparser->subparsers_number - 1) ?
					SUBCOMMAND_END ARG_SEPARATOR "..." :
					"");
			if (written < 0 || (unsigned long)written >= buffer_size_remaining) {
				return written;
			}
			buffer_current += written;
			buffer_size_remaining -= written;
		}
	}
	return (int)buffer_size - buffer_size_remaining;
}

extern char *argparser_format_usage(argparser_t argparser) {
	char *usage_message;
	char *optionals_usage = nullptr;
	char *positionals_usage = nullptr;
	char *subcommands_usage = nullptr;
	const char *program_name = argparser_get_program_name(argparser);
	for (size_t i = 0; i < argparser->arguments_number; i++) {
		const struct argparser_argument *parg = &(argparser->arguments[i]);
		char *str_vararg = get_arg_string(parg);
		char *old_usage;
		if (parg->name) {
			old_usage = positionals_usage;
			asprintf(&positionals_usage,
			         "%s%s ",
			         positionals_usage ? positionals_usage : "",
			         str_vararg);
		}
		else {
			old_usage = optionals_usage;
			asprintf(&optionals_usage,
			         "%s%s%s%s%s%s%s ",
			         optionals_usage ? optionals_usage : "",
			         !parg->is_required ? OPTION_START_STR : "",
			         parg->flag ? "-" : "--",
			         parg->flag ? parg->flag : parg->long_flag,
			         str_vararg ? ARG_SEPARATOR : "",
			         str_vararg ? str_vararg : "",
			         !parg->is_required ? OPTION_END_STR : "");
		}
		free(str_vararg);
		free(old_usage);
	}
	for (size_t i = 0; i < argparser->subparsers_number; i++) {
		char *old_subcommands_usage;
		old_subcommands_usage = subcommands_usage;
		asprintf(&subcommands_usage,
		         "%s%s%s%s",
		         old_subcommands_usage ? old_subcommands_usage : SUBCOMMAND_START,
		         argparser->subparsers[i].command_name,
		         (i < argparser->subparsers_number - 1) ? SUBCOMMAND_SEP : "",
		         (i == argparser->subparsers_number - 1) ?
		                 SUBCOMMAND_END ARG_SEPARATOR "..." :
		                 "");
		free(old_subcommands_usage);
	}
	asprintf(&usage_message,
	         "usage: %s%s%s%s%s",
	         program_name,
	         strlen(program_name) != 0 ? ARG_SEPARATOR : "",
	         optionals_usage ? optionals_usage : "",
	         positionals_usage ? positionals_usage : "",
	         subcommands_usage ? subcommands_usage : "");
	free(optionals_usage);
	free(positionals_usage);
	return usage_message;
}
