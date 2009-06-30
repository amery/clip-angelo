ifndef CLIPROOT
	CLIPROOT=$(shell cd ../../../; pwd)/cliproot
endif

include $(CLIPROOT)/include/Makefile.inc

TARGET=libclip-crypto$(DLLSUFF)
ATARGET=libclip-crypto.a
RTARGET=libclip-crypto$(DLLREALSUFF)
OBJS=crypto.o
LIBS=-lcrypto $(C_LIBS)

all: have_lib	$(TARGET) $(ATARGET)


have_lib:
	test -f /usr/include/openssl/evp.h
	awk 'BEGIN{FS="\""} /^#define[ \t]+[SL]N_/{print "\""$$2"\","}' /usr/include/openssl/objects.h > names.h

$(TARGET): $(OBJS)
	clip_makeslib$(SCRIPTSUFF) $(TARGET) $(OBJS) $(LIBS)

$(ATARGET): $(OBJS)
	clip_makelib$(SCRIPTSUFF) $(ATARGET) $(OBJS) $(LIBS)

crypto.o: crypto.c hashcode.h
	$(CC) $(C_FLAGS) -c crypto.c

hashcode.h:
#	echo '#define HASH_ferror ' $$(echo ferror| $(CLIPROOT)/bin/cliphash | cut -d' ' -f2) >hashcode.h

install:all
#	clip_cp$(SCRIPTSUFF) $(TARGET) $(CLIPROOT)/lib
	clip_cp$(SCRIPTSUFF) libclip-crypto $(DESTDIR)$(CLIPROOT)/lib
	#[ "$(TARGET)" = "$(RTARGET)" ] || cp $(RTARGET) $(DESTDIR)$(CLIPROOT)/lib
#	mkdir -p $(CLIPROOT)/doc/clip-crypto
#	cp *.html $(CLIPROOT)/doc/clip-crypto
	#-cd tdoc && $(MAKE) install
	mkdir -p $(DESTDIR)$(CLIPROOT)/doc/example/clip-crypto
	cp -R example/* $(DESTDIR)$(CLIPROOT)/doc/example/clip-crypto/

clean:
	rm -rf *.o *.bak *.a *.so hashcode.h *.dll *.exe names.h *.ex *.nm
	#-cd tdoc && $(MAKE) clean

distclean: clean
	#-cd tdoc && $(MAKE) distclean



