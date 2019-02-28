/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahorker <ahorker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/14 03:19:36 by ahorker           #+#    #+#             */
/*   Updated: 2019/02/14 05:40:58 by ahorker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fillit.h"

char	**int2tetra(int a, char c) //65 .46
{
	char 	**tet;
	int		i;

	i = -1;
	if (a == 0)
		exit(221);
	tet = (char **)malloc(sizeof(char *) * 5);
	tet[4] = NULL;
	while ((a & 0xF000) != 0 && i < 4)
	{
		tet[++i] = (char *)malloc(sizeof(char) * 5);
		tet[i][0] = 46 + (char)((a & 0x8000) != 0) * (c - 46);
		tet[i][1] = 46 + (char)((a & 0x4000) != 0) * (c - 46);
		tet[i][2] = 46 + (char)((a & 0x2000) != 0) * (c - 46);
		tet[i][3] = 46 + (char)((a & 0x1000) != 0) * (c - 46);
		tet[i][4] = 0;
		a <<= 4;
	}
	while (++i < 4)
		tet[i] = ft_memset((char *)ft_memalloc(sizeof(char) * 5), 46, 4);
	return (tet);
}

int		tetread(int fd, char buf[], int *t)
{
	int	ret, wl, i;

	ret = (int)read(fd, buf, 21);
	if ((ret < 20 && ret > 0) || ret < 0)
		exit(1);
	buf[ret] = 0;
	i = -1;
	t[0] = 0;
	wl = 5;
	while (buf[++i])
		if (((buf[i] == '.' || buf[i] == '#') && i % 5 != 4 && i != 20) ||
				(buf[i] == '\n' && (i % 5 == 4 || i == 20)))
			if ((*t = (buf[i] == '#') ? ((*t << 1) | 1) : *t) != 0 && buf[i] == '#')
				wl = wl < i % 5 ? wl : i % 5;
			else
				*t = (buf[i] == '.') ? (*t << 1) : *t;
		else
			exit(2);
	*t <<= wl;
	while ((*t & 0xF000) == 0 && *t)
		*t <<= 4;
	return (ret);
}

void	tetreadvalid(int fd, int const tet[], int inp[])
{
	char	buf[22];
	int		i, j, t, ret, p;

	j = 0;
	ret = 22;
	p = 1;
	while (ret > 19)
	{
		ret = tetread(fd, buf, &t);
		p = ret == 20 ? 0 : p;
		i = -1;
		while (tet[++i] && t)
			if (t == tet[i])
				break ;
		if (i > 18)
			exit(4);
		else
			inp[j++] = t;
		if (ret < 21)
			break ;
	}
	if (p == 1)
		exit(5);
}

int		main(int argc, char *argv[])
{
	int				fd, i, j, size;
	unsigned int	tet[20];
	int				inp[27];
	char 			***tetra;

	ft_memcpy((void *)tet,(void *)STRING_TETRA_1, 20);
	ft_memcpy((void *)(tet + 20),(void *)STRING_TETRA_2, 20);
	ft_bzero(inp, 27);
	if (argc != 2)
	{
		write(1, "usage: ./fillit source_file\n", 29);
		return (0);
	}
	fd = open(argv[1],O_RDONLY);
	tetreadvalid(fd, tet, inp);
	size = 0;
	while (inp[size] && size < 27)
		size++;
	tetra = (char ***)malloc(sizeof(char **) * (size + 1));
	tetra[size] = NULL;
	j = -1;
	while (++j < size)
	{
		tetra[j] = int2tetra(inp[j], 'A' + j);
		i = -1;
		while (++i < 4)
			printf("%0x, %s\n", inp[0], tetra[j][i]);
		printf("\n");
	}
	fillit(tetra);
	exit(0);
}
