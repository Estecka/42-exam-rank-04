/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/26 18:35:36 by abaur             #+#    #+#             */
/*   Updated: 2021/07/26 18:42:50 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

#include <unistd.h>

char**	EOARG = NULL;

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

/*
** Generates list of argument for a process.
** @param const char*** argcursor	The first argument of the current process.
** 	On return, this will point to the terminating token.
*/
static lselt*	ParseArgs(char*** argcursor){
	lselt*	args = NULL;

	while (*argcursor != EOARG) {
		if (GetSyntaxType(**argcursor) != SYNTARG)
			return args;

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
		ilt = lseltpush(ilt, NULL);
		(**ilt).child = ParseArgs(argcursor);
		
		ESyntax term = GetSyntaxType(**argcursor);
		++*argcursor;
		if (term == SYNTSEMICOLUMN)
			break;
	}
	return processes;
}

/*
** Generates a list of commands from the argument list.
*/
extern lselt*	ParseCommands(char** argv) {
	lselt* 	commands = NULL;
	lselt**	ilt = &commands;

	while (argv != EOARG){
		ilt = lseltpush(ilt, NULL);
		(**ilt).child = ParsePipes(&argv);
	}

	return commands;
}
