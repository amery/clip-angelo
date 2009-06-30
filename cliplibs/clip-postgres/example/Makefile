PRG=my_psql$(EXESUFF)
all: $(PRG)

clean:
	rm -f *.exe *.uri *.c *.o *.a *.so *.po *.log *.pa *.b *.BAK *.bak \
		*~ core* *core $(PRG) *.ex *.nm

my_psql$(EXESUFF): my_psql.prg
	clip -o$@ -eMs my_psql.prg -lclip-postgres

