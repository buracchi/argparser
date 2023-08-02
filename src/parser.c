#include <buracchi/argparser/argparser.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "buracchi/common/utilities/try.h"
#include <buracchi/common/utilities/strto.h>
#include <buracchi/common/utilities/utilities.h>

#include "struct_argparser.h"
#include "utils.h"

static int parse_arg_n(argparser_t this,
                       size_t argc,
                       char *const *argv,
                       struct argparser_argument **argv_argument_links,
                       size_t n);

static struct argparser_argument *match_arg(argparser_t this,
                                            size_t argc,
                                            char const *args,
                                            struct argparser_argument **argv_argument_links);

static int parse_action_store(argparser_t this,
                              size_t argc,
                              char *const *argv,
                              struct argparser_argument **argv_argument_links,
                              size_t n,
                              struct argparser_argument *argument);

static int parse_action_store_const(argparser_t this,
                                    size_t argc,
                                    char *const *argv,
                                    struct argparser_argument **argv_argument_links,
                                    size_t n,
                                    struct argparser_argument *argument);

static int parse_action_help(argparser_t argparser);

static int handle_unrecognized_elements(argparser_t argparser,
                                        size_t argc,
                                        char *const *argv,
                                        struct argparser_argument **argv_argument_links);

static int handle_required_missing_elements(argparser_t argparser,
                                            size_t argc,
                                            struct argparser_argument **argv_argument_links,
                                            bool subcommand_parsed);

static int handle_optional_missing_elements(argparser_t this,
                                            size_t argc,
                                            struct argparser_argument **argv_argument_links);

static void convert_result(argparser_t argparser,
                           struct argparser_argument *argument,
                           char const *arg);

extern int argparser_parse_args1(argparser_t argparser) {
    return (argparser_parse_args3(argparser,
                                  (char **) argparser->sys_argv + 1,
                                  argparser->sys_argc - 1));
}

extern int argparser_parse_args3(argparser_t argparser, char *args[const], size_t args_size) {
    struct argparser_argument **arg_argument_relationships = NULL;
    size_t positional_params_number = 0;
    size_t positional_params_left;
    bool subcommand_parsed = false;
    try(arg_argument_relationships = malloc(args_size * sizeof *arg_argument_relationships),
        NULL,
        fail);
    for (size_t i = 0; i < args_size; i++) {
        arg_argument_relationships[i] = NULL;
    }
    for (size_t i = 0; i < argparser->arguments_number; i++) {
        if (argparser->arguments[i].name) {
            positional_params_number++;
        }
    }
    positional_params_left = positional_params_number;
    for (size_t i = 0; i < args_size; i++) {
        bool is_arg_parsed = arg_argument_relationships[i];
        if (is_arg_parsed) {
            continue;
        }
        if (positional_params_left == 0) {
            for (size_t j = 0; j < argparser->subparsers_number; j++) {
                if (!strcmp(args[i], argparser->subparsers[j].command_name)) {
                    *(argparser->subparsers[j].selection_result) =
                        args[i];
                    argparser_parse_args3(
                        argparser->subparsers[j].parser,
                        args + i + 1,
                        args_size - i - 1);
                    args_size = i;
                    subcommand_parsed = true;
                    goto end;
                }
            }
        }
        if (parse_arg_n(argparser, args_size, args, arg_argument_relationships, i)) {
            break;
        }
        if (arg_argument_relationships[i] &&
            arg_argument_relationships[i]->name) {
            positional_params_left--;
        }
        if (positional_params_left == 0 && arg_argument_relationships[i] == NULL) {
            argparser_print_usage(argparser);
            printf("%s: error: argument {",
                   argparser->program_name ?
                   argparser->program_name :
                   basename(argparser->sys_argv[0]));
            for (size_t j = 0; j < argparser->subparsers_number; j++) {
                printf("%s%s",
                       argparser->subparsers[j].command_name,
                       (j < argparser->subparsers_number - 1) ? "," : "");
            }
            printf("}: invalid choice: '%s' (choose from ", args[i]);
            for (size_t j = 0; j < argparser->subparsers_number; j++) {
                printf("'%s'%s",
                       argparser->subparsers[j].command_name,
                       (j < argparser->subparsers_number - 1) ? "," : "");
            }
            printf(")\n");
            exit(EXIT_FAILURE);
        }
    }
end:
    handle_unrecognized_elements(argparser, args_size, args, arg_argument_relationships);
    handle_required_missing_elements(argparser, args_size, arg_argument_relationships, subcommand_parsed);
    handle_optional_missing_elements(argparser, args_size, arg_argument_relationships);
    free(arg_argument_relationships);
    return 0;
fail:
    return 1;
}

