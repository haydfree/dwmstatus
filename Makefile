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

