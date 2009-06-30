include Makefile.ini
#CLIP	= $(CLIPROOT)/bin/clip
CLIPFLAGS = -O -b -w -I ./include
CLIPLIBS  =
#LIBS = ./libclip-codb$(DLLSUFF)
LIBS = -lclip-codb
AB_OBJS  = ab_obj.o ab_menu.o ab_brow.o ab_init.o ab_funcs.o \
		ab_report.o
.SUFFIXES: .prg .o .po

MODULES = 	attr.po class.po counter.po default.po deposit.po \
				group.po index.po plugins.po report.po tcolumn.po tview.po user.po
DIRS_TO_SCAN =
#plugins reports

PROGS =
#codb_pack$(EXESUFF) codb_addobj$(EXESUFF) codb_ab$(EXESUFF) \
#	codb_make$(EXESUFF) codb_load$(EXESUFF) codb_export$(EXESUFF) codb_reindex$(EXESUFF) \
#	codb_arename$(EXESUFF) codb_rcheck$(EXESUFF) #test$(EXESUFF)

all : $(DIRS_TO_SCAN) $(PROGS) $(MODULES)

commit:
	../_cvs commit
update:
	../_cvs update -dP

clean : $(DIRS_TO_SCAN) sample
#	cd data; rm -f *.bak
#cd loaders; $(MAKE) clean
#	cd sample; $(MAKE) clean
#	cd plugins; rm -rf *.po *.bak *.log *.out *.po *core core* *.ex *.nm
#	cd reports; rm -rf *.po *.bak *.log *.po *core core* *.ex *.nm
	rm -rf *.exe *.o *.ppo *.c *.s *.a *.so *.dll* *.po *.pa *.log *.uri *.b *.BAK *.bak *~ core* *core
	rm -rf *.pro $(PROGS) test test2
	rm -rf *.ex *.nm

Makefile.ini :
	cp -f$(V) ../Makefile.ini ./

install : all

codb_ab$(EXESUFF): $(AB_OBJS) codb_ab.prg
#	cd plugins; $(CLIP) -pw *.prg
#	cd reports; $(CLIP) -pw *.prg
	$(CLIP) $(CLIPFLAGS) -o$@ -s -e -M codb_ab.prg $(AB_OBJS) $(CLIPLIBS) $(LIBS)

.prg.o:
	$(CLIP) $(CLIPFLAGS) $<

.prg.po:
	$(CLIP) $(CLIPFLAGS) -p $<

FORCE :

$(DIRS_TO_SCAN) sample example : Makefile FORCE
	$(Trace_o)
	$(E_if)if [ -d $@ ] ; then 																			\
					cd $@  																						\
					&& ( [ -f Makefile ] || ( [ -x configure ] && ./configure 0) )	\
					&& ( $(MAKE) $(MAKECMDGOALS)  		\
						||  $(MAKE) -f Makefile.in $(MAKECMDGOALS) ) ;				\
		fi
#					&& ( $(MAKE) $(MAKEFLAGS) $(ADDMAKECMDGOALS) $(MAKECMDGOALS)  		\
#						||  $(MAKE) -f Makefile.in $(MAKEFLAGS) $(ADDMAKECMDGOALS) $(MAKECMDGOALS) ) ;				\

