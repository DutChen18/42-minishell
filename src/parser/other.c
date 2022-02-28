/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   other.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/28 10:22:45 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/02/28 10:53:49 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int
	pr_brace_group(t_parser *pr, t_snode *parent)
{
	t_snode	*node;

	node = snode(sx_brace_group);
	if (pr_token(pr, NULL, sx_none, kw_lbrace))
	{
		if (pr_compound_list(pr, node))
		{
			if (pr_token(pr, NULL, sx_none, kw_rbrace))
			{
				node_add_child(parent, node);
				return (1);
			}
		}
	}
	node_destroy(node);
	return (0);
}

int
	pr_wordlist(t_parser *pr, t_snode *parent)
{
	t_snode	*node;

	node = snode(sx_wordlist);
	while (pr_token(pr, node, sx_word, tk_word))
		continue ;
	if (node->childs_size != 0)
	{
		node_add_child(parent, node);
		return (1);
	}
	node_destroy(node);
	return (0);
}

int
	pr_term(t_parser *pr, t_snode *parent)
{
	t_snode	*node;

	node = snode(sx_term);
	if (pr_and_or(pr, node))
	{
		if (pr_token(pr, NULL, sx_and, op_and))
			node->flags |= flag_and;
		else if (pr_token(pr, NULL, sx_semicolon, op_semi))
			node->flags |= flag_semi;
		while (pr_token(pr, NULL, sx_newline, tk_newline))
			node->flags |= flag_newline;
		if (node->flags)
			pr_term(pr, node);
		node_add_child(parent, node);
		return (1);
	}
	node_destroy(node);
	return (0);
}

int
	pr_pattern(t_parser *pr, t_snode *parent)
{
	if (!pr->current_ret)
		return (0);
	if (pr_convert_keyword(pr, pr->current, kw_esac))
		return (0);
	pr_token(pr, parent, sx_pattern, tk_word);
	return (1);
}

int
	pr_bang(t_parser *pr, t_snode *parent)
{
	t_snode	*node;

	node = snode(sx_bang);
	if (pr_token(pr, NULL, sx_none, kw_bang))
	{
		if (pr_pipe_sequence(pr, node))
		{
			node_add_child(parent, node);
			return (1);
		}
	}
	node_destroy(node);
	return (0);
}