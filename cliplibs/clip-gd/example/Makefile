PRG=gr$(EXESUFF)
all: $(PRG)

clean:
	rm -f *.exe *.uri *.c *.o *.a *.so *.po *.log *.pa *.b *.BAK *.bak \
		*~ core* *core $(PRG) *.ex *.nm

gr$(EXESUFF): gr.prg
	clip$(EXESUFF) -o$@ -s -e -M gr.prg -lclip-gd

