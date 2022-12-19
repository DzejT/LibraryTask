CFLAGS:=-Wall -Wextra -I../lib
LDFLAGS:= -L../lib -llogger
SRCS:=$(wildcard *.c)
OBJS:=$(SRCS:.c=.o)
TARGET_EXEC:=Program1

.PHONY: all clean

all: $(TARGET_EXEC)

$(TARGET_EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJS): $(SRCS)
	$(CC) $(CFLAGS) -c -o $@ $^
	
clean:
	rm -f $(TARGET_EXEC) $(OBJS)