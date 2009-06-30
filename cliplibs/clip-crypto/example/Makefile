PRG=crypto$(EXESUFF)
all: $(PRG)

clean:
	rm -f *.exe *.uri *.c *.o *.a *.so *.po *.log *.pa *.b *.BAK *.bak \
		*~ core* *core $(PRG) *.ex *.nm

crypto$(EXESUFF): crypto.prg
	clip$(EXESUFF) -o$@ -e -s -l -M crypto.prg -lclip-crypto

