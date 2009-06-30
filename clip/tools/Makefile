# main parts
#
#

BINS	=	clipar$(EXESUFF) clip_conv$(EXESUFF) clip_dbg$(EXESUFF) clip_trans$(EXESUFF) \
			clip_hashextract$(EXESUFF) cliphash$(EXESUFF) \
			genlist$(EXESUFF)	gen_tbl$(EXESUFF) $(PO_BINS)

SCRIPTS = clip_cp$(SCRIPTSUFF) clip_makelib$(SCRIPTSUFF) clip_makeslib$(SCRIPTSUFF) clip_msgfmt$(SCRIPTSUFF) clip_msgmerge$(SCRIPTSUFF)

DIRS_TO_SCAN =

export CC CFLAGS $(PO_BINS)
export CLIP CLIPFLAGS
export MEMDBG MEMDBGFLAGS
export CLIPROOT BINDIR
#export CVS_RSH

#main: install

all: links $(DIRS_TO_SCAN) $(BINS)

links:

FORCE :

../compiler/coll.o: ../compiler/coll.c Makefile
	$(Trace_o)
	cd ../compiler  																						\
	&& ( ( [ -x ./configure ] && ./configure ) || [ -f Makefile ] ) 				\
	&& $(MAKE) coll.o

../compiler/_win32.o: ../compiler/_win32.c Makefile
	$(Trace_o)
	cd ../compiler  																						\
	&& ( ( [ -x ./configure ] && ./configure ) || [ -f Makefile ] ) 				\
	&& $(MAKE) _win32.o

../libclipscreen/charset.o: ../libclipscreen/charset.c Makefile
	$(Trace_o)
	cd ../libclipscreen  																						\
	&& ( ( [ -x ./configure ] && ./configure ) || [ -f Makefile ] ) 				\
	&& $(MAKE) charset.o

clip_hash.o: ../libclipmain/cliphash.c
	$(CC) $(CFLAGS) -DSTANDALONE -o clip_hash.o -c $<

lex.hash.o: lex.hash.c

lex.hash.c: hasher.lex
	flex -Phash hasher.lex

clicutil.o: clicutil.c clic.tab.h
#Makefile: Makefile.in configure
#	./configure -f

