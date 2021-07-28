/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   microshell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/26 18:55:20 by abaur             #+#    #+#             */
/*   Updated: 2021/07/28 17:49:39 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdnoreturn.h>
#include <unistd.h>
#include <wait.h>

static char**	g_environ;
static char**	EOARG;

static size_t	strlen(const char* s){
	size_t	len = 0;
	while (*s)
		s++, len++;
	return len;
}

static short	strcmp(const char* s1, const char* s2){
	while (*s1 || *s2){
		if (*s1 != *s2)
			return 0;
		s1++, s2++;
	}
	return 1;
}

static noreturn void	throw(int status, const char* arg1, const char* arg2){
	write(STDERR_FILENO, "error: ", 7);
	write(STDERR_FILENO, arg1, strlen(arg1));
	write(STDERR_FILENO, arg2, strlen(arg2));
	write(STDERR_FILENO, "\n", 1);
	exit(status);
}

/*
** Points to the terminating argument of the current command.
** This may be either ";", or EOARG.
*/
static char**	GetCmdTerm(char** current){
	while (current != EOARG && !strcmp(*current, ";"))
			current++;
	return current;
}

static int	exec_process(char*const* argbegin, char*const* argend){
	char*	argv[1 + argend - argbegin];
	for(char*const*src=argbegin,**dst=argv; src!=argend; src++,dst++)
		*dst = *src;
	argv[argend - argbegin] = NULL;

	int pid = fork();
	if (!pid){
		execve(argv[0], argv, g_environ);
		throw(EXIT_FAILURE, "cannot execute ", argv[0]);
	}

	int	status;
	waitpid(pid, &status, 0);
	return WEXITSTATUS(status);
}

extern int	main(int argc, char** argv, char** environ){
	if (argc < 2)
		return EXIT_SUCCESS;

	EOARG = argv + argc;
	g_environ = environ;

	char** argbegin = argv+1;
	char** argend = GetCmdTerm(argbegin);
	while (1) {
		if (argbegin != argend)
			exec_process(argbegin, argend);
		if (argend == EOARG || ++argend == EOARG)
			break;
		argbegin = argend;
		argend   = GetCmdTerm(argbegin);
	}

	return EXIT_SUCCESS;
}