static int parse_arg_n(argparser_t this,
                       size_t argc,
                       char *const *argv,
                       struct argparser_argument **argv_argument_links,
                       size_t n) {
    struct argparser_argument *matching_arg;
    matching_arg = match_arg(this, argc, argv[n], argv_argument_links);
    if (matching_arg) {
        switch (matching_arg->action) {
            case ARGPARSER_ACTION_HELP:
                return parse_action_help(this);
            case ARGPARSER_ACTION_STORE:
                return parse_action_store(this, argc, argv, argv_argument_links, n, matching_arg);
            case ARGPARSER_ACTION_STORE_CONST:
                return parse_action_store_const(this, argc, argv, argv_argument_links, n, matching_arg);
            case ARGPARSER_ACTION_APPEND:
                // TODO
                break;
            case ARGPARSER_ACTION_APPEND_CONST:
                // TODO
                break;
            case ARGPARSER_ACTION_COUNT:
                // TODO
                break;
            case ARGPARSER_ACTION_EXTEND:
                // TODO
                break;
            default:
                break;
        }
    }
    return 0;
}

static struct argparser_argument *match_arg(argparser_t this,
                                            size_t argc,
                                            char const *args,
                                            struct argparser_argument **argv_argument_links) {
    bool match_positional = (args[0] != '-');
    bool match_optional = !match_positional && args[1];
    bool match_long_flag = match_optional && (args[1] == '-') && args[2];
    for (size_t i = 0; i < this->arguments_number; i++) {
        bool is_argument_parsed = false;
        for (size_t j = 0; j < argc; j++) {
            if (argv_argument_links[j] == &(this->arguments[i])) {
                is_argument_parsed = true;
                break;
            }
        }
        if (is_argument_parsed) {
            continue;
        }
        if ((match_positional && this->arguments[i].name) ||
            (match_optional && this->arguments[i].flag &&
             streq(args + 1, this->arguments[i].flag)) ||
            (match_long_flag && this->arguments[i].long_flag &&
             streq(args + 2, this->arguments[i].long_flag))) {
            return &(this->arguments[i]);
        }
    }
    return NULL;
}

static int handle_unrecognized_elements(argparser_t argparser,
                                        size_t argc,
                                        char *const *argv,
                                        struct argparser_argument **argv_argument_links) {
    bool all_arguments_recognized = true;
    for (size_t i = 0; i < argc; i++) {
        if (argv_argument_links[i] == NULL) {
            all_arguments_recognized = false;
            break;
        }
    }
    if (all_arguments_recognized) {
        return 0;
    }
    argparser_print_usage(argparser);
    printf("%s: error: unrecognized arguments: ",
           argparser->program_name ? argparser->program_name :
           basename(argparser->sys_argv[0]));
    for (size_t i = 0; i < argc; i++) {
        if (argv_argument_links[i] == NULL) {
            printf("%s ", argv[i]);
        }
    }
    printf("\n");
    exit(EXIT_FAILURE);
    return 0;
}

