ifndef CLIPROOT
	CLIPROOT=$(shell cd ../../../; pwd)/cliproot
endif

include $(CLIPROOT)/include/Makefile.inc

CLIPINCLUDE=-I$(CLIPROOT)/include

#C_FLAGS=-Wall -g -I. $(CLIPINCLUDE)
#CC=gcc
TARGET=libclip-com$(DLLSUFF)
ATARGET=libclip-com.a
RTARGET=libclip-com$(DLLREALSUFF)
OBJS=com.o
CFLAGS+=-IlibezV24 -Wall -g -fPIC
LIBEZ=libezV24/libezV24-0.a

DIRS_TO_SCAN = libezV24

all:  have_lib $(TARGET) $(ATARGET)

have_lib:
	true
#	test -f /usr/include/bzlib.h

$(TARGET): $(OBJS) $(LIBEZ)
	clip_makeslib$(SCRIPTSUFF) $(TARGET) $(OBJS) $(LIBEZ)

$(ATARGET): $(OBJS) $(LIBEZ)
	clip_makelib$(SCRIPTSUFF) $(ATARGET) $(OBJS) $(LIBEZ)

$(LIBEZ):
	cd libezV24 && $(MAKE)

com.o: com.c hashcode.h
	$(CC) $(CFLAGS) $(CLIPINCLUDE) -c com.c

hashcode.h:
#	echo '#define HASH_ferror ' $$(echo ferror| $(CLIPROOT)/bin/cliphash | cut -d' ' -f2) >hashcode.h

install : all
	clip_cp$(SCRIPTSUFF) $(TARGET) $(DESTDIR)$(CLIPROOT)/lib
	#[ "$(TARGET)" = "$(RTARGET)" ] || cp $(RTARGET) $(DESTDIR)$(CLIPROOT)/lib
	mkdir -p $(DESTDIR)$(CLIPROOT)/doc/clip-com
	#-cd tdoc && $(MAKE) install
	#-cp -R doc/* $(DESTDIR)$(CLIPROOT)/doc/clip-com/
	mkdir -p $(DESTDIR)$(CLIPROOT)/doc/example/clip-com
	-cp -R example/* $(DESTDIR)$(CLIPROOT)/doc/example/clip-com/

clean:$(DIRS_TO_SCAN) example
	rm -rf *.o *.bak *.a *.so hashcode.h *.dll *.exe bzpref.h *.ex *.nm
#	-cd libezV24; $(MAKE) clean; rm -f *.a *.so *.so.* *.exe
#	-cd example; $(MAKE) clean
	#-cd tdoc && $(MAKE) clean

distclean: clean
	#-cd tdoc && $(MAKE) distclean


FORCE :

$(DIRS_TO_SCAN) example : Makefile FORCE
	$(Trace_o)
	$(E_if)if [ -d $@ ] ; then 																			\
					cd $@  																						\
					&& ( [ -f Makefile ] || ( [ -x configure ] && ./configure 0) )	\
					&& ( $(MAKE) $(MAKECMDGOALS)  		\
						||  $(MAKE) -f Makefile.in $(MAKECMDGOALS) ) ;				\
		fi
#					&& ( $(MAKE) $(MAKEFLAGS) $(ADDMAKECMDGOALS) $(MAKECMDGOALS)  		\
#						||  $(MAKE) -f Makefile.in $(MAKEFLAGS) $(ADDMAKECMDGOALS) $(MAKECMDGOALS) ) ;				\

