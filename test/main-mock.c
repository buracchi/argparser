#include "main-mock.h"

#include <string.h>

#include <buracchi/argparser/argparser.h>

extern int mock_main(int argc, char **argv) {
    char *arg;
    char *foo;
    argparser_t argparser;
    argparser = argparser_init(argc, argv);
    argparser_set_description(argparser, "An example program.");
    argparser_add_argument(argparser, &arg, { .name = "arg", .help = "an important argument" });
    argparser_add_argument(argparser,
                           &foo,
                           {
                               .flag = "f",
                               .long_flag = "foo",
                               .is_required = true,
                               .help = "set the amount of foo data"
                           });
    argparser_parse_args(argparser);
    argparser_destroy(argparser);
    return (strcmp(arg, "test") == 0 && strcmp(foo, "11") == 0) ? 0 : 1;
}

extern int test_parse_array_of_strings(int argc, char **argv) {
    char *arg;
    char *foo;
    argparser_t argparser;
    argparser = argparser_init(argc, argv);
    argparser_set_description(argparser, "An example program.");
    argparser_add_argument(argparser, &arg, { .name = "arg", .help = "an important argument" });
    argparser_add_argument(argparser,
                           &foo,
                           {
                               .flag = "f",
                               .long_flag = "foo",
                               .is_required = true,
                               .help = "set the amount of foo data"
                           });
    argparser_parse_args(argparser, ((char *[]) {"test", "-f", "11"}));
    argparser_destroy(argparser);
    return (strcmp(arg, "test") == 0 && strcmp(foo, "11") == 0) ? 0 : 1;
}

extern int test_action_store_type_uint(int argc, char **argv) {
    unsigned int arg;
    argparser_t argparser;
    argparser = argparser_init(argc, argv);
    argparser_set_description(argparser, "An example program.");
    argparser_add_argument(argparser, &arg, { .name = "arg" });
    argparser_parse_args(argparser, ((char *[]) {"11"}));
    argparser_destroy(argparser);
    return (arg == 11) ? 0 : 1;
}

extern int test_action_store_true(int argc, char **argv) {
    bool f = false;
    argparser_t argparser;
    argparser = argparser_init(argc, argv);
    argparser_set_description(argparser, "An example program.");
    argparser_add_argument_action_store_true(
        argparser,
        &f,
        (struct argparser_argument) {.flag = "f", .help = "set f true"});
    argparser_parse_args(argparser, ((char *[]) {"-f"}));
    argparser_destroy(argparser);
    return (f == true) ? 0 : 1;
}

extern int test_action_store_false(int argc, char **argv) {
    bool f = true;
    argparser_t argparser;
    argparser = argparser_init(argc, argv);
    argparser_set_description(argparser, "An example program.");
    argparser_add_argument_action_store_false(
        argparser,
        &f,
        (struct argparser_argument) {.flag = "f", .help = "set f true"});
    argparser_parse_args(argparser, ((char *[]) {"-f"}));
    argparser_destroy(argparser);
    return (f == false) ? 0 : 1;
}

extern int test_optional_args_are_position_independent(int argc, char **argv) {
    char *arg1;
    char *arg2;
    char *foo;
    bool result = true;
    argparser_t argparser;
    argparser = argparser_init(argc, argv);
    argparser_set_description(argparser, "An example program.");
    argparser_add_argument(argparser, &arg1, { .name = "arg1" });
    argparser_add_argument(argparser, &arg2, { .name = "arg2" });
    argparser_add_argument(argparser, &foo, { .flag = "f" });
    argparser_parse_args(argparser, ((char *[]) {"-f", "a", "b", "c"}));
    result &= (!strcmp(foo, "a") && !strcmp(arg1, "b") && !strcmp(arg2, "c"));
    argparser_parse_args(argparser, ((char *[]) {"a", "-f", "b", "c"}));
    result &= (!strcmp(foo, "b") && !strcmp(arg1, "a") && !strcmp(arg2, "c"));
    argparser_parse_args(argparser, ((char *[]) {"a", "b", "-f", "c"}));
    result &= (!strcmp(foo, "c") && !strcmp(arg1, "a") && !strcmp(arg2, "b"));
    argparser_destroy(argparser);
    return result ? 0 : 1;
}
