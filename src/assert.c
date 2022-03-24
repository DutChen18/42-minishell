/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   assert.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/08 09:20:23 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/03/24 14:40:02 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "ft_printf.h"
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void
	sh_assert(int test)
{
	if (!test)
	{
		ft_fprintf(STDERR_FILENO, "Assertion failed!\n");
		exit(EXIT_FAILURE);
	}
}
