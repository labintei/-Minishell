# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: labintei <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/10/04 12:57:29 by labintei          #+#    #+#              #
#    Updated: 2021/10/04 12:57:32 by labintei         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS		=	./Exec/exec_new.c ./Exec/path.c ./Exec/redirection.c \
				./Exec/exec_builtin.c ./Exec/signals.c \
				./Exec/exec_one_cmd.c ./Exec/exec_new_bis.c \
				./Exec/exec_one_cmd_bis.c ./Exec/path1.c \
				./Exec/redirection_bis.c ./Exec/exec_new_1.c \
				./Env/convert_env.c ./Env/env.c ./Env/env1.c ./Env/expansion.c\
				./Env/expansion1.c\
				./Utils/utils.c ./Utils/utils1.c ./Utils/utils2.c \
				./Utils/fancy.c ./Utils/utils3.c ./Utils/clear.c \
				./Utils/view.c\
				./Parsing/count_word.c ./Parsing/count_char.c \
				./Parsing/main.c ./Parsing/cmds.c ./Parsing/parse.c \
				./Parsing/main1.c ./Parsing/main2.c ./Parsing/main3.c \
				./Parsing/main4.c \
				./Built/cd.c ./Built/echo.c ./Built/exit.c ./Built/export.c \
				./Built/pwd.c ./Built/unset.c ./Built/export1.c \
				./Built/export2.c ./Built/export3.c \
				./Error/error.c

OBJS		= $(SRCS:.c=.o)
CC			= gcc
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