static int handle_required_missing_elements(argparser_t argparser,
                                            size_t argc,
                                            struct argparser_argument **argv_argument_links,
                                            bool subcommand_parsed) {
    bool exists_missing_required_arg = false;
    bool is_first_missing_arg = true;
    if (argparser->subparsers_options.required && !subcommand_parsed) {
        exists_missing_required_arg = true;
    }
    else {
        for (size_t i = 0; i < argparser->arguments_number; i++) {
            struct argparser_argument *argument;
            bool is_arg_parsed = false;
            argument = &(argparser->arguments[i]);
            if (!argument->name && !argument->is_required) {
                continue;
            }
            for (size_t j = 0; j < argc; j++) {
                if (argument == argv_argument_links[j]) {
                    is_arg_parsed = true;
                    break;
                }
            }
            if (is_arg_parsed) {
                continue;
            }
            exists_missing_required_arg = true;
            break;
        }
    }
    if (!exists_missing_required_arg) {
        return 0;
    }
    argparser_print_usage(argparser);
    printf("%s: error: the following arguments are required: ",
           argparser->program_name ? argparser->program_name :
           basename(argparser->sys_argv[0]));
    for (size_t i = 0; i < argparser->arguments_number; i++) {
        struct argparser_argument *argument;
        bool is_arg_parsed = false;
        argument = &(argparser->arguments[i]);
        if (!argument->name && !argument->is_required) {
            continue;
        }
        for (size_t j = 0; j < argc; j++) {
            if (argument == argv_argument_links[j]) {
                is_arg_parsed = true;
                break;
            }
        }
        if (is_arg_parsed) {
            continue;
        }
        if (!argument->name) {
            printf("%s", is_first_missing_arg ? "" : ", ");
            printf("%s%s%s%s%s",
                   argument->flag ? "-" : "",
                   argument->flag ? argument->flag : "",
                   argument->flag && argument->long_flag ? "/" : "",
                   argument->long_flag ? "--" : "",
                   argument->long_flag ? argument->long_flag : "");
            is_first_missing_arg = false;
        }
    }
    for (size_t i = 0; i < argparser->arguments_number; i++) {
        struct argparser_argument *argument;
        bool is_arg_parsed = false;
        argument = &(argparser->arguments[i]);
        if (!argument->name && !argument->is_required) {
            continue;
        }
        for (size_t j = 0; j < argc; j++) {
            if (argument == argv_argument_links[j]) {
                is_arg_parsed = true;
                break;
            }
        }
        if (is_arg_parsed) {
            continue;
        }
        if (argument->name) {
            printf("%s", is_first_missing_arg ? "" : ", ");
            printf("%s", argument->name);
            is_first_missing_arg = false;
        }
    }
    if (argparser->subparsers_options.required && !subcommand_parsed) {
        printf("%s{", is_first_missing_arg ? "" : ", ");
        for (size_t i = 0; i < argparser->subparsers_number; i++) {
            printf("%s%s",
                   argparser->subparsers[i].command_name,
                   (i < argparser->subparsers_number - 1) ? "," : "");
        }
        printf("}");
    }
    printf("\n");
    exit(EXIT_FAILURE);
    return 0;
}

static int handle_optional_missing_elements(argparser_t this,
                                            size_t argc,
                                            struct argparser_argument **argv_argument_links) {
    for (size_t i = 0; i < this->arguments_number; i++) {
        struct argparser_argument *argument;
        bool is_arg_parsed = false;
        argument = &(this->arguments[i]);
        if (argument->name || argument->is_required) {
            continue;
        }
        for (size_t j = 0; j < argc; j++) {
            if (argument == argv_argument_links[j]) {
                is_arg_parsed = true;
                break;
            }
        }
        if (is_arg_parsed) {
            continue;
        }
        // TODO: handle all cases
        if (argument->action == ARGPARSER_ACTION_STORE) {
            if (argument->type == ARGPARSER_TYPE_CSTR) {
                *(argument->result) = argument->default_value;
            }
        }
    }
    return 0;
}

static int parse_action_help(argparser_t argparser) {
    argparser_print_help(argparser);
    exit(EXIT_SUCCESS);
    return 0;
}

