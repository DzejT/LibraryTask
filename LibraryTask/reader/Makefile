CFLAGS:=-Wall -Wextra
LDFLAGS:= -llogger -lsqlite3
SRCS:=$(wildcard *.c)
OBJS:=$(SRCS:.c=.o)
TARGET_EXEC:=reader

.PHONY: all clean

all: $(TARGET_EXEC)

$(TARGET_EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJS): $(SRCS)
	$(CC) $(CFLAGS) -c -o $@ $^
	
clean:
	rm -f $(TARGET_EXEC) $(OBJS)