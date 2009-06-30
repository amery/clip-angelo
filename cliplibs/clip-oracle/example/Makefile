PRG=my_sqlplus$(EXESUFF)
all: $(PRG)

clean:
	rm -f *.exe *.uri *.c *.o *.a *.so *.po *.log *.pa *.b *.BAK *.bak \
		*~ core* *core $(PRG) *.ex *.nm

my_sqlplus$(EXESUFF): my_sqlplus.prg
	clip$(EXESUFF) -o$@ -eMs my_sqlplus.prg -lclip-oracle

