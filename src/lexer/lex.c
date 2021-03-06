/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   lex.c                                            -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/05/10 12:00:20 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/05/10 12:00:20 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

static int
	is_blank(int c)
{
	return (c == ' ' || c == '\t');
}

static int
	check_op(t_lexer *lex)
{
	if (lex->depth == 0 && lex->src->cur == ')' && lex->id == lx_command)
	{
		lex->tok->id = op_rparen;
		return (0);
	}
	lex->tok->id = lex_op(lex);
	if (lex->tok->id == op_lparen)
		lex->depth += 1;
	if (lex->tok->id == op_rparen)
		lex->depth -= 1;
	return (1);
}

void
	lex_skip(t_lexer *lex)
{
	if (lex->src->cur == -1)
		lex_advance(lex);
	while (is_blank(lex->src->cur) && !lex_quoted(lex))
		lex_advance(lex);
	if (lex->src->cur == '#' && !lex_quoted(lex))
		while (lex->src->cur != '\n' && lex->src->cur != -1)
			lex_advance(lex);
}

int
	lex_lex_int(t_lexer *lex, t_token *tok)
{
	int	status;

	status = check_op(lex);
	if (tok->id != tk_null)
		return (status);
	tok->id = tk_word;
	status = lex_main(lex);
	if (lex->src->cur == -1)
		lex->has_eof = 1;
	return (status);
}

int
	lex_lex(t_lexer *lex, t_token *tok)
{
	if (lex->has_eof)
	{
		lex->has_eof = 0;
		return (0);
	}
	lex->new_part = 1;
	lex_skip(lex);
	lex->tok = tok;
	if (lex->src->cur == '\n')
	{
		tok->id = tk_newline;
		lex_nom_and_skip(lex);
		return (1);
	}
	if (lex->src->cur == -1)
	{
		if (lex->id == lx_command)
			return (-1);
		return (0);
	}
	return (lex_lex_int(lex, tok));
}
