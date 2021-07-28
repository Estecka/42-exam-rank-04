/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   microshell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/26 18:55:20 by abaur             #+#    #+#             */
/*   Updated: 2021/07/28 19:27:08 by abaur            ###   ########.fr       */
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
static char**	GetCmdTerm(char*const* current){
	while (current != EOARG && !strcmp(*current, ";"))
		current++;
	return (char**)current;
}
/*
** Points to the terminating argument of the current process.
** This may be either ";", "|", or EOARG.
*/
static char**	GetPipeTerm(char*const* current){
	while (current != EOARG && !strcmp(*current, ";") && !strcmp(*current, "|"))
		current++;
	return (char**)current;
}

static pid_t	create_process(char*const* argbegin, char*const* argend){
	char*	argv[1 + argend - argbegin];
	for(char*const*src=argbegin,**dst=argv; src!=argend; src++,dst++)
		*dst = *src;
	argv[argend - argbegin] = NULL;

	int pid = fork();
	if (!pid){
		execve(argv[0], argv, g_environ);
		throw(EXIT_FAILURE, "cannot execute ", argv[0]);
	}

	return pid;
}

static int	exec_pipechain(char*const* chainbegin, char*const* chainend){
	pid_t	lastpid;

	char*const*	procbegin = chainbegin;
	char*const*	procend   = GetPipeTerm(chainbegin);
	while (1) {
		lastpid = create_process(procbegin, procend);

		if (procend == chainend)
			break;
		procbegin = ++procend;
		procend   = GetPipeTerm(procbegin);
	}

	int	status;
	waitpid(lastpid, &status, 0);
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
			exec_pipechain(argbegin, argend);
		if (argend == EOARG || ++argend == EOARG)
			break;
		argbegin = argend;
		argend   = GetCmdTerm(argbegin);
	}

	return EXIT_SUCCESS;
}
