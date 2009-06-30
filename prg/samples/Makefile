#CLIP	= $(CLIPROOT)/bin/clip
CLIPFLAGS = -o$@ -O -w
CLIPLIBS  =
#GUILIB  = $(CLIPROOT)/lib/libxclip.a
SQLLIB	= $(CLIPROOT)/lib/libdbfsql.a
PGLIBS	= $(CLIPROOT)/lib/libpgclip.a /usr/lib/libpq.a /usr/lib/libcrypt.a
MSLIBS	= $(CLIPROOT)/lib/libmsclip.a /usr/lib/libmysqlclient.a
ORLIBS	= $(CLIPROOT)/lib/liborclip.a $(ORACLE_HOME)/lib/libclntsh.so
POBJS  = menu.po getfile.po
PAOBJS  = menu.po getfile.po
CLIP_LOCALE_ROOT=.
export CLIP_LOCALE_ROOT
.SUFFIXES: .prg .o .po

all : menu$(EXESUFF) fw_menu$(EXESUFF) getfile$(EXESUFF) tb_test$(EXESUFF) big_num$(EXESUFF) multilang$(EXESUFF)
		#pluralmsg$(EXESUFF)
install : all

clean:
	rm -rf *.exe *.uri *.c *.s *.o *.a *.so *.log *.pa *.b *.BAK *.bak *~ core* *core
	rm -rf locale.pot locale.mo *.ex *.nm
	rm -f menu$(EXESUFF) getfile$(EXESUFF) big_num$(EXESUFF) tb_test$(EXESUFF) fw_menu$(EXESUFF) multilang$(EXESUFF) pluralmsg$(EXESUFF)

menu$(EXESUFF):	menu.prg
	$(CLIP) $(CLIPFLAGS) -e -M menu.prg $(CLIPLIBS)

fw_menu$(EXESUFF): fw_menu.prg
	$(CLIP) $(CLIPFLAGS) -e -M fw_menu.prg $(CLIPLIBS)

getfile$(EXESUFF): getfile.prg
	$(CLIP) $(CLIPFLAGS) -e -M getfile.prg $(CLIPLIBS)

tb_test$(EXESUFF): tb_test.prg
	$(CLIP) $(CLIPFLAGS) -e -M tb_test.prg $(CLIPLIBS)

big_num$(EXESUFF): big_num.prg
	$(CLIP) $(CLIPFLAGS) -e -M big_num.prg $(CLIPLIBS)

multilang$(EXESUFF): multilang.prg
	$(CLIP) $(CLIPFLAGS) -e -M -E CLIP_MODULE="ml1" multilang.prg $(CLIPLIBS)
	clip_msgmerge
	clip_msgfmt

pluralmsg$(EXESUFF): pluralmsg.prg
#	$(CLIP) $(CLIPFLAGS) -E CLIP_MODULE="ml2" pluralmsg.prg
	$(CLIP) $(CLIPFLAGS) -e -M -E CLIP_MODULE="ml2" pluralmsg.prg $(CLIPLIBS)
	clip_msgmerge
	clip_msgfmt

mypsql$(EXESUFF): mypsql.prg
	$(CLIP) $(CLIPFLAGS) -e mypsql.prg $(SQLLIB) $(PGLIBS)

mymysql$(EXESUFF): mymysql.prg
	$(CLIP) $(CLIPFLAGS) -e mymysql.prg $(SQLLIB) $(MSLIBS)

mysqlplus$(EXESUFF): mysqlplus.prg
	$(CLIP) $(CLIPFLAGS) -e mysqlplus.prg $(SQLLIB) $(ORLIBS)

#gui_test$(EXESUFF): gui_test.prg $(GUILIB)
#	$(CLIP) $(CLPFLAGS) -e -s -M gui_test.prg `gtk-config --libs` $(GUILIB) -lz
