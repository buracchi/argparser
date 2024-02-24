#include <buracchi/cutest/cutest.h>

#include <string.h>

#include <buracchi/argparser/argparser.h>

TEST(argparser, example_usage) {
	char *argv[] = { "./test_program", "test", "-f", "11" };
	int argc = sizeof argv / sizeof *argv;
	char *arg;
	char *foo;
	bool parsedCorrectly = false;
	argparser_t argparser;
	argparser = argparser_init(argc, argv);
	argparser_set_description(argparser, "An example program.");
	argparser_add_argument(argparser, &arg, { .name = "arg", .help = "an important argument" });
	argparser_add_argument(argparser,
	                       &foo,
	                       { .flag = "f",
	                         .long_flag = "foo",
	                         .is_required = true,
	                         .help = "set the amount of foo data" });
	argparser_parse_args(argparser);
	argparser_destroy(argparser);
	parsedCorrectly = (strcmp(arg, "test") == 0 && strcmp(foo, "11") == 0);
	ASSERT_EQ(parsedCorrectly, true);
}

TEST(argparser, parse_array_of_string) {
	char *argv[] = { "./test_program" };
	int argc = sizeof argv / sizeof *argv;
	char *arg;
	char *foo;
	bool parsedCorrectly = false;
	argparser_t argparser;
	argparser = argparser_init(argc, argv);
	argparser_set_description(argparser, "An example program.");
	argparser_add_argument(argparser, &arg, { .name = "arg", .help = "an important argument" });
	argparser_add_argument(argparser,
	                       &foo,
	                       { .flag = "f",
	                         .long_flag = "foo",
	                         .is_required = true,
	                         .help = "set the amount of foo data" });
	argparser_parse_args(argparser, ((char *[]){ "test", "-f", "11" }));
	argparser_destroy(argparser);
	parsedCorrectly = (strcmp(arg, "test") == 0 && strcmp(foo, "11") == 0);
	ASSERT_EQ(parsedCorrectly, true);
}

TEST(argparser, action_store_type_uint) {
	char *argv[] = { "./test_program" };
	int argc = sizeof argv / sizeof *argv;
	unsigned int arg;
	bool parsedCorrectly = false;
	argparser_t argparser;
	argparser = argparser_init(argc, argv);
	argparser_set_description(argparser, "An example program.");
	argparser_add_argument(argparser, &arg, { .name = "arg" });
	argparser_parse_args(argparser, ((char *[]){ "11" }));
	argparser_destroy(argparser);
	parsedCorrectly = (arg == 11);
	ASSERT_EQ(parsedCorrectly, true);
}

TEST(argparser, action_store_true) {
	char *argv[] = { "./test_program" };
	int argc = sizeof argv / sizeof *argv;
	bool f = false;
	argparser_t argparser;
	argparser = argparser_init(argc, argv);
	argparser_set_description(argparser, "An example program.");
	argparser_add_argument_action_store_true(
		argparser,
		&f,
		(struct argparser_argument){ .flag = "f", .help = "set f true" });
	argparser_parse_args(argparser, ((char *[]){ "-f" }));
	argparser_destroy(argparser);
	ASSERT_EQ(f, true);
}

TEST(argparser, action_store_false) {
	char *argv[] = { "./test_program" };
	int argc = sizeof argv / sizeof *argv;
	bool f = true;
	argparser_t argparser;
	argparser = argparser_init(argc, argv);
	argparser_set_description(argparser, "An example program.");
	argparser_add_argument_action_store_false(
		argparser,
		&f,
		(struct argparser_argument){ .flag = "f", .help = "set f true" });
	argparser_parse_args(argparser, ((char *[]){ "-f" }));
	argparser_destroy(argparser);
	ASSERT_EQ(f, false);
}

TEST(argparser, optional_args_are_position_independent) {
	char *argv[] = { "./test_program" };
	int argc = sizeof argv / sizeof *argv;
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
	argparser_parse_args(argparser, ((char *[]){ "-f", "a", "b", "c" }));
	result &= (!strcmp(foo, "a") && !strcmp(arg1, "b") && !strcmp(arg2, "c"));
	argparser_parse_args(argparser, ((char *[]){ "a", "-f", "b", "c" }));
	result &= (!strcmp(foo, "b") && !strcmp(arg1, "a") && !strcmp(arg2, "c"));
	argparser_parse_args(argparser, ((char *[]){ "a", "b", "-f", "c" }));
	result &= (!strcmp(foo, "c") && !strcmp(arg1, "a") && !strcmp(arg2, "b"));
	argparser_destroy(argparser);
	ASSERT_EQ(result, true);
}
