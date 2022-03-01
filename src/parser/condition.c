/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   condition.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/28 10:12:45 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/03/01 13:17:38 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int
	pr_if_clause(t_parser *pr, t_snode *parent)
{
	return (pr_elif_part(pr, parent, kw_if));
}

int
	pr_elif_part(t_parser *pr, t_snode *parent, t_token_id id)
{
	t_snode	*node;

	node = snode(sx_if_clause);
	if (pr_token(pr, NULL, sx_none, id)
		&& pr_compound_list(pr, node)
		&& pr_token(pr, NULL, sx_none, kw_then)
		&& pr_compound_list(pr, node))
	{
		if (!pr_elif_part(pr, node, kw_elif)
			&& !pr_else_part(pr, node))
			node_add_child(node, snode(sx_compound_list));
		node_add_child(parent, node);
		return (1);
	}
	node_destroy(node);
	return (0);
}

int
	pr_else_part(t_parser *pr, t_snode *parent)
{
	if (pr_token(pr, NULL, sx_none, kw_else)
		&& pr_compound_list(pr, parent))
		return (1);
	return (0);
}
