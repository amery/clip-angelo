ifdef CLIPROOT
	CLIP	= $(CLIPROOT)/bin/clip$(EXESUFF)
	CLIPFLAGS = -nwgl
else
	CLIP=clip$(EXESUFF)
	CLIPFLAGS = -nw
endif
CLIPLIBS  =-lclip-cti
OBJS  =
ADD_OBJS =
POBJS  =
.SUFFIXES: .prg .o .po

all: ctitest$(EXESUFF) ctifd$(EXESUFF) browsecombo$(EXESUFF)

clean:
	rm -rf *.o *.c *.a *.so *.po *.pa *.log *.b *.ex *.exe *.nm *.BAK *.bak *~ core* *core
	rm -f ctitest ctifd browsecombo

commit:
	cvs commit

update:
	cvs update -dP

ucommit: update commit

ctitest$(EXESUFF): ctitest.prg $(OBJS)
	$(CLIP) -o$@ -swe ctitest.prg $(OBJS) $(ADD_OBJS) $(CLIPLIBS)

ctifd$(EXESUFF): ctifd.prg $(OBJS)
	$(CLIP) -o$@ -swe -M  ctifd.prg $(OBJS) $(ADD_OBJS) $(CLIPLIBS)

ctit$(EXESUFF): ctit.prg $(OBJS)
#	$(CLIP) -swe -M  ctit.prg $(OBJS) $(ADD_OBJS) $(CLIPLIBS)
	$(CLIP) -o$@ -swe  ctit.prg $(OBJS) $(ADD_OBJS) $(CLIPLIBS) -lclip-fw

browsecombo$(EXESUFF): browsecombo.prg $(OBJS)
	$(CLIP) -o$@ -swe browsecombo.prg $(OBJS) $(ADD_OBJS) $(CLIPLIBS)

depend:
	makedepend -w80 -- *.prg 2>/dev/null

.prg.o:
	$(CLIP) $(CLIPFLAGS) $< $(CLIPLIBS)

.prg.po:
	$(CLIP) $(CLIPFLAGS) -p $< $(CLIPLIBS)

