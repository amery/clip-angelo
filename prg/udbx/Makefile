ifdef CLIPROOT
include $(CLIPROOT)/include/Makefile.inc
endif

#CLIP	= $(CLIPROOT)/bin/clip
CLIPFLAGS = -Obl -I./include
CLIPLIBS  = -L$(CLIPROOT)/lib -lclip-codb -lclip-gtk2 -lclip

OBJS  = abx_obj.o abx_menu.o abx_events.o abx_funcs.o abx_meta_browse.o abx_getobj.o \
	abx_data_browse.o

BIN = codb_abx$(EXESUFF)

.SUFFIXES: .prg .o .po

all: $(BIN)

install: all
	mkdir -p $(DESTDIR)$(CLIPROOT)/codb_abx/plugins
	tar cf - plugins | (cd $(DESTDIR)$(CLIPROOT)/codb_abx && tar xf -)
	#cp plugins/*.prg $(DESTDIR)$(CLIPROOT)/codb_abx/plugins
	#cp plugins/*.po $(DESTDIR)$(CLIPROOT)/codb_abx/plugins
	cp codb_abx$(EXESUFF) $(DESTDIR)$(CLIPROOT)/bin/ 
	#cp codb_abx$(EXESUFF) $(DESTDIR)$(CLIPROOT)/bin/

clean:
	rm -rf *.exe *.o *.c *.s *.a *.so *.po *.pa *.log *.uri *.b *.BAK *.bak *~ core* *core $(BIN)
	rm -rf *.ex *.nm
	cd plugins && rm -rf *.out *.ex *.nm *.po *.bak

$(BIN): codb_abx.prg $(OBJS)
	cd plugins; $(CLIP) -pw *.prg
	$(CLIP) -o$@ $(CLIPFLAGS) -se $< $(OBJS) $(CLIPLIBS)

commit:
	_cvs commit

.prg.o:
	$(CLIP) $(CLIPFLAGS) $<

.prg.po:
	$(CLIP) $(CLIPFLAGS) -p $<
