include $(CLIPROOT)/include/Makefile.inc

TARGET=libclip-netto$(DLLSUFF)
RTARGET=libclip-netto$(DLLREALSUFF)
LIBS=libclip-netto.a

all : $(TARGET)

$(TARGET):
	cd misc && $(MAKE)
	clip_makeslib$(SCRIPTSUFF) $(TARGET) $(LIBS)

clean distclean*:
	cd misc && $(MAKE) clean
	rm -f $(LIBS) $(TARGET) $(RTARGET) *.ex *.nm *.bak

install : all $(TARGET)
	clip_cp$(SCRIPTSUFF) $(TARGET) $(DESTDIR)$(CLIPROOT)/lib
	#[ "$(TARGET)" = "$(RTARGET)" ] || cp $(RTARGET) $(DESTDIR)$(CLIPROOT)/lib
	clip_cp$(SCRIPTSUFF) $(LIBS) $(DESTDIR)$(CLIPROOT)/lib
	#mkdir -p $(CLIPROOT)/doc/clip-oasis/netto
	#cp doc/netto*.* $(CLIPROOT)/doc/clip-oasis/netto
	#cp doc/readme*.* $(CLIPROOT)/doc/clip-oasis/netto
	mkdir -p $(DESTDIR)$(CLIPROOT)/include/netto
	cp include/*.ch $(DESTDIR)$(CLIPROOT)/include/netto
	cp include/*.h $(DESTDIR)$(CLIPROOT)/include/netto

