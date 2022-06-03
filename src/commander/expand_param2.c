/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   expand_param2.c                                  -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/05/10 11:59:43 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/05/10 11:59:43 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "commander.h"

#include <stdlib.h>

int
	expand_assign(t_expand *exp, t_param_ctx *ctx, size_t i, int mode)
{
	char	*str;

	if (ctx->key[0] == '*'
		|| ctx->key[0] == '@'
		|| ctx->key[0] == '#'
		|| ctx->key[0] == '?'
		|| ctx->key[0] == '-'
		|| ctx->key[0] == '$'
		|| ctx->key[0] == '!'
		|| ft_isdigit(ctx->key[0]))
	{
		sh_err2(ctx->key, "cannot assign in this way");
		return (-1);
	}
	ctx->token->parts[0].data = (char *) ctx->token->parts[0].data + i;
	str = cm_expand_str(ctx->token, NULL, ' ', mode);
	ctx->token->parts[0].data = (char *) ctx->token->parts[0].data - i;
	if (str == NULL)
		return (-1);
	sh_setenv(ctx->key, str, 0);
	expansion_add_part(exp, sh_strlst_new(str), 0);
	return (0);
}

int
	expand_right(t_expand *exp, t_param_ctx *ctx, size_t i, int mode)
{
	t_expand	tmp;
	int			result;

	ctx->token->parts[0].data = (char *) ctx->token->parts[0].data + i;
	result = cm_expand_list(&tmp, ctx->token, mode);
	ctx->token->parts[0].data = (char *) ctx->token->parts[0].data - i;
	if (result < 0)
		return (-1);
	expansion_copy_parts(exp, &tmp);
	expansion_destroy(&tmp);
	return (0);
}

int
	expand_promote(t_expand *exp, t_expand *tmp)
{
	if (tmp->count == 0)
		expansion_add_part(exp, sh_strlst_new(ft_strdup("")), 0);
	else
		expansion_copy_parts(exp, tmp);
	expansion_destroy(tmp);
	return (0);
}

void
	expand_pattern_percent(t_exp_param_ctx *ctxx)
{
	ctxx->best = ft_strlen(ctxx->str);
	while (ctxx->str[ctxx->j] != '\0')
	{
		ctxx->match = pattern_match(ctxx->str + ctxx->j,
				ctxx->comp_pattern, 0);
		if (ctxx->match)
		{
			ctxx->best = ctxx->j;
			if (ctxx->long_mode)
				break ;
		}
		ctxx->j += 1;
	}
	ctxx->str[ctxx->best] = '\0';
}

void
	expand_pattern_hash(t_exp_param_ctx *ctxx, t_expand *exp)
{
	ctxx->best = 0;
	while (ctxx->str[ctxx->j] != '\0')
	{
		ctxx->j += 1;
		ctxx->ch = ctxx->str[ctxx->j];
		ctxx->str[ctxx->j] = '\0';
		ctxx->match = pattern_match(ctxx->str, ctxx->comp_pattern, 0);
		ctxx->str[ctxx->j] = ctxx->ch;
		if (ctxx->match)
		{
			ctxx->best = ctxx->j;
			if (!ctxx->long_mode)
				break ;
		}
	}
	exp->parts[exp->count - 1].str[ctxx->i]
		= ft_strdup(ctxx->str + ctxx->best);
	free(ctxx->str);
}
