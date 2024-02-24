#ifndef BURACCHI_ARGPARSER_H_INCLUDED
#define BURACCHI_ARGPARSER_H_INCLUDED
/** @file
 *
 * @brief Argparser interface.
 * @details This file represent the public interface of the Argparser module.
 * The argparse module makes it easy to write user-friendly command-line
 * interfaces. The program defines what arguments it requires, and argparse will
 * figure out how to parse those out of argv. The argparse module also
 * automatically generates help and usage messages and issues errors when users
 * give the program invalid arguments.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include <buracchi/argparser/argument-action-store.h>
#include <buracchi/argparser/exit.h>
#include <buracchi/argparser/sub-commands.h>
#include <buracchi/argparser/types.h>

static const char argparser_suppress[] = "==SUPPRESS==";

/**
 * Initializes a new instance of the `argparser_t` structure.
 *
 * @param argc The number of elements in the `argv` array.
 * @param argv An array of command-line arguments passed to the program.
 *
 * @return A new instance of the `argparser_t` structure.
 */
extern argparser_t argparser_init(int argc, const char *argv[argc + 1]);

/**
 * @brief Free memory associated to an initialized argparser_t structure.
 *
 * @param argparser
 * @return int
 */
extern void argparser_destroy(argparser_t argparser);

/**
 * @brief Set the argument parser object program name.
 *
 * @details By default argparser objects use argv[0] to determine
 * how to display the name of the program in help messages.
 * This default is almost always desirable because it will make the
 * help messages match how the program was invoked on the command line.
 * It is otherwise possible to specify an hardcoded string as the
 * program name using this function.
 *
 * @param argparser the argument parser object.
 * @param program_name The name of the program (default: basename(argv[0])).
 */
extern void argparser_set_program_name(argparser_t argparser, const char *program_name);

/**
 * @brief Set the argument parser object usage message.
 *
 * @details By default, argparser objects calculate the usage
 * message from the arguments they contain.
 * It is otherwise possible to specify an hardcoded string as the
 * usage message using this function.
 *
 * @param argparser the argument parser object.
 * @param usage The string describing the program usage.
 */
extern void argparser_set_usage(argparser_t argparser, const char *usage);

/**
 * @brief Set the Text to display before the argument help.
 *
 * @details Via this function it is possible to specify a brief description
 * of what the program does and how it works.
 * In help messages, the description is displayed between the command-line
 * usage string and the help messages for the various arguments.
 *
 * @param argparser the argument parser object.
 * @param description The string to display before the argument help.
 */
extern void argparser_set_description(argparser_t argparser, const char *description);

#define argparser_add_argument argparser_add_argument_action_store

extern int argparser_add_argument_action_store_true(argparser_t argparser,
                                                    bool *result,
                                                    struct argparser_argument argument);

extern int argparser_add_argument_action_store_false(argparser_t argparser,
                                                     bool *result,
                                                     struct argparser_argument argument);

#define argparser_parse_argsN(N3, N2, N1, N, ...) argparser_parse_args##N

extern int argparser_parse_args1(argparser_t argparser);

#define argparser_parse_args2(argparser, args)                                 \
	argparser_parse_args3((argparser), (args), (sizeof(args) / sizeof(char *)))

extern int argparser_parse_args3(argparser_t argparser, const char *args[const], size_t args_size);

/**
 * @brief Convert argument strings to objects.
 *
 * @details Previous calls to argparser_add_argument() determine exactly
 * what objects are created and how they are assigned.
 *
 * @param argparaser
 * @param args (optional) if unspecified it will be used the argc and
 * argv param specified in the argparser_init() call.
 * @param args_size (optional) if unspecified it will be used the sizeof
 * operator to infer the args elements number.
 *
 * @returns 0 on success, 1 otherwise.
 */
#define argparser_parse_args(argparaser, ...)                                     \
	argparser_parse_argsN(argparaser __VA_OPT__(, ) __VA_ARGS__, 3, 2, 1, 0)( \
		argparaser __VA_OPT__(, ) __VA_ARGS__)

/*******************************************************************************
 * 				           Help-formatting functions
 **
 ******************************************************************************/

/**
 * @brief Return the length of the usage string in byte including the nullptr
 * terminator.
 * @param argparser the argument parser object.
 * @return -1 on error, the length of the usage string otherwise.
 */
extern size_t argparser_usage_size(argparser_t argparser);

/**
 * @brief Return a string containing a brief description of how the
 * ArgumentParser should be invoked on the command line.
 * Write at most buffer_size bytes (including the nullptr terminator) to buffer.
 */
extern int argparser_nformat_usage(argparser_t argparser, char *buffer, size_t buffer_size);

/**
 * @brief Return a string containing a brief description of how the
 * ArgumentParser should be invoked on the command line.
 */
extern char *argparser_format_usage(argparser_t argparser);

/**
 * @brief Return a string containing a help message, including the program usage
 * and information about the arguments registered with the ArgumentParser.
 */
extern char *argparser_format_help(argparser_t argparser);

