# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: malatini <malatini@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/08/16 20:11:09 by labintei          #+#    #+#              #
#    Updated: 2021/09/10 19:04:28 by labintei         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS		=	./Exec/exec.c ./Exec/path.c ./Exec/redirection.c \
				./Exec/exec_builtin.c ./Exec/signals.c \
				./Env/convert_env.c ./Env/env.c ./Env/env2.c ./Env/expansion.c\
				./Utils/utils.c ./Utils/utils1.c ./Utils/utils2.c \
				./Utils/fancy.c ./Utils/utils3.c ./Utils/free.c \
				./Utils/clear.c ./Utils/view.c\
				./Parsing/count_word.c ./Parsing/count_char.c \
				./Parsing/count_redir.c ./Parsing/main.c ./Parsing/cmds.c \
				./Built/cd.c ./Built/echo.c ./Built/exit.c ./Built/export.c \
				./Built/pwd.c ./Built/unset.c

OBJS		= $(SRCS:.c=.o)
CC			= gcc -Wall -Werror -Wextra -g3 -fsanitize=address
RM			= rm -rf
NAME		= minishell
CFLAGS		= -Wall -Wextra -Werror

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
