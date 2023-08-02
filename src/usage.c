#include <buracchi/argparser/argparser.h>

#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <buracchi/common/utilities/utilities.h>

#include "struct_argparser.h"
#include "utils.h"

static char *get_arg_string(struct argparser_argument const *arg);

static char *get_flag_vararg(struct argparser_argument const *arg);

static char *get_narg_optional(char const *vararg);

static char *get_narg_list_n(char const *vararg, size_t n);

static char *get_narg_list(char const *vararg);

static char *get_narg_list_optional(char const *vararg);

extern int argparser_print_usage2(argparser_t argparser, FILE *file) {
    char *usage_message;
    usage_message = argparser_format_usage(argparser);
    fprintf(file, "%s\n", usage_message);
    free(usage_message);
    return 0;
}

extern int argparser_print_help2(argparser_t argparser, FILE *file) {
    char *help_message;
    help_message = argparser_format_help(argparser);
    fprintf(file, "%s", help_message);
    free(help_message);
    return 0;
}

extern char *argparser_format_usage(argparser_t argparser) {
    char *usage_message;
    char const *program_name;
    char *optionals_usage = NULL;
    char *positionals_usage = NULL;
    char *subcommands_usage = NULL;
    if (argparser->program_name) {
        program_name = argparser->program_name;
    }
    else {
        program_name = basename(argparser->sys_argv[0]);
    }
    for (size_t i = 0; i < argparser->arguments_number; i++) {
        struct argparser_argument const *parg = &(argparser->arguments[i]);
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
                     !parg->is_required ? "[" : "",
                     parg->flag ? "-" : "--",
                     parg->flag ? parg->flag : parg->long_flag,
                     str_vararg ? " " : "",
                     str_vararg ? str_vararg : "",
                     !parg->is_required ? "]" : "");
        }
        free(str_vararg);
        free(old_usage);
    }
    for (size_t i = 0; i < argparser->subparsers_number; i++) {
        char *old_subcommands_usage;
        old_subcommands_usage = subcommands_usage;
        asprintf(&subcommands_usage,
                 "%s%s%s%s",
                 old_subcommands_usage ? old_subcommands_usage : "{",
                 argparser->subparsers[i].command_name,
                 (i < argparser->subparsers_number - 1) ? "," : "",
                 (i == argparser->subparsers_number - 1) ? "} ..." : "");
        free(old_subcommands_usage);
    }
    asprintf(&usage_message,
             "usage: %s%s%s%s%s",
             program_name,
             strlen(program_name) != 0 ? " " : "",
             optionals_usage ? optionals_usage : "",
             positionals_usage ? positionals_usage : "",
             subcommands_usage ? subcommands_usage : "");
    free(optionals_usage);
    free(positionals_usage);
    return usage_message;
}

extern char *argparser_format_help(argparser_t argparser) {
    char *help_message;
    char *usage_message;
    char *optionals_description = NULL;
    char *positionals_description = NULL;
    char *subcommands_description = NULL;
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
    if (argparser->subparsers_options.title == NULL &&
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

static char *get_arg_string(struct argparser_argument const *arg) {
    char *vararg;
    bool is_positional = arg->name;
    if (is_positional) {
        asprintf(&vararg, "%s", arg->name);
    }
    else {
        bool needs_arg = (arg->action == ARGPARSER_ACTION_STORE) ||
                         (arg->action == ARGPARSER_ACTION_APPEND) ||
                         (arg->action == ARGPARSER_ACTION_EXTEND);
        if (!needs_arg) {
            return NULL;
        }
        vararg = get_flag_vararg(arg);
    }
    switch (arg->action_nargs) {
        case ARGPARSER_ACTION_NARGS_SINGLE:
            return vararg;
        case ARGPARSER_ACTION_NARGS_OPTIONAL:
            return get_narg_optional(vararg);
        case ARGPARSER_ACTION_NARGS_LIST_OF_N:
            return get_narg_list_n(vararg, arg->nargs_list_size);
        case ARGPARSER_ACTION_NARGS_LIST:
            return get_narg_list(vararg);
        case ARGPARSER_ACTION_NARGS_LIST_OPTIONAL:
            return get_narg_list_optional(vararg);
        default:
            return NULL;
    }
}

static char *get_flag_vararg(struct argparser_argument const *arg) {
    char *result;
    if (arg->long_flag) {
        asprintf(&result, "%s", arg->long_flag);
    }
    else {
        asprintf(&result, "%s", arg->flag);
    }
    char *ptr = result;
    while (*ptr) {
        *ptr = (*ptr == '-') ? '_' : toupper(*ptr);
        ptr++;
    }
    return result;
}

static char *get_narg_optional(char const *vararg) {
    char *result;
    size_t size = strlen(vararg) + 3;
    result = malloc(size);
    result[0] = '[';
    strcpy(result + 1, vararg);
    strcpy(result + size - 2, "]");
    return result;
}

static char *get_narg_list_n(char const *vararg, size_t n) {
    char *result;
    size_t size = strlen(vararg) * n + (n - 1) + 1;
    result = malloc(size);
    for (size_t i = 0; i < n; i++) {
        strcpy(result + ((strlen(vararg) + 1) * i), vararg);
        result[strlen(vararg) * (i + 1) + i] = ' ';
    }
    result[size - 1] = 0;
    return result;
}

static char *get_narg_list(char const *vararg) {
    char *result;
    size_t size = strlen(vararg) * 2 + 7;
    result = malloc(size);
    strcpy(result, vararg);
    strcpy(result + strlen(vararg), " [");
    strcpy(result + strlen(vararg) + 2, vararg);
    strcpy(result + size - 5, "...]");
    return result;
}

static char *get_narg_list_optional(char const *vararg) {
    char *result = get_narg_list(vararg);
    result = get_narg_optional(result);
    return result;
}
