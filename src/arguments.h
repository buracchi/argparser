#pragma once

#include <stddef.h>

#include "struct_argparser.h"

extern char *get_arg_string(const struct argparser_argument *arg);

extern size_t get_arg_string_len(const struct argparser_argument *arg);
