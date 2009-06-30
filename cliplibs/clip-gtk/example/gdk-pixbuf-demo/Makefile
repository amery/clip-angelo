SRC=pixbuf_demo$(EXESUFF) pixbuf_scale$(EXESUFF) zoomimage$(EXESUFF)

all: $(SRC)

clean:
	rm -f *.c *.o *.log *.bak
	rm -f $(SRC)
	rm -rf *.ex *.nm

pixbuf_demo$(EXESUFF) : pixbuf_demo.prg
	clip$(EXESUFF) -o$@ -ews pixbuf_demo.prg -lclip-gtk

pixbuf_scale$(EXESUFF) : pixbuf_scale.prg
	clip$(EXESUFF) -o$@ -ews pixbuf_scale.prg -lclip-gtk

zoomimage$(EXESUFF) : zoomimage.prg
	clip$(EXESUFF) -o$@ -ews zoomimage.prg -lclip-gtk

