#pragma once

#include <stddef.h>

typedef struct argparser *argparser_t;

/**
 * @enum argparser_action
 *
 * @brief Methods of handling command line arguments.
 *
 * @var argparser_action::ARGPARSER_ACTION_STORE
 *      @brief stores the argument's value
 *      @details (e.g. '--foo 1' will be stored as a KV pair {"foo","1"})
 *
 * @var argparser_action::ARGPARSER_ACTION_STORE_CONST
 *      @brief stores the value specified by the const_value field
 *      @details (e.g. '--foo'   will be stored as a KV pair
 *{"foo","argparser_argument::const_value"})
 *
 * @var argparser_action::ARGPARSER_ACTION_STORE_TRUE
 *      @brief special cases of @ref ARGPARSER_ACTION_STORE_CONST
 *		used for storing the value true
 *		@details set the default values to true
 *
 * @var argparser_action::ARGPARSER_ACTION_STORE_FALSE
 *      @brief special cases of @ref ARGPARSER_ACTION_STORE_CONST
 *		used for storing the value false
 *		@details set the default values to false
 *
 * @var argparser_action::ARGPARSER_ACTION_APPEND
 *      @brief stores a list, and appends each argument value to the list
 *      @details It is useful to allow an option to be specified multiple times.
 *		If the default value is non-empty, the default elements will be
 *present in the parsed value for the option, with any values from the command
 *line appended after those default values. (e.g. '--foo 1 --foo 2' will be
 *stored as a KV pair {"foo",{"1", "2", nullptr}})
 *
 * @var argparser_action::ARGPARSER_ACTION_APPEND_CONST
 *      @brief stores a list, and appends the value specified by the const_value
 *		argument to the list.
 *		@details It is useful when multiple arguments need to store
 *constants to the same list.
 *
 * @var argparser_action::ARGPARSER_ACTION_COUNT
 *      @brief counts the number of times a keyword argument occurs
 *      @details (e.g. '-vvv' will be stored as a KV pair {"v",3})
 *
 * @var argparser_action::ARGPARSER_ACTION_HELP
 *      @brief This prints a complete help message for all the options in the
 *      current parser and then exits.
 *      @details By default a help action is automatically added to the parser.
 *
 * @var argparser_action::ARGPARSER_ACTION_VERSION
 *      @brief This expects a .version=keyword argument in the
 *		argparser_add_argument() call, and prints version information
 *and exits when invoked.
 *
 * @var argparser_action::ARGPARSER_ACTION_EXTEND
 *      @brief stores a list, and extends each argument value to the list
 *      @details (e.g. '--foo f1 --foo f2 f3 f4' will be stored as a KV pair
 *{"foo",{"f1", "f2", "f3", "f4", nullptr}})
 *
 * @var argparser_action::ARGPARSER_BOOLEAN_OPTIONAL
 *      @brief add support for boolean actions such as --foo and --no-foo.
 *      @details (e.g. --no-foo' will be stored as a KV pair {"foo", false})
 *
 */
enum argparser_action {
	ARGPARSER_ACTION_STORE,
	ARGPARSER_ACTION_STORE_CONST,
	ARGPARSER_ACTION_STORE_TRUE,
	ARGPARSER_ACTION_STORE_FALSE,
	ARGPARSER_ACTION_APPEND,
	ARGPARSER_ACTION_APPEND_CONST,
	ARGPARSER_ACTION_COUNT,
	ARGPARSER_ACTION_VERSION,
	ARGPARSER_ACTION_HELP,
	ARGPARSER_ACTION_EXTEND,
	ARGPARSER_ACTION_BOOLEAN_OPTIONAL
};

/**
 * @enum argparser_action_nargs
 *
 * @brief Methods of associating command line arguments with a single action.
 *
 * @var argparser_action::ARGPARSER_ACTION_NARGS_SINGLE
 *      @brief One argument will be consumed from the command line and produced
 *       as a single item.
 *
 * @var argparser_action::ARGPARSER_ACTION_NARGS_OPTIONAL
 *      @brief One argument will be consumed from the command line if possible,
 *       and produced as a single item.
 *      @details If no command-line argument is present, the value from @ref
 *       default_value will be produced. Note that for optional arguments, there
 *       is an additional case - the option string is present but not followed
 *       by a command-line argument. In this case the value from @ref
 *       const_value will be produced.
 *
 * @var argparser_action::ARGPARSER_ACTION_NARGS_LIST_OF_N
 *      @brief N arguments from the command line will be gathered together into
 *       a nullptr terminated list.
 *
 * @var argparser_action::ARGPARSER_ACTION_NARGS_LIST
 *      @brief special cases of @ref ARGPARSER_ACTION_STORE_CONST
 *
 * @var argparser_action::ARGPARSER_ACTION_NARGS_LIST_OPTIONAL
 *      @brief special cases of @ref ARGPARSER_ACTION_STORE_CONST
 *      @details the parsing won't fail if no argument is provided.
 *
 */
