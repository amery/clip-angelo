PRG=bz2$(EXESUFF)
all: $(PRG)

clean:
	rm -f *.exe *.uri *.c *.o *.a *.so *.po *.log *.pa *.b *.BAK *.bak \
		*~ core* *core $(PRG) *.bz2 *.ex *.nm

bz2$(EXESUFF): bz2.prg
	clip$(EXESUFF) -o$@ -e -s -l -M bz2.prg -lclip-bzip2

