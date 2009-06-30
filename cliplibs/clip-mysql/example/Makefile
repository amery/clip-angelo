PRG=my_mysql$(EXESUFF)
all: $(PRG)

clean:
	rm -f *.exe *.uri *.c *.o *.a *.so *.po *.log *.pa *.b *.BAK *.bak \
		*~ core* *core $(PRG) *.ex *.nm

my_mysql$(EXESUFF): my_mysql.prg
	clip$(EXESUFF) -o$@ -eMs my_mysql.prg -lclip-mysql

