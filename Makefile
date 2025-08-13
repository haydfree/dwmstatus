TARGET		= dwmstatus
CC		= gcc
CFLAGS		= -I. -I/usr/X11R6/include -g3 -O0 -std=c99 -Wall -Werror -Wextra -pedantic
LDFLAGS		= -L/usr/X11R6/lib
LIBS		= -lX11
SRCS		= main.c dwmstatus.c util.c

${TARGET}: ${SRCS}
	${CC} ${CFLAGS} ${LDFLAGS} ${SRCS} ${LIBS} -o $@

run: ${TARGET}
	./${TARGET}

install: ${TARGET}
	cp ${TARGET} /usr/local/bin/${TARGET}
