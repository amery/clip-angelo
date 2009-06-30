PRG=my_isql$(EXESUFF)
all: $(PRG)

clean:
	rm -f *.exe *.uri *.c *.o *.a *.so *.po *.log *.pa *.b *.BAK *.bak \
		*~ core* *core $(PRG) *.ex *.nm

$(PRG): $(PRG).prg
	clip$(EXESUFF) -o$@ -eMs $(PRG).prg -lclip-odbc

