HDRS = \
	lselt.h \

SRCS = \
	lselt.c \
	microshell.c \

OBJS = ${SRCS:.c=.o}

NAME = microshell

CC = clang
CFLAGS = -g -DTEST_SH -Wall -Wextra #-Werror


all: ${NAME}

${NAME}: ${OBJS}
	${CC} ${OBJS} -o ${NAME} ${CFGLAGS}

${OBJS}: ${HDRS}

clean:
	rm -f *.o

fclean: clean
	rm -f ${NAME}

re: fclean all

.PHONY: clean fclean all re
