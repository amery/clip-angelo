#CC=gcc
CFLAGS+= -I../include.h
#-pg -DMEMDBG

SCRLIB = ./libclipscreen.a
SCROBJS = tcaps.o termgoto.o screen.o scankey.o keymap.o
# charset.o

PROGS = kbd cmap
#testscr 
install all: $(SCRLIB) $(PROGS)

KBDOBJS = scankey.o keymap.o kbd.o

kbd: $(KBDOBJS)
	$(CC) $(CFLAGS) -o kbd $(KBDOBJS)

CMAPOBJS = charset.o cmap.o

cmap:	$(CMAPOBJS)
	$(CC) $(CFLAGS) -o cmap $(CMAPOBJS)

clean:
	rm -f *.o *.bak *.b *.BAK *.a $(PROGS) $(SCRLIB)

testscr: testscr.o  Makefile $(SCRLIB) charset.o
	$(CC) $(CFLAGS) -o testscr testscr.o $(SCRLIB) charset.o -lgpm



$(SCRLIB): $(SCROBJS)
	rm -f $(SCRLIB)
	clip_makelib$(SCRIPTSUFF) $(SCRLIB) $(SCROBJS)
	ranlib $(SCRLIB)

