/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   ft_lstsize_bonus.c                               -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/05/10 11:58:27 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/05/10 11:58:27 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int
	ft_lstsize(t_list *lst)
{
	if (lst == NULL)
		return (0);
	return (ft_lstsize(lst->next) + 1);
}
