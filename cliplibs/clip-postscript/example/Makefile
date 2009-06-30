# CLIP-POSTSCRIPT library examples Makefile
# Copyright (C) 2007, E/AS Software Foundation
# Web: http://eas.lrn.ru
ifndef CLIPROOT
	CLIPROOT=$(shell cd ../../../../; pwd)/cliproot
endif

include $(CLIPROOT)/include/Makefile.inc
CLIPINCLUDE = -I$(CLIPROOT)/include
CLIP = $(CLIPROOT)/bin/clip$(EXESUFF)

.SUFFIXES: .prg .o .po
.PHONY: all clean

# Form test
PRG=testform$(EXESUFF)
LIBS=-lclip-postscript -lclip-xml -lexpat

all: $(PRG)

clean:
	rm -f *.o *.bak core* *core $(PRG) *.log *.nm *.ex *.exe

$(PRG): testform.prg
	$(CLIP) -o$@ -ewsln $(CLIPINCLUDE) testform.prg $(LIBS)
