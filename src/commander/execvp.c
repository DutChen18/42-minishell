/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   execvp.c                                         -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/05/10 11:59:30 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/05/10 11:59:30 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "commander.h"
#include "memory.h"
#include "libft.h"
#include "minishell.h"
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

static void
	sh_free_list(char **list)
{
	size_t	i;

	i = 0;
	while (list[i] != NULL)
	{
		free(list[i]);
		i += 1;
	}
	free(list);
}

/* ODOT: maybe this shouldn't exec the .crashrc again? */

static int
	sh_exec(char *name, char **argv)
{
	int		ret;
	size_t	i;
	char	**argv2;
	char	**envp;

	envp = sh_env();
	sh_fd_before_exec();
	ret = execve(name, argv, envp);
	if (ret == -1 && errno == ENOEXEC)
	{
		i = 0;
		while (argv[i] != NULL)
			i += 1;
		argv2 = sh_safe_malloc(sizeof(*argv2) * (i + 2));
		argv2[i + 1] = NULL;
		while (i > 0)
		{
			argv2[i] = ft_strdup(argv[i - 1]);
			i -= 1;
		}
		argv2[0] = ft_strdup(sh()->self);
		ret = execve(sh()->self, argv2, envp);
		sh_free_list(argv2);
	}
	return (sh_free_list(envp), ret);
}

/* ODOT: check if splitting by : is correct */
int
	sh_execvp(char **argv)
{
	size_t	i;
	char	**path;
	char	*strings[2];
	int		ret;

	strings[0] = sh_getenv("PATH", NULL);
	if (ft_strchr(argv[0], '/') != NULL || strings[0] == NULL
		|| argv[0][0] == '\0')
		return (sh_exec(argv[0], argv));
	path = ft_split(strings[0], ':');
	i = 0;
	while (path[i] != NULL)
	{
		strings[1] = sh_join2(path[i], '/', argv[0]);
		if (access(strings[1], X_OK) == 0)
		{
			ret = sh_exec(strings[1], argv);
			free(strings[1]);
			sh_free_list(path);
			return (ret);
		}
		free(strings[1]);
		i += 1;
	}
	return (sh_free_list(path), -1);
}