install : all
#	cp -f$(V) $(PO_BINS) $(DESTDIR)$(CLIPROOT)/bin
	chmod 755 $(BINS) $(SCRIPTS)
	cp -f$(V) $(BINS) $(SCRIPTS) $(DESTDIR)$(CLIPROOT)/bin
	cp -f$(V) clip_bug  $(DESTDIR)$(CLIPROOT)/bin/clip_bug$(SCRIPTSUFF)
	cp -f$(V) joinlib  $(DESTDIR)$(CLIPROOT)/bin/joinlib$(SCRIPTSUFF)
	cp -f$(V) inst_end  $(DESTDIR)$(CLIPROOT)/bin/inst_end$(SCRIPTSUFF)
	cp -f$(V) add_meta_tag  $(DESTDIR)$(CLIPROOT)/bin/add_meta_tag$(SCRIPTSUFF)
	cp -f$(V) lowname $(DESTDIR)$(CLIPROOT)/bin/lowname$(SCRIPTSUFF)
	cp -f$(V) tconv $(DESTDIR)$(CLIPROOT)/bin/tconv$(SCRIPTSUFF)
	ln -sf$(V) $(DESTDIR)$(CLIPROOT)/bin/* $(BINDIR)/

clipar$(EXESUFF) :	clipar.o ../compiler/coll.o #getopt.o
	$(CC) $(CFLAGS) $(STATICLINK) -o $@ clipar.o ../compiler/coll.o $(MLIB) #getopt.o

clip_conv$(EXESUFF) :  clip_conv.o ../libclipscreen/charset.o
	$(CC) $(CFLAGS) -o $@ clip_conv.o ../libclipscreen/charset.o $(MLIB)

clip_dbg$(EXESUFF) :  clip_dbg.o ../compiler/coll.o ../compiler/_win32.o
	$(CC) $(CFLAGS) -o $@ clip_dbg.o ../compiler/coll.o ../compiler/_win32.o $(ADDLIBS)

clip_trans$(EXESUFF) :  clip_trans.o
	$(CC) $(CFLAGS) -o $@ clip_trans.o

clip_msgmerge$(SCRIPTSUFF) : clip_msgmerge.in configure
	./configure -f

clip_makeslib$(SCRIPTSUFF) : clip_makeslib.in configure
	./configure -f

clip_cp$(SCRIPTSUFF) : clip_cp.in configure
	./configure -f

clip_makelib$(SCRIPTSUFF) : clip_makelib.in configure
	./configure -f

clip_msgfmt$(SCRIPTSUFF) : clip_msgfmt.in configure
	./configure -f

cliphash$(EXESUFF) : calchash.o clip_hash.o
	$(CC) $(BCFLAGS) -o $@ calchash.o clip_hash.o
	rm -rf bin
	mkdir bin
	cd ../bin;	ln -sf$(V) ../tools/cliphash$(EXESUFF) .

genlist$(EXESUFF) : genlist.c
	$(CC) $(CFLAGS) -o $@ genlist.c

gen_tbl$(EXESUFF) : gen_tbl.o ../libclipscreen/charset.o ../compiler/coll.o
	$(CC) $(CFLAGS) -o $@  gen_tbl.o ../libclipscreen/charset.o ../compiler/coll.o $(MLIB)

clip_hashextract$(EXESUFF): lex.hash.o clip_hash.o
	$(CC) -O -o $@ lex.hash.o clip_hash.o

po_extr$(EXESUFF): po_util.o ../compiler/coll.o po_extr.o
	$(CC) $(CFLAGS) -o $@ po_extr.o po_util.o ../compiler/coll.o $(MLIB)

po_subst$(EXESUFF): po_util.o ../compiler/coll.o po_subst.o
	$(CC) $(CFLAGS) -o $@ po_subst.o po_util.o ../compiler/coll.o $(ICONV_LIB) $(MLIB)

po_compat$(EXESUFF): po_util.o po_compat.o
	$(CC) $(CFLAGS) -o $@ po_compat.o po_util.o


clean :  $(DIRS_TO_SCAN) cleanc

cleanc :
	rm -f$(V) *.ex *.nm charsets
	rm -rf$(V) $(BINS) *.o *.a *.so *.dll *.pa *.b *.BAK *.bak *~ core* *core\
		*.output test dtest clip_dbg$(EXESUFF) clip_trans$(EXESUFF) \
		bin *.exe locale.pot license.h clipcfg.h clipcfg.sh \
		clip_cp$(SCRIPTSUFF) clip_makelib$(SCRIPTSUFF) clip_makeslib$(SCRIPTSUFF) \
		calchash$(EXESUFF) genlist$(EXESUFF) hashextract$(EXESUFF) clip_hashextract$(EXESUFF) \
		clip_msgfmt$(SCRIPTSUFF) clip_msgmerge$(SCRIPTSUFF) Makefile.inc Makefile.tmp \
		Makefile.incl  || true

distclean: clean
	rm -f$(V) configure.flags
	rm -f$(V) Makefile.inc clipcfg.sh clipcfg.h clip_msgmerge$(SCRIPTSUFF) \
		clip_msgfmt$(SCRIPTSUFF) clip_makeslib$(SCRIPTSUFF) clip_makelib$(SCRIPTSUFF) clip_cp$(SCRIPTSUFF) \
		license.txt license.h || true
	rm -f$(V) cliproot || true

.prg.c:
	$(CLIP) $(CLIPFLAGS) -c $<

.prg.po:
	$(CLIP) $(CLIPFLAGS) -p $<

.c.o:
	$(CC) $(CFLAGS) -c $<

depend:
#	$(MAKE) hashcode.h
	makedepend -Y  -- $(CFLAGS) -- $(SRCS) 2>/dev/null

commit:
	./_cvs_commit

update:
	./_cvs_update

ucommit:
	./_cvs_commit
	./_cvs_update

shell:
	sh

$(DIRS_TO_SCAN) example : Makefile FORCE
	$(Trace_o)
	$(E_if)if [ -d $@ ] ; then 																			\
					cd $@  																						\
					&& ( ( [ -x ./configure ] && ./configure ) || [ -f Makefile ] ) 				\
					&& $(MAKE) $(MAKEFLAGS) $(ADDMAKECMDGOALS) $(MAKECMDGOALS) ; 				\
		fi


# DO NOT DELETE

