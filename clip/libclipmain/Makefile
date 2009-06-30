CFLAGS += -I.. -I../include -I../include.h
LIB = libclipmain.a
#OBJS = $(sort clipbase.o _clipboard.o cliphash.o _date.o _dbg.o _depsort.o diskutils.o _file1.o gettext.o integer.o \
#		_math.o _mem.o ncp.o rational.o _regex.o rt.tab.o _string.o _system.o _tcp.o _tcpserv.o)
#LIB
OBJS = $(sort cliphash.o rt.tab.o \
	   _file1.o _string.o _date.o \
	  clipbase.o _math.o diskutils.o _mem.o \
	  _system.o rational.o integer.o gettext.o _dbg.o \
	  _tcp.o _tcpserv.o _regex.o ncp.o _clipboard.o _depsort.o _ctools_s.o _file.o  _io.o _thread.o _util.o  \
			 clipdbg.o cliprt.o cliptbl.o clipvm.o \
			hash.o set.o )


ADDOBJS = ../compiler/coll.o ../libclipscreen/charset.o ../compiler/_win32.o ../compiler/list.o ../compiler/plural.tab.o

install all:	$(LIB)

clean:
	rm -f *.o *.a *.b *.bak *.BAK *.tmp *.ex *.nm \
	test_ref.c test.c test.log \
	*.dbf *.ntx *.cdx *.dbt *.fpt \
	$(LIB) test

../compiler/coll.o : ../compiler/coll.c
	$(Trace_o)
	cd ../compiler  																	\
	&& ( ( [ -x ./configure ] && ./configure ) || [ -f Makefile ] ) 	\
	&& $(MAKE)  coll.o

../compiler/list.o : ../compiler/list.c
	$(Trace_o)
	cd ../compiler  																	\
	&& ( ( [ -x ./configure ] && ./configure ) || [ -f Makefile ] ) 	\
	&& $(MAKE)  list.o

../compiler/plural.tab.o : ../compiler/plural.tab.c
	$(Trace_o)
	cd ../compiler  																	\
	&& ( ( [ -x ./configure ] && ./configure ) || [ -f Makefile ] ) 	\
	&& $(MAKE)  plural.tab.o

../compiler/_win32.o : ../compiler/_win32.c
	$(Trace_o)
	cd ../compiler  																	\
	&& ( ( [ -x ./configure ] && ./configure ) || [ -f Makefile ] ) 	\
	&& $(MAKE)  _win32.o

$(LIB): $(OBJS) $(ADDOBJS)
	rm -f$(V) $(LIB)
	clip_makelib$(SCRIPTSUFF) $(LIB) $(OBJS) $(ADDOBJS) && ranlib $(LIB)

../libclipscreen/charset.o: ../libclipscreen/charset.c Makefile
	$(Trace_o)
	cd ../libclipscreen  															\
	&& ( ( [ -x ./configure ] && ./configure ) || [ -f Makefile ] ) 	\
	&& $(MAKE) charset.o
#	$(CC) $(CFLAGS) -Ilibclipscreen -c libclipscreen/charset.c

