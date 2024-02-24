#pragma once

#include <buracchi/argparser/types.h>

/*******************************************************************************
 * 				                Exiting functions *
 ******************************************************************************/

/**
 * @brief Prints a usage message incorporating the message to stderr and exits.
 * @param argparser The argparser instance.
 * @param message The message to be printed after the usage message.
 */
extern void argparser_error(argparser_t argparser, const char *message);

/**
 * @brief Prints the message and terminates the program with the specified exit
 * status.
 *
 * @param argparser The argparser instance.
 * @param status The exit status code.
 * @param message The message to be printed before exiting.
 */
extern void argparser_exit_status_message(argparser_t argparser, int status, const char *message);

/**
 * @brief Exits the program with the specified exit status.
 *
 * @param argparser The argparser instance.
 * @param status The exit status code.
 */
static inline void argparser_exit_status(argparser_t argparser, int status) {
	argparser_exit_status_message(argparser, status, nullptr);
}

/**
 * @brief Prints the message and terminates the program.
 *
 * @param argparser The argparser instance.
 * @param message The message to be printed before exiting.
 */
static inline void argparser_exit_message(argparser_t argparser, const char *message) {
	argparser_exit_status_message(argparser, 0, message);
}

/**
 * @brief Terminates the program with default exit behavior.
 *
 * @param argparser The argparser instance.
 */
static inline void argparser_exit1(argparser_t argparser) {
	argparser_exit_status_message(argparser, 0, nullptr);
}

/**
 * @brief Terminates the program, exiting with the specified status and,
 * if given, it prints a message before that.
 * The user can override the exit behavior using the function
 * 'argparser_set_exit_function'.
 *
 * @param argparser: An argparser instance.
 * @param status: An int exit status code (default is 0).
 * @param message: A const char* message to be printed before exiting
 *                 (default is none).
 */
#define argparser_exit(argparaser, ...)                                                   \
	argparser_exitN(argparaser __VA_OPT__(, ) __VA_ARGS__, _status_message, 2, 1, 0)( \
		argparaser __VA_OPT__(, ) __VA_ARGS__)

#define argparser_exitN(N3, N2, N1, N, ...) argparser_exit##N

#define argparser_exit2(argparser, value)                                      \
	_Generic((value),                                                      \
	        int: argparser_exit_status,                                    \
	        char *: argparser_exit_message,                                \
	        const char *: argparser_exit_message)(argparser, value)
