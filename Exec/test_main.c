/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labintei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 18:48:32 by labintei          #+#    #+#             */
/*   Updated: 2021/09/20 19:08:15 by labintei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

// Grosso modo il va falloir pipe_fd
// Faire un fd= dup(1) pour recuperer stdout
// Faire la redirection output
// Puis apres dup2(1, fd);

int		main(void)
{
	int		pipe_fd[2];


	char	s[] = "Ne s affichera pas dans le pipe";
	pipe(pipe_fd);

	int		n;

	n = dup(1);
	printf("\n1\n");
	dup2(pipe_fd[1], 1);
	printf("\n2 Gananaannufwfhuhirurhiurehiuerghuhui\n");

	int		m;

	char	s2[strlen(s)];

	m = read(pipe_fd[0], s2, sizeof(s2) -1);
	s2[m] = '\0';

	int		fd;

	fd = open("Temp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	write(fd, s2, strlen(s2));

	fflush(stdout);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	close(fd);
	//close(n);

	return 0;
}
