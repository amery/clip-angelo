SRC=accel$(EXESUFF) clipgtktest$(EXESUFF) note$(EXESUFF) simple$(EXESUFF) tbrow$(EXESUFF) test$(EXESUFF) test_gtk$(EXESUFF) test_image$(EXESUFF) \
	test_list$(EXESUFF) window$(EXESUFF)  winpic$(EXESUFF) ifactory$(EXESUFF)
all: $(SRC)

clean:
	rm -f *.exe *.c *.o *.a *.so *.po *.log *.pa *.b *.BAK *.bak *~ core* *core
	rm -f $(SRC)
	rm -rf *.ex *.nm

clipgtktest$(EXESUFF): clipgtktest.prg
	clip$(EXESUFF) -o$@  -es clipgtktest.prg -lclip-gtk
#	clip$(EXESUFF) -o$@  -esg clipgtktest.prg ./libclip-gtk.so -lgtk -lgdk

accel$(EXESUFF): accel.prg
	clip$(EXESUFF) -o$@  -es -M accel.prg -lclip-gtk

note$(EXESUFF): note.prg
	clip$(EXESUFF) -o$@  -es -M note.prg -lclip-gtk

tbrow$(EXESUFF): tbrow.prg
	clip$(EXESUFF) -o$@  -es -M tbrow.prg -lclip-gtk

simple$(EXESUFF): simple.prg
	clip$(EXESUFF) -o$@  -es simple.prg -lclip-gtk

#ext: ext.prg
#	clip$(EXESUFF) -o$@  -es ext.prg -lclip-gtk

winpic$(EXESUFF): winpic.prg
	clip$(EXESUFF) -o$@  -es winpic.prg -lclip-gtk

#winpic1$(EXESUFF): winpic1.prg
#	clip$(EXESUFF) -o$@  -es winpic1.prg -lclip-gtk

window$(EXESUFF): window.prg
	clip$(EXESUFF) -o$@  -es window.prg -lclip-gtk

test_gtk$(EXESUFF): test_gtk.prg
	clip$(EXESUFF) -o$@  -es test_gtk.prg -lclip-gtk
#	clip$(EXESUFF) -o$@  -esg test_gtk.prg ./libclip-gtk.a -lgtk -lgdk

test_image$(EXESUFF): test_image.prg
	clip$(EXESUFF) -o$@  -es test_image.prg -lclip-gtk

test_list$(EXESUFF): test_list.prg
	clip$(EXESUFF) -o$@  -es test_list.prg -lclip-gtk

test$(EXESUFF): test.prg
	clip$(EXESUFF) -o$@  -es test.prg -lclip-gtk

ifactory$(EXESUFF): ifactory.prg
	clip$(EXESUFF) -o$@  -es ifactory.prg -lclip-gtk
#	clip$(EXESUFF) -o$@ -esg ifactory.prg ./libclip-gtk.so -lgtk -lgdk


