prefix?=/usr/local

ifdef WHEEL_GROUP
CFLAGS+=-DWHEEL_GROUP=\"$(WHEEL_GROUP)\"
endif

all: rly

install: all
	install -c -m 2755 -o root -g root -d $(DESTDIR)$(prefix)/sbin
	install -c -m 4750 -o root -g wheel rly $(DESTDIR)$(prefix)/sbin
