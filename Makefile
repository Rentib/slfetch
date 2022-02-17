# See LICENSE file for copyright and license details.
# slfetch - simple linux fetch / suckless fetch
.POSIX:

include config.mk

REQ = util
all: slfetch

$(COM:=.o): config.mk $(REQ:=.h)
slfetch.o: slfetch.c slfetch.h config.h config.mk $(REQ:=.h)

.c.o:
	$(CC) -o $@ -c $(CFLAGS) $<

config.h:
	cp config.def.h $@

slfetch: slfetch.o $(REQ:=.o)
	$(CC) -o $@ $(REQ:=.o) slfetch.o

clean:
	rm -f slfetch slfetch.o $(REQ:=.o)

install: all
	mkdir -p "$(DESTDIR)$(PREFIX)/bin"
	cp -f slfetch "$(DESTDIR)$(PREFIX)/bin"
	chmod 755 "$(DESTDIR)$(PREFIX)/bin/slfetch"

uninstall:
	rm -f "$(DESTDIR)$(PREFIX)/bin/slfetch"
