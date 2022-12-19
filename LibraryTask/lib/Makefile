CFLAGS:=-Wall -fPIC -shared -I`pwd`
SRCS:= $(wildcard *.c)
OBJS:=$(SRCS:.c=.o)
LIB:=liblogger.so
PREFIX:=/usr/local

.PHONY: all clean install uninstall

all: $(LIB)

$(LIB): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ -L`pwd` -lsqlite3

install:$(LIB) logger.h
	install -d $(PREFIX)/lib $(PREFIX)/include
	install -m 644 logger.h $(PREFIX)/include
	install -m 644 $(LIB) $(PREFIX)/lib

uninstall:
	rm -f $(PREFIX)/include/logger.h
	rm -f $(PREFIX)/lib/$(LIB)

clean:
	rm -f $(LIB) logger.o