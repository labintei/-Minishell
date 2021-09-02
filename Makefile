# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: labintei <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/08/16 20:11:09 by labintei          #+#    #+#              #
#    Updated: 2021/09/01 19:31:56 by labintei         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS	=	env.c main.c utils.c utils1.c cmds.c convert_env.c \
		count_word.c count_char.c view.c exec.c path.c clear.c \
		./built/cd.c ./built/echo.c ./built/exit.c ./built/export.c \
		./built/pwd.c ./built/unset.c

OBJS	= $(SRCS:.c=.o)

CC	= gcc -g3 -fsanitize=address
RM	= rm -rf
NAME	= minishell
CFLAGS	= -Wall -Wextra -Werror

all:		${NAME}

.c.o:		${CC} ${CFLAGS} -c $< -o ${<:.c=.o}

${NAME}:	${OBJS}
		${CC} -o $(NAME) $(SRCS) -lreadline

clean:
		$(RM) ${OBJS}

fclean:		clean
		$(RM) $(NAME)

re: 		fclean all

.PHONY:		all clean fclean re
