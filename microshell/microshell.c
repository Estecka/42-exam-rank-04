/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   microshell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/26 18:55:20 by abaur             #+#    #+#             */
/*   Updated: 2021/07/31 21:08:17 by abaur            ###   ########.fr       */
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
static void	errlog(const char* arg1, const char* arg2){
	write(STDERR_FILENO, "error: ", 7);
	if (arg1) write(STDERR_FILENO, arg1, strlen(arg1));
	if (arg2) write(STDERR_FILENO, arg2, strlen(arg2));
	write(STDERR_FILENO, "\n", 1);
}

static noreturn void	throw(int status, const char* arg1, const char* arg2){
	errlog(arg1, arg2);
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


static pid_t	create_process(char*const* argbegin, char*const* argend, pid_t pipefd[3]){
	char*	argv[1 + argend - argbegin];
	for(char*const*src=argbegin,**dst=argv; src!=argend; src++,dst++)
		*dst = *src;
	argv[argend - argbegin] = NULL;

	int pid = fork();
	if (!pid){
		for (int i=0; i<2; ++i)
		if (pipefd[i])
			dup2(pipefd[i], i);
		for (int i=0; i<3; ++i)
		if (pipefd[i])
			close(pipefd[i]);

		execve(argv[0], argv, g_environ);


		throw(EXIT_FAILURE, "cannot execute ", argv[0]);
	}

	return pid;
}

static int	exec_pipechain(char*const* chainbegin, char*const* chainend){
	pid_t	lastpid;
	pid_t	pipefds[4] = {0, 0, 0, 0};

	char*const*	procbegin = chainbegin;
	char*const*	procend   = GetPipeTerm(chainbegin);
	while (1) {
		if (procend != chainend){
			pipe(pipefds+2);
			pipefds[1] = pipefds[3];
			pipefds[3] = 0;
		}

		lastpid = create_process(procbegin, procend, pipefds);

		for (int i=0; i<2; ++i)
		if(pipefds[i])
			close(pipefds[i]);
		pipefds[0] = pipefds[2];
		pipefds[1] = 0;
		pipefds[2] = 0;

		if (procend == chainend)
			break;
		procbegin = ++procend;
		procend   = GetPipeTerm(procbegin);
	}

	int	status;
	waitpid(lastpid, &status, 0);
	return WEXITSTATUS(status);
}

static int	exec_cd(char*const* argv, char*const* argend){
	if (argend-argv != 2)
		return errlog("cd: bad arguments", NULL), EXIT_FAILURE;

	if (chdir(argv[1]) < 0)
		return errlog("cd: cannot change directory to ", argv[0]), EXIT_FAILURE;

	return EXIT_SUCCESS;
}

extern int	main(int argc, char** argv, char** environ){
	if (argc < 2)
		return EXIT_SUCCESS;

	EOARG = argv + argc;
	g_environ = environ;

	char** argbegin = argv+1;
	char** argend = GetCmdTerm(argbegin);
	while (1) {
		if (argbegin != argend){
			if (strcmp("cd", *argbegin))
				exec_cd(argbegin, argend);
			else
				exec_pipechain(argbegin, argend);
		}
		if (argend == EOARG || ++argend == EOARG)
			break;
		argbegin = argend;
		argend   = GetCmdTerm(argbegin);
	}

	return EXIT_SUCCESS;
}
