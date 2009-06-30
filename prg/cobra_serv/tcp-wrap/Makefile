LIBS = -lwrap
CC = gcc
CFLAGS = -O

all: tcp-wrap$(EXESUFF)

tcp-wrap$(EXESUFF): tcp-wrap.c
	$(CC) $(CFLAGS) -o $@ tcp-wrap.c $(LIBS)

clean:
	rm -f$(V) tcp-wrap$(EXESUFF) *.bak *.o

