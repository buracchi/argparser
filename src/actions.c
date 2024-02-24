#include <buracchi/argparser/argparser.h>

#include <stdlib.h>
#include <string.h>

#include <buracchi/common/utilities/try.h>

#include "struct_argparser.h"

extern int argparser_add_argument_action_store_cstr(argparser_t argparser,
                                                    char **result,
                                                    struct argparser_argument argument) {
	argparser->arguments_number++;
	argument.action = ARGPARSER_ACTION_STORE;
	argument.type = ARGPARSER_TYPE_CSTR;
	argument.destination = (void **)result;
	struct argparser_argument *reallocated_address;
	try(reallocated_address =
	            realloc(argparser->arguments,
	                    sizeof *argparser->arguments * argparser->arguments_number),
	    nullptr,
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
	argument.destination = (void **)result;
	struct argparser_argument *reallocated_address;
	try(reallocated_address =
	            realloc(argparser->arguments,
	                    sizeof *argparser->arguments * argparser->arguments_number),
	    nullptr,
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
	argument.destination = (void **)result;
	struct argparser_argument *reallocated_address;
	try(reallocated_address =
	            realloc(argparser->arguments,
	                    sizeof *argparser->arguments * argparser->arguments_number),
	    nullptr,
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
	argument.destination = (void **)result;
	struct argparser_argument *reallocated_address;
	try(reallocated_address =
	            realloc(argparser->arguments,
	                    sizeof *argparser->arguments * argparser->arguments_number),
	    nullptr,
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
	argument.destination = (void **)result;
	struct argparser_argument *reallocated_address;
	try(reallocated_address =
	            realloc(argparser->arguments,
	                    sizeof *argparser->arguments * argparser->arguments_number),
	    nullptr,
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
	argument.destination = (void **)result;
	argument.const_value = (void *)true;
	*result = false;
	struct argparser_argument *reallocated_address;
	try(reallocated_address =
	            realloc(argparser->arguments,
	                    sizeof *argparser->arguments * argparser->arguments_number),
	    nullptr,
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
	argument.destination = (void **)result;
	argument.const_value = (void *)false;
	*result = true;
	struct argparser_argument *reallocated_address;
	try(reallocated_address =
	            realloc(argparser->arguments,
	                    sizeof *argparser->arguments * argparser->arguments_number),
	    nullptr,
	    fail);
	argparser->arguments = reallocated_address;
	memcpy(&(argparser->arguments[argparser->arguments_number - 1]),
	       &argument,
	       sizeof *argparser->arguments);
	return 0;
fail:
	return 1;
}
