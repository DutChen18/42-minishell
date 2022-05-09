#include "minishell.h"
#include "commander.h"
#include "parser.h"
#include "memory.h"
#include <unistd.h>
#include <libgen.h>
#include <stdlib.h>

/* technically these are not all builtin utilities, but the minishell */
/* subject requires that it is implemented within the shell */
static const t_builtin
	*get_utilities(void)
{
	static const t_builtin utilities[] = {
		{ "getopts", sh_getopts },
		{ "echo", sh_echo },
		{ "cd", sh_cd },
		{ "pwd", sh_pwd },
		{ "env", sh_env_builtin },
		{ "alias", sh_unimplemented },
		{ "false", sh_false },
		{ "jobs", sh_unimplemented, },
		{ "false", sh_true },
		{ "bg", sh_unimplemented, },
		{ "fc", sh_unimplemented, },
		{ "kill", sh_unimplemented, },
		{ "umask", sh_unimplemented, },
		{ "cd", sh_unimplemented, },
		{ "fg", sh_unimplemented, },
		{ "newgrp", sh_unimplemented, },
		{ "unalias", sh_unimplemented, },
		{ "command", sh_unimplemented, },
		{ "getopts", sh_unimplemented, },
		{ "read", sh_unimplemented, },
		{ "wait", sh_unimplemented, },
	};

	return (utilities);
}

static size_t
	get_utilities_count(void)
{
	return (21);
}

static const t_builtin
	*get_builtins(void)
{
	const static t_builtin	builtins[] = {
		{ "exit", sh_exit },
		{ ".", sh_dot },
		{ ":", sh_colon },
		{ "set", sh_set },
		{ "break", sh_break },
		{ "export", sh_export },
		{ "continue", sh_continue },
		{ "shift", sh_shift },
		{ "unset", sh_unset_builtin }
	};

	return (builtins);
}

static size_t
	get_builtins_count(void)
{
	return (9);
}

void
	sh_init(char **argv, char **env)
{
	char	*tmp;

	sh_env_init(env);
	sh()->pwd = sh_getcwd();
	sh()->self = sh_join_path(sh()->pwd, argv[0]);
	sh()->builtins = get_builtins();
	sh()->builtins_size = get_builtins_count();
	sh()->functions = NULL;
	sh()->functions_size = 0;
	sh()->utilities = get_utilities();
	sh()->utilities_size = get_utilities_count();
	sh()->args = sh_strlst_dup(argv);
	sh()->interactive = sh_has_term(STDIN_FILENO);
	sh()->io[SH_STDIN_INDEX] = STDIN_FILENO;
	sh()->io[SH_STDOUT_INDEX] = STDOUT_FILENO;
	sh()->io[SH_STDERR_INDEX] = STDERR_FILENO;
	sh()->last_bg_proc = -1;
	sh()->is_subshell = 0;
	tmp = ft_strdup(argv[0]);
	sh()->name = sh_basename(tmp);
	free(tmp);
	sh_setenv("OPTIND", "1", 0);
	sh_setenv("PS1", "$ ", 0);
	sh_setenv("PS2", "> ", 0);
	sh_setenv("PS4", "+ ", 0);
}

void
	sh_destroy(void)
{
	size_t	i;

	sh_env_destroy();
	i = 0;
	while (i < sh()->functions_size)
	{
		free(sh()->functions[i].key);
		node_destroy(sh()->functions[i].body);
		i++;
	}
	free(sh()->self);
	free(sh()->functions);
	free(sh()->name);
	free(sh()->pwd);
	sh_strlst_clear(sh()->args);
}

static t_function
	*sh_get_function(const char *key)
{
	size_t	index;

	index = 0;
	while (index < sh()->functions_size)
	{
		if (!ft_strcmp(sh()->functions[index].key, key))
			return (&sh()->functions[index]);
		index++;
	}
	return (NULL);
}

void
	sh_add_function(const char *key, t_snode *body)
{
	t_function	*entry;

	entry = sh_get_function(key);
	if (entry == NULL)
	{
		sh()->functions = sh_safe_realloc(sh()->functions,
			sizeof(*sh()->functions) * sh()->functions_size,
			sizeof(*sh()->functions) * (sh()->functions_size + 1));
		entry = &sh()->functions[sh()->functions_size];
		entry->key = NULL;
		entry->body = NULL;
		sh()->functions_size += 1;
	}
	/* TODO free previous entry */
	entry->key = ft_strdup(key);
	entry->body = sh_safe_malloc(sizeof(*body));
	node_move(entry->body, body);
}
