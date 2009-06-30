ifdef CLIPROOT
include $(CLIPROOT)/include/Makefile.inc
endif
#CLIP	= $(CLIPROOT)/bin/clip
CLIPFLAGS = -o$@ -O -w
CLIPLIBS  =
PGLIBS	= libpgclip.a libpq.a libcrypt.a
MSLIBS	= libmsclip.a libmysqlclient.a
ORLIBS	= liborclip.a $(ORACLE_HOME)/lib/libclntsh.so
# WWWLIBS - links all RDBMS libraries. Remove unused ones.
#WWWLIBS	= libdbfsql.a $(PGLIBS) $(MSLIBS) $(ORLIBS)
WWWLIBS	=
POBJS	= menu.po getfile.po
PAOBJS	= menu.po getfile.po
.SUFFIXES: .prg .o .po

all: $(sort clip_run$(EXESUFF) clip_prg$(EXESUFF) clip_we$(EXESUFF) clip_bl$(EXESUFF) www_clip$(EXESUFF) wcl2prg$(EXESUFF) clip_blank$(EXESUFF) sqlrun$(EXESUFF) \
	clip_hv$(EXESUFF) clip_fl$(EXESUFF) clip_dbf2txt$(EXESUFF) )

clean:
	rm -rf *.uri *.c *.s *.o *.a *.po *.log *.pa *.b *.BAK *.bak *~ core* *core \
		clip_run clip_prg clip_bl www_clip wcl2prg sqlrun \
		clip_blank clip_we clip_hv clip_fl clip_dbf2txt *.exe
	rm -rf *.ex *.nm

clip_run: clip_run.prg
	$(CLIP) $(CLIPFLAGS) -e -M clip_run.prg $(CLIPLIBS)
#	$(CLIP) -p test.prg
#	mv test.po test.po.0
#	#echo "#!./clip_run" >test.po
#	echo "#!"$(CLIPROOT)/bin/clip_run >test.po
#	cat test.po.0 >>test.po
#	chmod +x test.po

install:all
	cp clip_run$(EXESUFF) $(DESTDIR)$(CLIPROOT)/bin
	cp clip_prg$(EXESUFF) $(DESTDIR)$(CLIPROOT)/bin
	cp clip_bl$(EXESUFF) $(DESTDIR)$(CLIPROOT)/bin
	cp clip_we$(EXESUFF) $(DESTDIR)$(CLIPROOT)/bin
	cp .macro $(DESTDIR)$(CLIPROOT)/etc
	cp .templ $(DESTDIR)$(CLIPROOT)/etc
	cp clip_hv$(EXESUFF) $(DESTDIR)$(CLIPROOT)/bin
	cp clip_fl$(EXESUFF) $(DESTDIR)$(CLIPROOT)/bin
	cp clip_dbf2txt$(EXESUFF) $(DESTDIR)$(CLIPROOT)/bin
	cp clip_blank$(EXESUFF) $(DESTDIR)$(CLIPROOT)/bin
	cp www_clip$(EXESUFF) $(DESTDIR)$(CLIPROOT)/bin
	cp wcl2prg$(EXESUFF) $(DESTDIR)$(CLIPROOT)/bin
	cp sqlrun$(EXESUFF) $(DESTDIR)$(CLIPROOT)/bin


clip_prg$(EXESUFF): clip_prg.prg
	$(CLIP) $(CLIPFLAGS) -e -M clip_prg.prg $(CLIPLIBS)

clip_run$(EXESUFF): clip_run.prg
	$(CLIP) $(CLIPFLAGS) -e -M clip_run.prg $(CLIPLIBS)

clip_bl$(EXESUFF): clip_bl.prg
	$(CLIP) $(CLIPFLAGS) -e -M clip_bl.prg $(CLIPLIBS)

clip_we$(EXESUFF): clip_we.prg
	$(CLIP) $(CLIPFLAGS) -e -M clip_we.prg $(CLIPLIBS)

clip_hv$(EXESUFF): clip_hv.prg
	$(CLIP) $(CLIPFLAGS) -e -M clip_hv.prg $(CLIPLIBS)

clip_fl$(EXESUFF): clip_fl.prg
	$(CLIP) $(CLIPFLAGS) -e -M clip_fl.prg $(CLIPLIBS)

clip_dbf2txt$(EXESUFF): clip_dbf2txt.prg
	$(CLIP) $(CLIPFLAGS) -e -M clip_dbf2txt.prg $(CLIPLIBS)

clip_blank$(EXESUFF): clip_blank.prg
	$(CLIP) $(CLIPFLAGS) -e clip_blank.prg $(CLIPLIBS)

www_clip$(EXESUFF): www_clip.prg
	$(CLIP) $(CLIPFLAGS) -e www_clip.prg $(CLIPLIBS)  $(WWWLIBS)

wcl2prg$(EXESUFF): wcl2prg.prg
	$(CLIP) $(CLIPFLAGS) -e wcl2prg.prg $(CLIPLIBS)

sqlrun$(EXESUFF): sqlrun.prg
	$(CLIP) $(CLIPFLAGS) -e sqlrun.prg $(CLIPLIBS)  $(WWWLIBS)

