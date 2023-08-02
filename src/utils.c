#include "utils.h"

#include <stdbool.h>
#include <string.h>

extern char *basename(char *filename) {
    char *first_slash;
    char *last_slash;
    bool filename_contains_slash;
    bool filename_contains_trailing_slash;
    bool filename_contains_only_slash;
    if (filename == NULL || filename[0] == '\0') {
        return ".";
    }
    last_slash = strrchr(filename, '/');
    filename_contains_slash = (last_slash != NULL);
    if (!filename_contains_slash) {
        return filename;
    }
    filename_contains_trailing_slash = (*(last_slash + 1) == '\0');
    if (!filename_contains_trailing_slash) {
        return last_slash + 1;
    }
    first_slash = last_slash;
    while (first_slash > filename && *(first_slash - 1) == '/') {
        first_slash--;
    }
    filename_contains_only_slash = (first_slash == filename);
    if (filename_contains_only_slash) {
        return "/";
    }
    return "";
}
