/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/21 15:30:57 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/02/21 15:30:57 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef INPUT_H
# define INPUT_H

# include <stddef.h>

typedef struct s_readline	t_readline;
typedef struct s_input		t_input;

struct s_readline
{
	int	warning_silencer;
};

struct s_input
{
	t_readline	rl;
	char		*line;
	size_t		index;
	int			more;
};

char	*input_readline(t_readline *rl, const char *prompt);
int		input_readchar(t_input *in);
int		input_ioend(t_input *in, const char *end);

#endif