#pragma once

#include <buracchi/argparser/types.h>

/**
 * Many programs split up their functionality into a number of sub-commands,
 * for example, the svn program can invoke sub-commands like svn checkout,
 * svn update, and svn commit.
 * Splitting up functionality this way can be a particularly good idea when a
 * program performs several different functions which require different kinds
 * of command-line arguments.
 * ArgumentParser supports the creation of such sub-commands with the
 * add_subparsers() method.
 * The add_subparsers() method is normally called with no arguments and returns
 * a special action object.
 * This object has a single method, add_parser(), which takes a command name
 * and any ArgumentParser constructor arguments, and returns an ArgumentParser
 * object that can be modified as usual.
 */

struct argparser_subparsers_options {
	const char *title; /* title for the sub-parser group in help output; by
	                    default “subcommands” if description is provided,
	                    otherwise uses title for positional arguments */
	const char *description; /* description for the sub-parser group in help
	                          output, by default None */
	const char *prog; /* usage information that will be displayed with
	                         sub-command help, by default the name of the
	                         program and any positional arguments before the
	                         subparser argument */
	bool required; /* Whether a subcommand must be provided, by default False */
	const char *help;    /* help for sub-parser group in help output, by
	                            default None */
	const char *metavar; /* string presenting available sub-commands in
	                            help; by default it is None and presents
	                            sub-commands in form {cmd1, cmd2, ..}; */
};

extern void argparser_set_subparsers_options(argparser_t argparser,
                                             struct argparser_subparsers_options options);

/**
 *
 * @param argparser
 * @param selection_result pointer to a variable storing the user subcommand
 * selection
 * @param command_name
 * @param help default None
 * @return
 */
extern argparser_t argparser_add_subparser(argparser_t argparser,
                                           const char **selection_result,
                                           const char *command_name,
                                           const char *help);
