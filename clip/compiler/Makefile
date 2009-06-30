ifndef CLIPROOT
	CLIPROOT=$(shell cd ../../../; pwd)/cliproot
endif

include $(CLIPROOT)/include/Makefile.inc

# main parts
#
#

#export CC
#DBGFLAGS	= -g #-DMEMDBG
#WARNFLAGS	= -Wimplicit -Wall

#CLIPFLAGS	= -g -v2 -N#-O
#CLIPFLAGS	= -v2 -N#-O
#CVS_RSH = ./ssh.sh
#YACC = bison
#CLIPROOT = $(CLIP_ROOT)
# strip on install
#ISTRIP = --strip
#CLIP_NAMES ?= 1
#export CLIP_NAMES

.SUFFIXES: .prg .c .po
.PHONY:	tests libclipscreen hash

SRCS	= coll.c list.c hash.c getopt.c getopt1.c clicutil.c \
	  clic.y clic.lex file.c node.c locale.c \
	  clip.c clicgt.c _win32.c

OBJS	= $(sort clic.tab.o lex.yy.o lex.cl.o coll.o list.o getopt.o getopt1.o \
	  clicutil.o  clip.o file.o node.o clip_hash.o ../libclipscreen/charset.o locale.o \
	  clicgt.o plural.tab.o _win32.o )
OBJDIR	= .
LIBS	= -lm

LIB	= libclip.a
SLIB	= libclip$(DLLSUFF)
SLIBREAL = libclip$(DLLREALSUFF)
LIBOBJS = $(sort cliprt.o clipdbg.o cliphash.o clipvm.o hash.o rt.tab.o coll.o _io.o \
	  cliptbl.o set.o _util.o _file.o _file1.o _string.o _date.o \
	  list.o clipbase.o _math.o _ctools_s.o diskutils.o _mem.o \
	  _system.o _thread.o rational.o integer.o charset.o gettext.o _dbg.o \
	  _tcp.o _tcpserv.o _regex.o ncp.o _clipboard.o plural.tab.o _depsort.o _win32.o )

EXTOBJS =

BIN=$(CLIP)
#clipar$(EXESUFF) \
#	clip_dbg$(EXESUFF) clip_trans$(EXESUFF) gen_tbl$(EXESUFF) \
#	clip_conv$(EXESUFF) \
#	$(PO_BINS)
BINS = $(BIN)

SCRIPTS=add_meta_tag joinlib.sh lowname tconv \
	clip_msgmerge clip_msgfmt clip_makeslib \
	clip_cp clip_makelib \

#koitbl.o alttbl.o

HASHSRCS = cliprt.c clipdbg.c clipvm.c clipmain.c rt.y _io.c \
	   set.c _util.c _file.c _file1.c _string.c _date.c \
	   clipbase.c _math.c _ctools_s.c diskutils.c _mem.c \
	  _system.c _thread.c rational.c integer.c \
	   _dbg.c _tcp.c _tcpserv.c _regex.c _thread.c

# mapbase.o
CLIP_CLD =
#clip_cld/clipcld
RDDBASE = libcliprdd/libcliprdd.a
CLASSLIB = libclipclasses/libclipclasses.a
FUNCSLIB = libclipfunctions/libclipfunctions.a
#CODBLIB = codb/libclip-codb.a codb/libcodb-query.a
SCREEN = libclipscreen/libclipscreen.a
TERMCAP = #libclipscreen/libtermcap.a
SQL = libclipdbfsql/libclipdbfsql.a

BASES =  $(RDDBASE) libclipmain/libclipmain.a


ALIBS = $(BASES) $(SCREEN) $(TASK) $(TERMCAP) $(DBGLIB) $(CLASSLIB) $(FUNCSLIB)\
	$(SQL)
DIRS_TO_SCAN =
#libclipclasses libclipdbfsql libclipfunctions libclipmain libcliprdd libclipscreen libcliptask

#DISPLIB=-lncurses
#DISPLIB=$(TERMCAP)

SYS = $OSNAME
#SYS := $(shell ./sys_name.sh)

#SYSTEM_NAME := $(shell ./sys_name.sh)
SYSTEM_NAME = $(OSNAME)

DEFS	=
# -DVERSION=\"0.1\"
# -DSYSTEM=\"$(SYSTEM_NAME)\"

#CFLAGS  = -I. -I../include -I../include.h $(C_FLAGS) $(ADD_CFLAGS) $(DEFS) $(MEMDBGFLAGS)
#BCFLAGS = -I. -I../include -I../include.h $(C_FLAGS) $(ADD_CFLAGS) $(DEFS)
#  $(WARNFLAGS) $(DBGFLAGS) $(OPTFLAGS) -I$(INCLUDE)

export CC CFLAGS
export CLIP CLIPFLAGS
export MEMDBG MEMDBGFLAGS
export CLIPROOT BINDIR
#export CVS_RSH

#main: install

all: links $(BIN)

links:

FORCE :

../libclipscreen/charset.o: ../libclipscreen/charset.c Makefile
	$(Trace_o)
	cd ../libclipscreen  																	\
	&& ( ( [ -x ./configure ] && ./configure ) || [ -f Makefile ] ) 			\
	&& $(MAKE) charset.o