static int parse_action_store(argparser_t this,
                              size_t argc,
                              char *const *argv,
                              struct argparser_argument **argv_argument_links,
                              size_t n,
                              struct argparser_argument *argument) {
    bool is_positional = argument->name;
    switch (argument->action_nargs) {
        case ARGPARSER_ACTION_NARGS_SINGLE: {
            char *arg;
            if (is_positional) {
                arg = argv[n];
            }
            else {
                if (n == argc - 1) {
                    return 1;
                }
                arg = argv[n + 1];
            }
            switch (argument->type) {
                case ARGPARSER_TYPE_BOOL:
                    // TODO
                    break;
                case ARGPARSER_TYPE_CSTR:
                    *(argument->result) = arg;
                    break;
                case ARGPARSER_TYPE_INT:
                case ARGPARSER_TYPE_LONG:
                case ARGPARSER_TYPE_USHORT:
                case ARGPARSER_TYPE_UINT:
                    convert_result(this, argument, arg);
                    break;
            }
            if (is_positional) {
                argv_argument_links[n] = argument;
            }
            else {
                argv_argument_links[n] = argument;
                argv_argument_links[n + 1] = argument;
            }
            break;
        }
        case ARGPARSER_ACTION_NARGS_OPTIONAL:
            // TODO
            break;
        case ARGPARSER_ACTION_NARGS_LIST_OF_N:
            // TODO
            break;
        case ARGPARSER_ACTION_NARGS_LIST:
            // TODO
            break;
        case ARGPARSER_ACTION_NARGS_LIST_OPTIONAL:
            // TODO
            break;
        default:
            break;
    }
    return 0;
}

static int parse_action_store_const(argparser_t this,
                                    size_t argc,
                                    char *const *argv,
                                    struct argparser_argument **argv_argument_links,
                                    size_t n,
                                    struct argparser_argument *argument) {
    bool is_positional = argument->name;
    switch (argument->action_nargs) {
        case ARGPARSER_ACTION_NARGS_SINGLE:
            if (is_positional) {
                // TODO
            }
            else {
                if (argument->type == ARGPARSER_TYPE_BOOL) {
                    *((bool *) (argument->result)) = (bool) (argument->const_value);
                }
                argv_argument_links[n] = argument;
            }
            break;
        case ARGPARSER_ACTION_NARGS_OPTIONAL:
            // TODO
            break;
        case ARGPARSER_ACTION_NARGS_LIST_OF_N:
            // TODO
            break;
        case ARGPARSER_ACTION_NARGS_LIST:
            // TODO
            break;
        case ARGPARSER_ACTION_NARGS_LIST_OPTIONAL:
            // TODO
            break;
        default:
            break;
    }
    return 0;
}

static void convert_result(argparser_t argparser,
                           struct argparser_argument *argument,
                           char const *arg) {
    static char const *
        type_names[] = {[ARGPARSER_TYPE_UINT] = "unsigned int", [ARGPARSER_TYPE_USHORT] = "unsigned short", [ARGPARSER_TYPE_LONG] = "long"};
    enum cmn_strto_error ret;
    char const *type_name;
    char const *arg_name;
    switch (argument->type) {
        case ARGPARSER_TYPE_INT:
            ret = cmn_strto_n((int *) argument->result, arg);
            break;
        case ARGPARSER_TYPE_LONG:
            ret = cmn_strto_n((long *) argument->result, arg);
            break;
        case ARGPARSER_TYPE_USHORT:
            ret = cmn_strto_n((unsigned short *) argument->result, arg);
            break;
        case ARGPARSER_TYPE_UINT:
            ret = cmn_strto_n((unsigned int *) argument->result, arg);
            break;
        default:
            return; // unreachable
    }
    type_name = type_names[argument->type];
    if (ret != CMN_STRTO_SUCCESS) {
        argparser_print_usage(argparser);
        if (argument->name) {
            arg_name = argument->name;
        }
        else {
            asprintf((char **) &arg_name,
                     "%s%s%s%s%s",
                     argument->flag ? "-" : "",
                     argument->flag ? argument->flag : "",
                     argument->flag && argument->long_flag ? "/" : "",
                     argument->long_flag ? "--" : "",
                     argument->long_flag ? argument->long_flag : "");
        }
        fprintf(stdout,
                "%s: error: argument %s: invalid %s value: '%s'",
                argparser->program_name ? argparser->program_name :
                basename(argparser->sys_argv[0]),
                arg_name,
                type_name,
                arg);
        fflush(stdout);
        exit(EXIT_FAILURE);
    }
}
