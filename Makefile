TARGET      = dwmstatus
CC          = clang
CFLAGS      = -I. -ansi -g3 -O0 -D_POSIX_C_SOURCE=200809L
LDFLAGS     = -lX11

SRCS        = main.c dwmstatus.c util.c test.c
OBJS        = ${SRCS:.c=.o}

all: clean ${TARGET}

${TARGET}: ${OBJS}
	@echo "target: objs"
	${CC} ${OBJS} ${LDFLAGS} -o ${TARGET}

.c.o:
	@echo ".c.o"
	${CC} ${CFLAGS} -c $< -o $@

clean:
	@echo "clean"
	rm -f ${OBJS} ${TARGET}

run: ${TARGET}
	@echo "run"
	./${TARGET}

commit:
	@echo "commit"
	git add .
	git commit -m "AUTO COMMIT: `date +'%Y-%m-%d %H:%M:%S'`"
	git push origin linux-port

install: ${TARGET}
	@echo "install"
	rm /usr/local/bin/${TARGET}
	cp ${TARGET} /usr/local/bin/${TARGET}

.PHONY: all clean run commit debug install

