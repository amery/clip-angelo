ifdef CLIPROOT
include $(CLIPROOT)/include/Makefile.inc
endif
#CLIP	= clip
#CLIP	= $(CLIPROOT)/bin/clip
CLIPFLAGS = -O -b -l -w
CLIPLIBS  = -lclip-codb -lclip-r2d2
OBJS  = #cobra_conn.o
CHS  = #corba_serv.ch
MODULES = module1.po codbtview.po codbdlist.po
.SUFFIXES: .prg .o .po

all: cobra_serv$(EXESUFF) $(MODULES) AUTH WRAP

clean:
	rm -f *.exe *.o *.c *.s *.a *.so *.po *.pa *.log *.uri *.b *.BAK *.bak *~ core* *core
	rm -f cobra_serv$(EXESUFF) *.ex *.nm *.pro
	cd mod-ini; $(MAKE) clean
	cd auth; $(MAKE) clean
	cd tcp-wrap; $(MAKE) clean

cobra_serv$(EXESUFF) :cobra_serv.prg $(CHS) $(OBJS)
	$(CLIP) $(CLIPFLAGS) -esl -o$@ cobra_serv.prg $(OBJS) $(CLIPLIBS)
	cd mod-ini; $(CLIP) -pw *.prg

AUTH:
	cd auth; $(MAKE) all

WRAP:
	cd tcp-wrap; $(MAKE) all

install: all
	mkdir -p $(DESTDIR)$(CLIPROOT)/cobra
	cp cobra_serv$(EXESUFF) $(DESTDIR)$(CLIPROOT)/bin
	cp cobra_serv$(EXESUFF) $(DESTDIR)$(CLIPROOT)/cobra/
	#ln -sf $(DESTDIR)$(CLIPROOT)/bin/cobra_serv$(EXESUFF)  $(DESTDIR)$(CLIPROOT)/cobra/cobra_serv$(EXESUFF)
	cp cobra_serv.ini $(DESTDIR)$(CLIPROOT)/cobra
	mkdir -p $(DESTDIR)$(CLIPROOT)/cobra/mod-ini
	cp ./mod-ini/Makefile ./mod-ini/*.po ./mod-ini/*.prg $(DESTDIR)$(CLIPROOT)/cobra/mod-ini
	mkdir -p $(DESTDIR)$(CLIPROOT)/cobra/tcp-wrap
	cp ./tcp-wrap/Makefile ./tcp-wrap/tcp-* $(DESTDIR)$(CLIPROOT)/cobra/tcp-wrap
	cp $(MODULES) $(DESTDIR)$(CLIPROOT)/cobra/
	mkdir -p $(DESTDIR)$(CLIPROOT)/cobra/files
	cp ./auth/pam-err-msg $(DESTDIR)$(CLIPROOT)/cobra/files
	cd auth; $(MAKE) install
	#cd tcp-wrap; $(MAKE) install

.prg.o:
	$(CLIP) $(CLIPFLAGS) $<

.prg.po:
	$(CLIP) $(CLIPFLAGS) -n -p $<

