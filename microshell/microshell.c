/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   microshell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/20 18:19:00 by abaur             #+#    #+#             */
/*   Updated: 2021/07/20 19:04:36 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lselt.h"

#include "unistd.h"
#include "stdlib.h"


enum ESyntax {
	SYNTUNDEFINED = 0,
	SYNTARG = 1,
	SYNTPIPE = 2,
	SYNTSEMICOLUMN = 3,
};

static enum ESyntax	GetSyntaxType(const char* arg) {
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


static lselt*	ParseArgs(char** cursor);
static lselt*	ParsePipes(char** cursor);
static lselt*	ParseCommands(char** cursor);


extern int	main(int argc, char** argv){
	char**	cursor;
	lselt*	commands;


}
