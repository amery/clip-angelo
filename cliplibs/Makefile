#

DIRS_TO_SCAN = 	clip-bzip2 		clip-codb 			clip-com 		clip-crypto clip-fcgi 	clip-gd \
			clip-glade2 	clip-gtk2 			clip-gzip 		clip-mysql 	clip-oasis 	clip-odbc 	clip-oracle \
			clip-postgres 	clip-postscript 	clip-r2d2 		clip-rtf 	clip-xml 	clip-ui

-include Makefile.01

#ifdef CLIPROOT
#	CLIP = $(CLIPROOT)/bin/clip$(C64)$(EXESUFF)
#else
	CLIP = clip$(C64)$(EXESUFF)
#endif

CFLAGS = -fPIC -I.
CLIPFLAGS = -l -w -O -n -I . -I .. -I ../include -I$(CLIPROOT)

export CFLAGS CLIP CLIPFLAGS CLIPROOT
#clip-cti  clip-ctcom clip-fw clip-gtk clip-gtkextra

all local user system install lib: $(DIRS_TO_SCAN)
	for dir in $(dirs); \
	do \
		(cd $$dir ;\
		[ -x ./configure ] && ./configure ;\
		$(MAKE) $@ );\
	done

Makefile.01 : configure
	./configure


clean : $(DIRS_TO_SCAN) 
	for dir in $(dirs); \
		do (cd $$dir && $(MAKE) clean || true; rm -rf debian build-stamp $dir.spec);\
	done
	cd clip-interbase \
		&& $(MAKE) clean
	rm -fv Makefile.01

distclean: clean
	for dir in $(dirs); \
		do (cd $$dir && $(MAKE) distclean || true);\
	done

deb: clean
	for dir in $(dirs); \
		do ./mkdeb $$dir; \
	done

rpm:
	for dir in $(dirs); \
		do ./mkrpm $$dir; \
	done

rpm-opt:
	for dir in $(dirs); \
		do ./mkrpm -opt $$dir; \
	done

tgz tbz:
	for dir in $(dirs); \
		do ./mktgz $$dir; \
	done


./Makefile.ini : Makefile Makefile.in
	cp -f$(V) ../Makefile.ini ./
FORCE :

$(DIRS_TO_SCAN) example : Makefile FORCE
	$(Trace_o)
	$(E_if)if [ -d $@ ] ; then 																			\
					cd $@  																						\
					&& ( [ -f Makefile ] || ( [ -x configure ] && ./configure 0) )	\
					&& ( $(MAKE) $(MAKECMDGOALS)  		\
						||  $(MAKE) -f Makefile.in $(MAKECMDGOALS) ) ;				\
		fi
#					&& ( $(MAKE) $(MAKEFLAGS) $(ADDMAKECMDGOALS) $(MAKECMDGOALS)  		\
#						||  $(MAKE) -f Makefile.in $(MAKEFLAGS) $(ADDMAKECMDGOALS) $(MAKECMDGOALS) ) ;				\

