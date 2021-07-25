/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   microshell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/20 18:19:00 by abaur             #+#    #+#             */
/*   Updated: 2021/07/25 19:25:43 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lselt.h"

#include "unistd.h"
#include "stdlib.h"
#include "stdio.h"


typedef enum e_syntax	ESyntax;
enum e_syntax {
	SYNTARG        = 0,
	SYNTPIPE       = 1,
	SYNTSEMICOLUMN = 2,
};

static ESyntax	GetSyntaxType(const char* arg) {
	if (arg[0] == '\0' 
	||  arg[1] != '\0')
		return SYNTARG;

	if (arg[0] == '|')
		return SYNTPIPE;
	else
	if (arg[0] == ';')
		return SYNTSEMICOLUMN;
	else
		return SYNTARG;
}

static char**	EOARG;

/*
** Generates list of argument for a process.
** @param const char*** argcursor	The first argument of the current process.
** 	On return, this will point to the first argument in the next process.
*/
static lselt*	ParseArgs(char*** argcursor){
	lselt*	args = NULL;

	while (*argcursor != EOARG) {
		if (GetSyntaxType(**argcursor) != SYNTARG)
			return ++*argcursor, args;

		printf("\t\tArg %s\n", **argcursor);
		lseltpush(&args, **argcursor);
		++*argcursor;
	}
	return args;
}

/*
** Generates list of process for a command.
** @param const char*** argcursor	The first argument of the first process in the command.
** 	On return, this will point to the first argument in the next command.
*/
static lselt*	ParsePipes(char*** argcursor) {
	lselt* 	processes = NULL;
	lselt**	ilt = &processes;

	while(*argcursor != EOARG) {
		if (GetSyntaxType(**argcursor) == SYNTSEMICOLUMN)
			return ++*argcursor, processes;

		printf("\tPipe %s\n", **argcursor);
		*ilt = lseltpush(ilt, NULL);
		(**ilt).child = ParseArgs(argcursor);
	}
	return processes;
}

/*
** Generates a list of commands from the argument list.
*/
static lselt*	ParseCommands(char** argv) {
	lselt* 	commands = NULL;
	lselt**	ilt = &commands;

	while (argv != EOARG){
		printf("Command %s\n", *argv);
		*ilt = lseltpush(ilt, NULL);
		(**ilt).child = ParsePipes(&argv);
	}

	return commands;
}


extern int	main(int argc, char** argv){
	lselt*	commands = NULL;
	EOARG = argv + argc;

	commands = ParseCommands(argv);

	lseltlog(commands);
	lseltdelete(commands);
}
