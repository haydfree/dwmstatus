TARGET      	= dwmstatus
CC          	= gcc
CFLAGS      	= -g3 -O0 
LDFLAGS     	= 
LIBS			= -L/usr/X11R6/lib -lX11
INCS			= -I/usr/X11R6/include
SRCS        	= dwmstatus.c

${TARGET}: ${SRCS}
	${CC} ${CFLAGS} ${LDFLAGS} ${INCS} ${SRCS} ${LIBS} -o ${TARGET}

run: ${TARGET}
	./${TARGET}

install: ${TARGET}
	cp ${TARGET} /usr/local/bin/${TARGET}

