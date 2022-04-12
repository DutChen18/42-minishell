#include "commander.h"
#include "minishell.h"

#include <stdio.h>
/*
$ for name in 1 2 3 4; do
> echo $name
> done
sx_list
	sx_term
		sx_pipe_sequence
			sx_for_clause:
				tk_name: name
					lx_normal: name
				sx_wordlist
					sx_word:
						tk_word: 1
							lx_normal: 1
					sx_word:
						tk_word: 2
							lx_normal: 2
					sx_word:
						tk_word: 3
							lx_normal: 3
					sx_word:
						tk_word: 4
							lx_normal: 4
				sx_compound_list
					sx_term \n
						sx_pipe_sequence
							sx_simple_cmd
								sx_wordlist
									sx_word:
										tk_word: echo
											lx_normal: echo
									sx_word:
										tk_word: $name
											lx_normal: 
											lx_parameter:
												tk_word: name
											lx_normal: 
								sx_io_redirect_list
								sx_ass_list
				sx_io_redirect_list



> while a; do b; done
sx_list
  sx_term
    sx_pipe_sequence
      sx_while_clause
        sx_compound_list
          sx_term ;
            sx_pipe_sequence
              sx_simple_cmd
                sx_wordlist
                  sx_word:
                    tk_word: a
                      lx_normal: a
                sx_io_redirect_list
                sx_ass_list
        sx_compound_list
          sx_term ;
            sx_pipe_sequence
              sx_simple_cmd
                sx_wordlist
                  sx_word:
                    tk_word: b
                      lx_normal: b
                sx_io_redirect_list
                sx_ass_list
        sx_io_redirect_list
*/

/*
TODO: Check this:
> for name in 1 2 3 4; do
> ./fdcheck
> done < src/main.c
*/

/* TODO implement no wordlist */
/* TODO setup redirects */
pid_t
	cm_for_clause(t_snode *fornode, const int io[3], int closefd)
{
	char	*name;
	char	**list;
	char	**list_cpy;
	int		rc;
	int		for_io[3];
	t_snode	*do_node;

	sh()->loop_depth += 1;
	(void) closefd;
	rc = 0;
	name = fornode->token.str;
	list = cm_word_list_to_array(fornode->childs[0]);
	list_cpy = list;
	do_node = fornode->childs[1];
	ft_memcpy(for_io, io, sizeof(for_io));
	_cm_setup_builtin_redirects(fornode->childs[fornode->childs_size - 1], for_io);
	while (*list && !sh()->breaking)
	{
		sh()->continuing = 0;
		sh_setenv(name, *list, 0);
		rc = commandeer(do_node, for_io);
		list += 1;
	}
	sh_strlst_clear(list_cpy);
	if (sh()->breaking > 0)
	{
		sh()->breaking -= 1;
		rc = 0;
	}
	sh()->loop_depth -= 1;
	return (cm_convert_retcode(rc));
}

pid_t
	cm_while_until_clause(t_snode *node, const int io[3], int closefd)
{
	int	rc;
	int	loop_io[3];

	sh()->loop_depth += 1;
	(void) closefd;
	rc = -1;
	sh_assert(node->childs_size >= 3);
	ft_memcpy(loop_io, io, sizeof(loop_io));
	_cm_setup_builtin_redirects(node->childs[node->childs_size - 1], loop_io);
	while (!commandeer(node->childs[0], loop_io) == !(node->flags & flag_until) && !sh()->breaking)
	{
		sh()->continuing = 0;
		rc = commandeer(node->childs[1], loop_io);
	}
	if (sh()->breaking > 0)
	{
		sh()->breaking -= 1;
		rc = 0;
	}
	sh()->loop_depth -= 1;
	return (cm_convert_retcode(rc));
}
