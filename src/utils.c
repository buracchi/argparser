#include "utils.h"

#include <errno.h>
#include <stdlib.h>
#include <string.h>

extern bool basename(const char *filename, size_t s, char dest[s]) {
	char *first_slash;
	char *last_slash;
	bool filename_contains_slash;
	bool filename_contains_trailing_slash;
	bool filename_contains_only_slash;

	if (s < 1) {
		return false;
	}
	if (filename == nullptr || filename[0] == '\0') {
		if (s < sizeof ".") {
			return false;
		}
		strncpy(dest, ".", s);
		return true;
	}
	last_slash = strrchr(filename, '/');
	filename_contains_slash = (last_slash != nullptr);
	if (!filename_contains_slash) {
		if (s < strlen(filename) + 1) {
			return false;
		}
		strncpy(dest, filename, s);
		return true;
	}
	filename_contains_trailing_slash = (*(last_slash + 1) == '\0');
	if (!filename_contains_trailing_slash) {
		if (s < strlen(last_slash + 1) + 1) {
			return false;
		}
		strncpy(dest, last_slash + 1, s);
		return true;
	}
	first_slash = last_slash;
	while (first_slash > filename && *(first_slash - 1) == '/') {
		first_slash--;
	}
	filename_contains_only_slash = (first_slash == filename);
	if (filename_contains_only_slash) {
		if (s < sizeof "/") {
			return false;
		}
		strncpy(dest, "/", s);
		return true;
	}
	dest[0] = '\0';
	return true;
}

extern bool isnumber(const char *str) {
	char *end;
	errno = 0;
	const long double sld = strtold(str, &end);
	if (end == str || '\0' != *end) {
		return false;
	}
	return true;
}
