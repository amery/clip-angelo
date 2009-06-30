ifdef CLIPROOT
include $(CLIPROOT)/include/Makefile.inc
endif
#CLIP	= clip
#CLIP	= $(CLIPROOT)/bin/clip
CLIPFLAGS = -w -O -b -l
CLIPLIBS  = -lclip-codb -lcodb-query
TARGET = codb$(EXESUFF)
OBJS  = codb.o
.SUFFIXES: .prg .o .po

all: $(TARGET)

clean:
	rm -f *.exe *.o *.s *.c *.a *.so *.po *.pa *.log *.uri *.b *.BAK *.bak *~ core* *core
	rm -f $(TARGET) *.ex *.nm

$(TARGET): $(OBJS)
	$(CLIP) $(CLIPFLAGS) -o$@ -eslM $(OBJS) $(CLIPLIBS)

install: all
	mkdir -p $(DESTDIR)$(CLIPROOT)/bin
	cp $(TARGET) $(DESTDIR)$(CLIPROOT)/bin

.prg.o:
	$(CLIP) $(CLIPFLAGS) -n $<

