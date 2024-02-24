#include "arguments.h"

#include <ctype.h>

#include <buracchi/common/utilities/utilities.h>

static inline char *get_flag_vararg(const struct argparser_argument *arg);
static inline char *get_narg_optional(const char *vararg);
static inline char *get_narg_list_n(const char *vararg, size_t n);
static inline char *get_narg_list(const char *vararg);
static inline char *get_narg_list_optional(const char *vararg);

extern char *get_arg_string(const struct argparser_argument *arg) {
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
			return nullptr;
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
		return nullptr;
	}
}

extern inline size_t get_arg_string_len(const struct argparser_argument *arg) {
	size_t vararg_len = 0;
	bool is_positional = arg->name;
	if (is_positional) {
		vararg_len += strlen(arg->name);
	}
	else {
		bool needs_arg = (arg->action == ARGPARSER_ACTION_STORE) ||
		                 (arg->action == ARGPARSER_ACTION_APPEND) ||
		                 (arg->action == ARGPARSER_ACTION_EXTEND);
		if (!needs_arg) {
			return 0;
		}
		vararg_len += arg->long_flag ? strlen(arg->long_flag) : strlen(arg->flag);
	}
	switch (arg->action_nargs) {
	case ARGPARSER_ACTION_NARGS_SINGLE:
		return vararg_len;
	case ARGPARSER_ACTION_NARGS_OPTIONAL:
		return strlen("[") + vararg_len + strlen("]");
	case ARGPARSER_ACTION_NARGS_LIST_OF_N:
		return arg->nargs_list_size * (vararg_len + strlen(" ")) - strlen(" ");
	case ARGPARSER_ACTION_NARGS_LIST:
		return vararg_len + strlen(" [") + vararg_len + strlen("...]");
	case ARGPARSER_ACTION_NARGS_LIST_OPTIONAL:
		return strlen("[") + vararg_len + strlen(" [") + vararg_len +
		       strlen("...]") + strlen("]");
	default:
		return 0;
	}
}

static inline char *get_flag_vararg(const struct argparser_argument *arg) {
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

static inline char *get_narg_optional(const char *vararg) {
	char *result;
	size_t size = strlen(vararg) + 3;
	result = malloc(size);
	result[0] = '[';
	strcpy(result + 1, vararg);
	strcpy(result + size - 2, "]");
	return result;
}

static inline char *get_narg_list_n(const char *vararg, size_t n) {
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

static inline char *get_narg_list(const char *vararg) {
	char *result;
	size_t size = strlen(vararg) * 2 + 7;
	result = malloc(size);
	strcpy(result, vararg);
	strcpy(result + strlen(vararg), " [");
	strcpy(result + strlen(vararg) + 2, vararg);
	strcpy(result + size - 5, "...]");
	return result;
}

static inline char *get_narg_list_optional(const char *vararg) {
	char *result = get_narg_list(vararg);
	result = get_narg_optional(result);
	return result;
}
