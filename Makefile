<<<<<<< HEAD
TARGET		= dwmstatus
CC		= gcc
CFLAGS		= -I. -I/usr/X11R6/include -g3 -O0 -std=c99 -Wall -Werror -Wextra -pedantic
LDFLAGS		= -L/usr/X11R6/lib
LIBS		= -lX11
SRCS		= main.c dwmstatus.c util.c

${TARGET}: ${SRCS}
	${CC} ${CFLAGS} ${LDFLAGS} ${SRCS} ${LIBS} -o $@

run: ${TARGET}
<<<<<<< HEAD
	./${TARGET}
=======
	@echo "run"
	${TARGET}

commit:
	@echo "commit"
	git add .
	git commit -m "AUTO COMMIT: `date +'%Y-%m-%d %H:%M:%S'`"
	git push origin linux-port
>>>>>>> ca0d8a8 (AUTO COMMIT: 2025-06-03 16:17:54)

install: ${TARGET}
	cp ${TARGET} /usr/local/bin/${TARGET}
=======
TARGET      	= dwmstatus
CC          	= gcc
CFLAGS      	= -g3 -O0 
LDFLAGS     	= 
LIBS		= -lX11
SRCS        = main.c dwmstatus.c util.c

${TARGET}: ${SRCS}
	${CC} ${CFLAGS} ${LDFLAGS} ${SRCS} ${LIBS} -o ${TARGET}

run: ${TARGET}
	./${TARGET}

install: ${TARGET}
	cp ${TARGET} /usr/local/bin/${TARGET}

>>>>>>> linux-port
