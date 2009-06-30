all:sheet$(EXESUFF)

clean:
	rm -f *.exe *.c *.o *.a *.so *.po *.log *.pa *.b *.BAK *.bak *~ core* *core sheet$(EXESUFF)
	rm -f *.ex *.nm

sheet$(EXESUFF): sheet.prg
	clip$(EXESUFF) -o$@ -e -s -l sheet.prg -lclip-gtkextra -lclip-gtk

