/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   microshell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/20 18:19:00 by abaur             #+#    #+#             */
/*   Updated: 2021/07/26 18:44:30 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lselt.h"
#include "parser.h"

#include "unistd.h"
#include "stdlib.h"
#include "stdio.h"

extern int	main(int argc, char** argv){
	lselt*	commands = NULL;
	EOARG = argv + argc;

	commands = ParseCommands(argv+1);

	lseltlog(commands);
	lseltdelete(commands);
}
