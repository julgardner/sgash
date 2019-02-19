.SUFFIXES : .c

INCDIR =
LIBDIR =

CC = gcc

CFLAGS = -g

LIBS =

OBJS = sgash.o

TARGET = sgash

$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LIBDIR) $(LIBS)

.c.o:
	$(CC) -c $(CFLAGS) $(INCDIR) $<

.PHONY: clean

clean:
	rm -f $(OBJS) $(TARGET) core
