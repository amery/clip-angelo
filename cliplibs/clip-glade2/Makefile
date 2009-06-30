# Makefile for clip-libglade2

ifndef CLIPROOT
	CLIPROOT=/usr/local/clip
endif

include $(CLIPROOT)/include/Makefile.inc

# Check libglade version.
LIBGLADE_MIN=2.4.2


XTARGET=$(XLIB) $(XSLIB)

CFLAGS = $(C_FLAGS)  -I../clip-gtk2

OBJS=clip_glade.o


XLIB=libclip-glade2.a
XSLIB=libclip-glade2$(DLLSUFF)


include Makefile.01

.SUFFIXES: .c .o


all: have_libglade  $(XTARGET)


Makefile.01 : configure
	./configure

have_libglade:
	@ test -n "$(LIBGLADE_VER)" || echo libglade2 not found
	@ test -n "$(LIBGLADE_VER)" || exit
	@ test $(LIBGLADE_VER) '>' $(LIBGLADE_MIN) -o $(LIBGLADE_VER) = $(LIBGLADE_MIN)  || echo libglade detected $(LIBGLADE_VER), needed $(LIBGLADE_MIN) or upper && exit
	@ test $(LIBGLADE_VER) '>' $(LIBGLADE_MIN) -o $(LIBGLADE_VER) = $(LIBGLADE_MIN)  && exit



$(XLIB): $(OBJS)
	rm -f $(XLIB)
	clip_makelib$(SCRIPTSUFF) $(XLIB) $(OBJS)
	ranlib $(XLIB)

$(XSLIB): $(XLIB)
	rm -f $(XSLIB)
	clip_makeslib$(SCRIPTSUFF) $(XSLIB) $(XLIB) $(LIBS)


install:all
	clip_cp$(SCRIPTSUFF) $(XLIB) $(SXLIB) $(DESTDIR)$(CLIPROOT)/lib
	chmod 0644 $(DESTDIR)$(CLIPROOT)/lib/$(XLIB)
	chmod 0755 $(DESTDIR)$(CLIPROOT)/lib/$(XSLIB)

%.o: %.c Makefile Makefile.01
	echo $(CFLAGS)
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o *~ *.so *.a *.nm *.ex Makefile.01

