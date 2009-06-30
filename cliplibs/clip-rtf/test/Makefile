INCLUDEDIRS=-I. -I$(CLIPROOT)/include

ifdef CLIPROOT
	CLIP	= $(CLIPROOT)/bin/clip$(EXESUFF)
else
	CLIP = clip$(EXESUFF)
endif
CLIPFLAGS = -l -n -w
CLIPLIBS  =
BINS = rtfdemo$(EXESUFF)

.SUFFIXES: .prg .o .po

all: $(BINS)

rtfdemo$(EXESUFF) : rtfdemo.prg
	$(CLIP) -o$@-e -s -l -M rtfdemo.prg -lclip-rtf


clean:
	rm -f *.exe *.c *.o *.po *.log *.bak *.rtf $(BINS) *.ex *.nm

.prg.o:
	$(CLIP) $(CLIPFLAGS) $<