/*******************************************************************************
 * 				            Help-printing functions
 **
 ******************************************************************************/

/**
 * Printing help
 *
 * In most typical applications, argparser_parse_args() will take care of
 * formatting and printing any usage or error messages However, several
 * formatting functions are available
 */

#define argparser_print_usageN(N2, N1, N, ...) argparser_print_usage##N

#define argparser_print_usage1(argparser)                                      \
	argparser_print_usage2(argparser, stdout)

extern int argparser_print_usage2(argparser_t argparser, FILE *file);

/**
 * @brief Print a brief description of how the ArgumentParser should be invoked
 * on the command line. If file is unspecified stdout is assumed.
 *
 * @param argparser the argument parser object.
 * @param file the file to write the usage message to.
 */
#define argparser_print_usage(argparaser, ...)                                  \
	argparser_print_usageN(argparaser __VA_OPT__(, ) __VA_ARGS__, 2, 1, 0)( \
		argparaser __VA_OPT__(, ) __VA_ARGS__)

#define argparser_print_helpN(N2, N1, N, ...) argparser_print_help##N

#define argparser_print_help1(argparser)                                       \
	argparser_print_help2(argparser, stdout)

extern int argparser_print_help2(argparser_t argparser, FILE *file);

/**
 * @brief Print a help message, including the program usage and information
 * about the arguments registered with the ArgumentParser. If file is
 * unspecified stdout is assumed.
 *
 * @param argparser the argument parser object.
 * @param file the file to write the help message to.
 */
#define argparser_print_help(argparaser, ...)                                  \
	argparser_print_helpN(argparaser __VA_OPT__(, ) __VA_ARGS__, 2, 1, 0)( \
		argparaser __VA_OPT__(, ) __VA_ARGS__)

/*******************************************************************************
 * 				                  Other utilities
 **
 ******************************************************************************/

extern const char *argparser_get_program_name(argparser_t argparser);

/**
 * TODO:
 *  * @var argparser::epilog
 *		@brief Text to display after the argument help
 *		@details By default, the epilog will be line-wrapped so that it
 *fits within the given space. To change this behavior, see the
 *		@ref argparser::formatter argument.
 *		(default: none)
 *
 * @var argparser::parents
 *		@brief A nullptr terminated list of argparser object references
 *		whose arguments should also be included
 *		@details Sometimes, several parsers share a common set of
 *arguments. Rather than repeating the definitions of these arguments, a single
 *		parser with all the shared arguments passed via the
 *		@ref argparser::parents argument to the argparser objects
 *can be used. It takes a list of argparser object references, collects all
 *the positional and optional actions from them, and adds these actions to the
 *argparser object being constructed. Note that most parent parsers will
 *specify argparser::add_help as false. Otherwise, the ArgumentParser will
 *see two -h/--help options (one in the parent and one in the child) and raise
 *an error. Note You must fully initialize the parsers before passing them via
 *		@ref argparser::parents.
 *		If you change a parent parsers after the child parser
 *initialization, those changes will not be reflected in the child.
 *
 * @var argparser::formatter
 *		@brief A class for customizing the help output
 *		@details Currently undocumented
 *
 * @var argparser::prefix_chars
 *		@brief The set of characters that prefix optional arguments
 *		@details Most command-line options will use - as the prefix,
 *e.g. -f/--foo. Parsers that need to support different or additional prefix
 *characters, e.g. for options like +f or /foo, may specify them using this
 *argument. The prefix_chars argument defaults to '-'. Supplying a set of
 *characters that does not include - will cause -f/--foo options to be
 *disallowed.
 *
 * @var argparser::fromfile_prefix_chars
 *		@brief The set of characters that prefix files from which
 *		additional arguments should be read
 *		@details Currently undocumented
 *
 * @var argparser::argument_default
 *		@brief The global default value for arguments
 *		@details Currently undocumented
 *
 * @var argparser::conflict_handler
 *		@brief The strategy for resolving conflicting optionals (usually
 *unnecessary)
 *		@details Currently undocumented
 *
 * @var argparser::add_help
 *		@brief Add a -h/--help option to the parser
 *		@details By default, argparser objects add an option which
 *simply displays the parser’s help message. Occasionally, it may be useful to
 *disable the addition of this help option. The help option is typically
 *-h/--help. The exception to this is if the
 *		@ref argparser::prefix_chars is specified and does not include
 *-, in which case -h and --help are not valid options. (default: true)
 *
 * @var argparser::allow_abbrev
 *		@brief Allows long options to be abbreviated if the abbreviation
 *is unambiguous.
 *		@details Currently undocumented
 *
 * @var exit_on_error
 *		@brief Determines whether or not the program should exits
 *		with error info when an error occurs.
 *		@details Normally, when you invoke argparser_parse_args() with
 *a argparser object containing an invalid argument list, the program will
 *exit with error info. If the user would like to catch errors manually, the
 *feature can be enabled by setting @ref argparser::exit_on_error to false
 *		(default: true)
 */
#endif // BURACCHI_ARGPARSER_H_INCLUDED
