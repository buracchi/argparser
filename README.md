# argparser

The argparser library is a command-line argument parser heavily inspired by Python's `argparse` module.

The argparser library makes it easy to write user-friendly command-line interfaces.
The program defines what arguments it requires, and argparse will figure out how to parse those out of `argv`.
The argparser library also automatically generates help and usage messages.
The library will also issue errors when users give the program invalid arguments.

## Core Functionality

The argparse library's support for command-line interfaces is built around the opaque type `argparser_t`.
An instance of type `argparser_t` is a container for argument specifications and has options that apply the parser
as whole:

```c
argparser_t parser = argparser_init(argc, argv);
argparser_set_program_name(parser, "ProgramName");
argparser_set_description(parser, "What the program does");
argparser_set_epilog(parser, "Text at the bottom of help");
```

The `argparser_add_argument()` macro attaches individual argument specifications to the parser.
It supports positional arguments, options that accept values, and on/off flags:

```c
char* filename = NULL;
int count = NULL;
bool verbose = false;
argparser_add_argument(parser, &filename, { .name = "filename" });
argparser_add_argument(parser, &count, { .flag = "c", .long_flag = "count" });
argparser_add_argument_action_store_true(parser, 
                                         &verbose, 
                                         { .flag = "v", 
                                           .long_flag = "verbose" 
                                         });
```

The `argparser_parse_args()` function runs the parser and places the extracted data in the declared memory positions:

```c
argparser_parse_args(parser);
argparser_destroy(parser);
printf("%s %d %s\n", filename, count, verbose ? "true" : "false");
```
