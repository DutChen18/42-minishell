#include "minishell.h"
#include "commander.h"
#include "parser.h"

#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <readline/readline.h>

extern int rl_catch_signals;
extern void (*rl_event_hook)(void);
extern void rl_clear_history(void);

t_snode
	*pr_parse(t_parser *pr)
{
	t_snode	*node;
	t_snode	*child;

	node = snode(sx_none);
	if (!pr_complete_cmd(pr, node))
	{
		node_destroy(node);
		return (NULL);
	}
	sh_assert(node->childs_size == 1);
	child = node->childs[0];
	node->childs_size = 0;
	node_destroy(node);
	return (child);
}

void
	sh_new_command(t_input *in, t_parser *pr)
{
	history_new_command();
	sh()->restart = 0;
	sh()->continuing = 0;
	in->more = 0;
	pr->lexer->error = 0;
}

int
	sh_cm_run(t_input *in)
{
	t_source	src;
	t_lexer		lex;
	t_parser	pr;
	t_snode		*node;
	int			std_io[3];

	rl_catch_signals = 1;
	rl_event_hook = sh_nop1;
	src_init(&src, in);
	lex_init(&lex);
	pr_init(&pr);
	lex.src = &src;
	pr.lexer = &lex;
	std_io[SH_STDIN_INDEX] = STDIN_FILENO;
	std_io[SH_STDOUT_INDEX] = STDOUT_FILENO;
	std_io[SH_STDERR_INDEX] = STDERR_FILENO;
	sh()->last_command = NULL;
	rl_clear_history();
	while (1)
	{
		sh_new_command(in, &pr);
		if (pr.current.id != tk_invalid)
			token_destroy(&pr.current);
		pr_next_token(&pr);
		if (pr.current.id == tk_invalid || pr.current.id == tk_null)
			break ;
		node = pr_parse(&pr);
		if (sh()->restart)
		{
			node_destroy(node);
			continue;
		}
		if (node != NULL)
			commandeer(node, std_io);
		node_destroy(node);
		if (pr.lexer->error)
			sh_err1("syntax error");
		add_history(history_get_last_command());
		history_new_command();
	}
	pr_destroy(&pr);
	// TODO: errors?
	return (0);
}
