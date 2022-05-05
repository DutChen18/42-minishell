/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   main.c                                           -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/03/25 16:22:30 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/03/25 16:22:30 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include "lexer.h"
#include "input.h"
#include "commander.h"
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <libgen.h>

void
	run_start_scripts(void)
{
	t_input		in;
	int			fd;
	const char	*home;
	char		*rc;

	home = sh_getenv("HOME", NULL);
	if (home != NULL)
	{
		rc = ft_strjoin(home, "/.crashrc");
		fd = open(rc, O_RDONLY);
		if (fd >= 0)
		{
			input_new(&in, in_file, (void*)(unsigned long long) fd);
			sh_cm_run(&in);
			input_destroy(&in);
			close(fd);
		}
		free(rc);
	}	
}

int
	main(int argc, char **argv, char **envp)
{
	t_input		in;
	int			fd;

	sh_init(argv, envp);
	enable_signal_child_reaper_handler();
	run_start_scripts();
	if (argc >= 2)
	{
		fd = open(argv[1], O_RDONLY);
		sh_fdctl(fd, SH_FD_FIOCLEX, 1);
		sh_strlst_clear(sh()->args);
		sh()->args = sh_strlst_dup(argv + 1);
		sh()->interactive = 0;
		input_new(&in, in_file, (void *)(unsigned long long) fd);
	}
	else
		input_new(&in, in_readline, NULL);
	disable_kill_signals();
	sh_cm_run(&in);
	input_destroy(&in);
	sh_destroy();
	return (EXIT_SUCCESS);
}
