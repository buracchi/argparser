#pragma once

#include <buracchi/argparser/types.h>

extern int argparser_add_argument_action_store_cstr(argparser_t argparser,
                                                    char **result,
                                                    struct argparser_argument argument);

extern int argparser_add_argument_action_store_int(argparser_t argparser,
                                                   int *result,
                                                   struct argparser_argument argument);

extern int argparser_add_argument_action_store_long(argparser_t argparser,
                                                    long int *result,
                                                    struct argparser_argument argument);

extern int argparser_add_argument_action_store_ushort(argparser_t argparser,
                                                      unsigned short int *result,
                                                      struct argparser_argument argument);

extern int argparser_add_argument_action_store_uint(argparser_t argparser,
                                                    unsigned int *result,
                                                    struct argparser_argument argument);

#define argparser_add_argument_action_store(argparser, result, ...)               \
	_Generic((result),                                                        \
	        char **: argparser_add_argument_action_store_cstr,                \
	        int *: argparser_add_argument_action_store_int,                   \
	        long int *: argparser_add_argument_action_store_long,             \
	        unsigned short int *: argparser_add_argument_action_store_ushort, \
	        unsigned int *: argparser_add_argument_action_store_uint)(        \
		(argparser),                                                      \
		(result),                                                         \
		(struct argparser_argument)__VA_ARGS__)
