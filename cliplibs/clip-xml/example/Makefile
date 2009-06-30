# CLIP-XML library examples Makefile
# Copyright (C) 2006, E/AS Software Foundation
# Web: http://eas.lrn.ru
# Copyright (C) 2006, ITK
# Web: http://www.itk.ru
ifndef CLIPROOT
	CLIPROOT=$(shell cd ../../../../; pwd)/cliproot
endif

include $(CLIPROOT)/include/Makefile.inc
CLIPINCLUDE = -I$(CLIPROOT)/include
CLIP = clip$(EXESUFF)
C_FLAGS=-g

.SUFFIXES: .prg .o .po
.PHONY: all clean

# Widget tests
LIBS=-lclip-xml

PROGS=test_xml$(EXESUFF) test$(EXESUFF)

EXTRALIBS=

all: $(PROGS)

clean:
	rm -f *.o core* *core $(PROGS) *.log *.nm *.ex *.exe $(OUTPUT)
	rm -f *.exe *.c *.o *.a *.so *.po *.log *.pa *.b *.BAK *.bak *~ core* *core
	rm -rf *.ex *.nm

test_xml$(EXESUFF): test_xml.prg
	$(CLIP) -o$@ -ewsln  $(C_FLAGS) $(CLIPINCLUDE) test_xml.prg $(LIBS) $(EXTRALIBS)

test$(EXESUFF): test.prg
	clip$(EXESUFF) -o$@ -esg test.prg $(LIBS)

