#pragma once

#include <stddef.h>

extern bool basename(const char *restrict filename, size_t s, char dest[restrict s]);

extern bool isnumber(const char *str);
