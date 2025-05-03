TARGET      = dwmstatus
CC          = clang
CFLAGS      = -I. -I/usr/X11R6/include -ansi -D\_GNU\_SOURCE -g3 -O0
LDFLAGS     = -I. -L/usr/X11R6/lib -lX11

SRCS        = main.c dwmstatus.c util.c test.c
OBJS        = ${SRCS:.c=.o}

all: clean ${TARGET} run

${TARGET}: ${OBJS}
	@${CC} ${OBJS} ${LDFLAGS} -o ${TARGET}

.c.o:
	@${CC} ${CFLAGS} -c $< -o $@

clean:
	@rm -f ${OBJS} ${TARGET}

run: ${TARGET}
	@${TARGET}

commit:
	@git add .
	@git commit -m "AUTO COMMIT: `date +'%Y-%m-%d %H:%M:%S'`"
	@git push origin main

debug:
	@echo "SRCS: ${SRCS}"
	@echo "OBJS: ${OBJS}"

install: ${TARGET}
	@rm /usr/local/bin/${TARGET}
	@cp ${TARGET} /usr/local/bin/${TARGET}

.PHONY: all clean run commit debug install

