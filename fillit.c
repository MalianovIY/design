/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fillit.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahorker <ahorker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/14 03:19:36 by ahorker           #+#    #+#             */
/*   Updated: 2019/02/14 05:40:58 by ahorker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fillit.h"

char	put_tetra_help(t_tet *m, t_tet **t, int i)
{
	int	k[6];

	k[5] = 0;
	k[1] = i / m->x;
	k[2] = i % m->x;
	k[0] = (*t)->k - 1;
	while (++k[0] < 13 && k[5] < 4)
	{
		k[3] = k[0] / 4;
		k[4] = k[0] % 4;
		if ((*t)->t[k[3]][k[4]] != '.')
		{
			m->t[k[1] + k[3]][k[2] + k[4] - (*t)->k] = (*t)->t[0][(*t)->k];
			k[5]++;
		}
	}
	return (1);
}

int	put_tetra(t_tet *m, t_tet **t)
{
	int	i, k, l;

	i = -1;
	while (++i < m->k)
	{
		while(m->t[i / m->x][i % m->x] != '.' && m->t[i / m->x][i % m->x])
			i++;
		if (m->t[i / m->x][i % m->x] == '.')
		{
			if ((*t)->y + i / m->x > m->x
					|| (*t)->x + i % m->x - (*t)->k > m->x
					|| i % m->x - (*t)->k < 0)
				continue ;
			k = (*t)->k - 1;
			l = 0;
			while (++k < 13 && l < 4)
				if ((*t)->t[k / 4][k % 4] != '.' && m->t[i / m->x + k / 4]
							[i % m->x + k % 4 - (*t)->k] != '.')
						break ;
				else
					l++;
			if (l == 4)
				return ((int)((*t)->p = put_tetra_help(m, t, i)));
		}
	}
	return (0);
}

void	remove_tetra(t_tet *m, t_tet **t)
{
	int i, k, p;

	(*t)->p = 0;
	p = (*t)->t[0][(*t)->k];
	i = -1;
	while (++i < m->k)
		if (m->t[i / m->x][i % m->x] == p)
			break ;
	k = (*t)->k - 1;
	while (++k < 16)
		if ((*t)->t[k / 4][k % 4] - '.')
			m->t[i / m->x + k / 4][i % m->x + k % 4 - (*t)->k] = 46;
}

int		backtracking(t_tet *m, t_tet **t, int se)
{
	int		p, i;
	t_tet	*t1;

	p = -1;
	t1 = *t;
	while (++p < se)
	{
		if (t1->p == 0)
		{
			if (put_tetra(m, &t1) == 1)
			{
				if (backtracking(m, t, se) == 0)
					remove_tetra(m, &t1);
			}
			else
				return (0);
		}
		t1 = t1->next;
	}
	i = 0;
	while (m->t[i])
		ft_putendl(m->t[i++]); //write normal function of output
	return (1);
}

int	fill_it(t_tet **t, int n)
{
	t_tet	m;
	int		i[2];

	m.x = n > 3 ? n : n + 1;
	m.k = m.x * m.x;
	i[0] = -1;
	while (++i[0] < 53)
	{
		m.t[i[0]][53] = 0;
		m.t[53][i[0]] = 0;
		i[1] = -1;
		while (++i[1] < 53)
			m.t[i[0]][i[1]] = '.';
	}
	while (backtracking(&m, t, n) == 0)
	{
		m.x++;
		m.k = m.x * m.x;
	}
	return (1);
}
