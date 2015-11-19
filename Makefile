# dwmsd - dynamic window manager status daemon

CFLAGS += -Wall -pedantic -I/opt/X11/include -L/opt/X11/lib -O3
LDFLAGS += -lX11

SRC = dwm0d.c dwm0c.c
OBJ = ${SRC:.c=.o}

PREFIXDIR=/usr

all: dwm0d dwm0c

.c.o:
	@echo CC -c $<
	@${CC} -c ${CFLAGS} $<

dwm0d: ${OBJ}
	@echo CC -o $@
	@${CC} -o $@ dwm0d.o ${LDFLAGS}

dwm0c: ${OBJ}
	@echo CC -o $@
	@${CC} -o $@ dwm0c.o ${LDFLAGS}

clean:
	@rm -f dwm0d dwm0c *.o

install: all
	@install -d -m 0755	${DESTDIR}${PREFIXDIR}/bin
	@install -m 0755 dwm0d	${DESTDIR}${PREFIXDIR}/bin/
	@install -m 0755 dwm0c	${DESTDIR}${PREFIXDIR}/bin/

.PHONY: all clean install
