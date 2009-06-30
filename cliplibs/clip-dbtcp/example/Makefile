PRG=my_dbtcp$(EXESUFF)
all: $(PRG)

clean:
	rm -f *.exe *.uri *.c *.o *.a *.so *.po *.log *.pa *.b *.BAK *.bak \
		*~ core* *core $(PRG) *.ex *.nm

my_dbtcp$(EXESUFF): my_dbtcp.prg
	clip$(EXESUFF) -p$@ -eMs my_dbtcp.prg -lclip-dbtcp

