/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   shft_isxdigit.c                                  -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/05/10 11:58:49 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/05/10 11:58:49 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

int
	ft_isxdigit(int ch)
{
	return ((ch >= '0' && ch <= '9')
		|| (ch >= 'A' && ch <= 'F')
		|| (ch >= 'a' && ch <= 'f'));
}
