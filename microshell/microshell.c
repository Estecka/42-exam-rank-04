/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   microshell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/26 18:55:20 by abaur             #+#    #+#             */
/*   Updated: 2021/07/26 20:07:50 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdnoreturn.h>
#include <unistd.h>
#include <wait.h>

static char**	g_environ;

static size_t	strlen(const char* s){
	size_t	len = 0;
	while (*s)
		s++, len++;
	return len;
}

static void	errlog(char* arg1, char* arg2){
	write(STDERR_FILENO, "error: ", 7);
	write(STDERR_FILENO, arg1, strlen(arg1));
	write(STDERR_FILENO, arg2, strlen(arg2));
	write(STDERR_FILENO, "\n", 1);
}

static noreturn void	throw(int status){
	errlog("fatal", "");
	exit(status);
}

static int	exec_process(char*const* argbegin, char*const* argend){
	char*	argv[argend - argbegin];
	for(char*const*src=argbegin,**dst=argv; src!=argend; src++,dst++)
		*dst = *src;

	int pid = fork();
	if (!pid){
		execve(argv[0], argv, g_environ);
		errlog("cannot execute ", argv[0]);
		exit(EXIT_FAILURE);
	}

	int	status;
	wait(&status);
	return WEXITSTATUS(status);
}

extern int	main(int argc, char** argv, char** environ){
	g_environ = environ;
	exec_process(argv+1, argv+argc);
}