enum argparser_action_nargs {
	ARGPARSER_ACTION_NARGS_SINGLE,
	ARGPARSER_ACTION_NARGS_OPTIONAL,
	ARGPARSER_ACTION_NARGS_LIST_OF_N,
	ARGPARSER_ACTION_NARGS_LIST,
	ARGPARSER_ACTION_NARGS_LIST_OPTIONAL,
};

enum argparser_type {
	ARGPARSER_TYPE_BOOL,
	ARGPARSER_TYPE_CSTR,
	ARGPARSER_TYPE_INT,
	ARGPARSER_TYPE_LONG,
	ARGPARSER_TYPE_USHORT,
	ARGPARSER_TYPE_UINT,
};

/**
 * @struct argparser_argument
 *
 * @brief Argument item.
 *
 * @var argparser_argument::name
 *      @brief Specify the name of a positional argument (like a value or a list
 *       of filenames).
 *
 * @var argparser_argument::flag
 *      @brief Specify a character flag identifying an option which will be
 *       preceded by a hyphen.
 *      @details If name is not nullptr this field will be ignored during parsing.
 *
 * @var argparser_argument::long_flag
 *      @brief Specify a name flag identifying an option which will be preceded
 *       by two hyphens.
 *      @details If name is not nullptr this field will be ignored during parsing.
 *
 * @var argparser_argument::is_required
 *      @brief Specify if an option is required, @ref argparser_parse_args
 * will report an error if that option is not present at the command line.
 *      @details If name is not nullptr this field will be ignored during parsing.
 *       The default value of this field is false.
 *
 * @var argparser_argument::action
 *      @brief Specify how the argument should be handled.
 *      @details Argparser objects associate command-line arguments with
 *       actions. These actions can do just about anything with the command-line
 *       arguments associated with them, though most actions simply add a value
 *       into the map returned by \ref argparser_parse_args().
 *       The default action is @ref ARGPARSER_ACTION_STORE.
 *
 * @var argparser_argument::action_nargs
 *      @brief Specify how to associate different number of command-line
 *       arguments with a single action.
 *      @details If @ref ARGPARSER_ACTION_NARGS_SINGLE or
 *       @ref ARGPARSER_ACTION_NARGS_OPTIONAL are specified then the
 *       parser will produce a single item. If @ref
 *       ARGPARSER_ACTION_NARGS_LIST_OF_N or @ref
 *       ARGPARSER_ACTION_NARGS_LIST are specified the parser will gather
 *       together the arguments from the command line into a nullptr terminated
 *       string array.
 *
 * @var argparser_argument::nargs_list_size
 *      @brief Specify the number of elements that will be gathered together
 *       form the command line if @ref ARGPARSER_ACTION_NARGS_SINGLE is
 *       selected as the value of the @ref action_nargs field.
 *      @details This field is ignored if the @ref action_nargs field constains
 *       a value different from @ref ARGPARSER_ACTION_STORE.
 *
 * @var argparser_argument::const_value
 *      @brief Specify the value used when the action is set to
 *       @ref ARGPARSER_ACTION_STORE_CONST. The default value is nullptr.
 *
 * @var argparser_argument::default_value
 *      @brief Specify a pointer to the value that should be used if the
 *      command-line argument is not present.
 *      On error message will be generated if there wasn’t at least one
 *      command-line argument present if the argument was positional
 *      or if it was an option with the required field set to true.
 *
 * @var argparser_argument::choices
 *      @brief Specify a nullptr terminated array of acceptable values, @ref
 *       argparser_parse_args will report an error if the argument was not
 * one of the acceptable values.
 *
 * @var argparser_argument::help
 *      @brief A string containing a brief description of the argument showed
 *       when a user requests help.
 *
 * @var argparser_argument::destination
 *      @brief Specify the identifier that will be used to retrive the argument
 *       supplied.
 *      @details The destination attribute allows to retrive the argument
 *       supplied. For positional argument, the default value is equal to name.
 *       For optional argument, the default value is inferred from the flags
 *       strings taking the first long option string and stripping away the
 *       initial -- string. If no long option strings were supplied, dest will
 *       be derived from the first short option string by stripping the initial
 *       - character. Any internal - characters will be converted to _
 *       characters to make sure the string is a valid attribute name.
 */
struct argparser_argument {
	bool active;
	void **destination;
	enum argparser_action action;
	const char *name;
	const char *flag;
	const char *long_flag;
	bool is_required;
	enum argparser_action_nargs action_nargs;
	size_t nargs_list_size;
	void *const_value;
	void **default_value;
	char **choices;
	const char *help;
	enum argparser_type type;
};
