CLIP	= clip$(EXESUFF)
CLIPFLAGS = -a -O -b
CLIPLIBS  = -lclip-codb

PROGS = my001$(EXESUFF) cgi_currency$(EXESUFF)


.SUFFIXES: .prg .o .po

all: $(PROGS)

clean:
	rm -rf *.exe *.uri *.c *.o *.a *.so *.po *.log *.pa *.b *.BAK *.bak *~ core* *core
	rm -rf $(PROGS) *.ex *.nm

my001$(EXESUFF):  my001.prg
	$(CLIP) $(CLIPFLAGS) -o$@ -esl -M my001.prg $(CLIPLIBS)

cgi_currency$(EXESUFF): cgi_currency.prg
	$(CLIP) $(CLIPFLAGS) -o$@ -e -M cgi_currency.prg $(CLIPLIBS)

