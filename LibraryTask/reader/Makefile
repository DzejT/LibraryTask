CFLAGS:=-Wall -Wextra -I../lib
LDFLAGS:= -L../lib -llogger
SRCS:=$(wildcard *.c)
OBJS:=$(SRCS:.c=.o)
TARGET_EXEC:=reader


.PHONY: all clean

all: $(TARGET_EXEC)

$(TARGET_EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) 

$(OBJS): $(SRCS)
	$(CC) $(CFLAGS) -o $@ -c $^
	
clean:
	rm -f $(TARGET_EXEC) $(OBJS)