$(BIN):  $(OBJS) seq_no.txt $(MLIB)
	$(CC) $(CFLAGS) $(STATICLINK) -DSEQ_NO=\"`cat seq_no.txt`\" 	\
		-o $@ version.c 															\
		$(OBJS) $(LIBS) $(ICONV_LIB) $(MLIB)
		cp -fupv $@ $(DESTDIR)$(CLIPROOT)/bin/
		ln -sf$(V) $(DESTDIR)$(CLIPROOT)/bin/$@ $(BINDIR)/

seq_no.txt:
	cp -fpu$(V) ../tools/seq_no.sh ./.

clip_hash.o: ../libclipmain/cliphash.c
	$(CC) $(CFLAGS) -DSTANDALONE -o clip_hash.o -c $<

clean :  $(DIRS_TO_SCAN) cleanc

cleanc :
	rm -f$(V) *.ex *.nm charsets
	rm -f$(V) $(BIN) *.o *.a *.so *.dll *.pa *.b *.BAK *.bak *~ core* *core\
		*.output test dtest clip clipar clip_dbg clip_trans \
		bin *.exe locale.pot license.h clipcfg.h clipcfg.sh \
		clip_cp clip_makelib clip_makeslib \
		calchash genlist hashextract clip_hashextract \
		clip_msgfmt clip_msgmerge Makefile.inc Makefile.tmp \
		Makefile.incl

distclean : clean
	rm -f$(V) configure.flags
	rm -f$(V) Makefile.inc clipcfg.sh clipcfg.h clip_msgmerge \
		clip_msgfmt clip_makeslib clip_makelib clip_cp \
		license.txt license.h
	rm -f$(V) cliproot

install : all
	cp -f$(V) $(BIN) $(DESTDIR)$(CLIPROOT)/bin

.prg.c:
	$(CLIP) $(CLIPFLAGS) -c $<

.prg.po:
	$(CLIP) $(CLIPFLAGS) -p $<

cliprt.o: cliprt.c
	$(CC) $(CFLAGS) -c -O $<

.c.o:
	$(CC) $(CFLAGS) -c $<

_ctools_s.o: _ctools_s.c
	$(CC) $(CFLAGS) -O -c  _ctools_s.c

_math.o: _math.c
	$(CC) $(CFLAGS) -O0 -c _math.c


clic.tab.o: clic.tab.c ../include.h/clic.h
	$(CC) $(CFLAGS) -c $<

lex.yy.o: lex.yy.c
	$(CC) $(CFLAGS) -c $<

lex.cl.o: lex.cl.c
	$(CC) $(CFLAGS) -c $<

clic.tab.c: clic.y ../include.h/clic.h
	$(YACC) -bclic -v -d clic.y

rt.tab.c: rt.y ../include.h/clic.h
	$(YACC) -brt -prt -v -d rt.y

plural.tab.c: plural.y
	$(YACC) -bplural -pplural plural.y

plural.tab.o: plural.tab.c

#note: flex -i for case-insencitive parser
lex.yy.c: clic.lex clic.tab.h
	flex -i clic.lex

lex.cl.c: clic_c.lex clic.tab.h
	flex -Pcl clic_c.lex

lex.hash.o: lex.hash.c

lex.hash.c: hasher.lex
	flex -Phash hasher.lex

clicutil.o: clicutil.c clic.tab.h

clic_name.o: clic_name.c

clic_name.c: clic.tab.c
	echo "#include <stdio.h>" >clic_name.c
	echo "#ifndef YYDEBUG" >>clic_name.c
	echo "#define YYDEBUG 0" >>clic_name.c
	echo "#endif" >>clic_name.c
	echo "#if !YYDEBUG" >>clic_name.c
	echo "#define static" >>clic_name.c
	sed -n -e "/^\(.*\)yytname[ \t]*\[\]/,/^};/p" clic.tab.c >> clic_name.c
	echo "int yymaxtoken=sizeof(yytname)/sizeof(char*);" >>clic_name.c
	echo "#endif" >>clic_name.c

depend:
#	$(MAKE) hashcode.h
	makedepend -Y  -- $(CFLAGS) -- $(SRCS) 2>/dev/null

lex: yacc _lex.yy.c _lex.cl.c _lex.hash.c
yacc: clic.tab.c rt.tab.c plural.tab.c

commit:
	./_cvs_commit

update:
	./_cvs_update

ucommit:
	./_cvs_commit
	./_cvs_update

shell:
	sh

#modu.po: modu.prg $(CLIP)

$(DIRS_TO_SCAN) example : Makefile FORCE
	$(Trace_o)
	$(E_if)if [ -d $@ ] ; then 																			\
					cd $@  																						\
					&& ( ( [ -x ./configure ] && ./configure ) || [ -f Makefile ] ) 				\
					&& $(MAKE) $(MAKEFLAGS) $(ADDMAKECMDGOALS) $(MAKECMDGOALS) ; 				\
		fi


# DO NOT DELETE

