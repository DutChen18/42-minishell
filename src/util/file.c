/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   file.c                                           -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/05/10 12:00:39 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/05/10 12:00:39 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include <unistd.h>
#include <errno.h>

void
	sh_fdctl(int fd, int flag, int on)
{
	sh_assert(fd <= OPEN_MAX);
	if (fd < 0)
		return ;
	sh()->fd_flags[fd] |= flag * !!on;
}

void
	sh_fd_before_exec(void)
{
	size_t	index;
	int		*flags;

	index = 0;
	flags = sh()->fd_flags;
	while (index < OPEN_MAX)
	{
		if (flags[index] & SH_FD_FIOCLEX)
		{
			close(index);
			sh_assert(!(errno <= 0 && errno != EBADF));
		}
		index++;
	}
}
