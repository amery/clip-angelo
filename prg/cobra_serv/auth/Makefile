ifdef CLIPROOT
include $(CLIPROOT)/include/Makefile.inc
endif
#CLIP	= $(CLIPROOT)/bin/clip

# need package libpam0g-dev

LIBS = -lpam
CC = gcc
CFLAGS = -O

all: pam-auth$(EXESUFF)
	 #sudo ./make_mod.sh; if [ ! -x pam-auth ]  ; then rm -f pam-auth ; fi
	 #./make_mod.sh; if [ ! -x pam-auth ]  ; then rm -f pam-auth ; fi
	 #./make_mod.sh; if [ $? != 0 ] ; then rm -f pam-auth ; fi

pam-auth$(EXESUFF): pam-auth.c
	$(CC) $(CFLAGS) -o $@ pam-auth.c $(LIBS) || exit 0

clean:
	rm -f pam-auth$(EXESUFF) *.bak *.o

install:
	mkdir -p $(DESTDIR)$(CLIPROOT)/cobra/auth
	cd $(DESTDIR)$(CLIPROOT)/cobra/auth; rm -f pam-auth$(EXESUFF)
	cp Makefile make_mod.sh other pam* $(DESTDIR)$(CLIPROOT)/cobra/auth
	cd $(DESTDIR)$(CLIPROOT)/cobra/auth; test -z $(DESTDIR) && sudo ./make_mod.sh || exit 0

