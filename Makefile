WHEEL_GROUP?=wheel
CFLAGS+=-DWHEEL_GROUP=\"$(WHEEL_GROUP)\"
prefix?=/usr/local

all: rly

install: all
	install -c -m 2755 -o root -g root -d $(DESTDIR)$(prefix)/sbin
	install -c -m 4750 -o root -g $(WHEEL_GROUP) rly $(DESTDIR)$(prefix)/sbin
