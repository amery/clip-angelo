#  GNU make makefile
#

ifndef CLIPROOT
	CLIPROOT=$(shell cd ../../../; pwd)/cliproot
endif
include $(CLIPROOT)/include/Makefile.inc

LIBODBC=-lodbc $(C_LIBS)
# Note: below we check if LIBOOB is empty when library is missed
LIBOOB=-L/usr/local/easysoft/oob/client -lesoobclient

TARGET=libclip-odbc$(DLLSUFF)
ATARGET=libclip-odbc.a
RTARGET=libclip-odbc$(DLLREALSUFF)
TARGET1=libclip-oob$(DLLSUFF)
RTARGET1=libclip-oob$(DLLREALSUFF)
OBJS=odbc_clip.o odbc.o

all:  $(TARGET) $(TARGET1) $(ATARGET)

$(TARGET): $(OBJS)
	clip_makeslib$(SCRIPTSUFF) $(TARGET) $(OBJS) $(LIBODBC)

$(ATARGET): $(OBJS)
	clip_makelib$(SCRIPTSUFF) $(ATARGET) $(OBJS) $(LIBODBC)

$(TARGET1): $(OBJS)
	[ "$(LIBOOB)" = '' ] || clip_makeslib$(SCRIPTSUFF) $(TARGET1) $(OBJS) $(LIBOOB) $(C_LIBS)

odbc_clip.o: odbc_clip.c odbc.ch
	$(CC) $(C_FLAGS) -c odbc_clip.c

odbc.o: odbc.prg odbc.ch
	$(CLIPROOT)/bin/clip odbc.prg

install:all
	cp -fpuv $(TARGET).* $(DESTDIR)$(CLIPROOT)/lib
	cp -fpuv $(TARGET1).* $(DESTDIR)$(CLIPROOT)/lib
	#[ "$(TARGET)" = "$(RTARGET)" ] || cp $(RTARGET) $(DESTDIR)$(CLIPROOT)/lib
	#[ "$(TARGET1)" = "$(RTARGET1)" ] || cp $(RTARGET1) $(DESTDIR)$(CLIPROOT)/lib
	mkdir -p $(DESTDIR)$(CLIPROOT)/doc/clip-odbc
	cp *.html $(DESTDIR)$(CLIPROOT)/doc/clip-odbc
	cp odbc.ch $(DESTDIR)$(CLIPROOT)/include
	mkdir -p $(DESTDIR)$(CLIPROOT)/doc/example/clip-odbc
	cp -R example/* $(DESTDIR)$(CLIPROOT)/doc/example/clip-odbc/

clean:
	rm -rf *.o odbc.c *.s *.bak *.a *.so *.dll *.exe odbc_ref.c *.ex *.nm Makefile

distclean: clean
	rm -f Makefile

