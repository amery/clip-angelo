LIBS= -lgtk-x11-2.0 -lgdk-x11-2.0 -latk-1.0 -lgdk_pixbuf-2.0 -lm -lpangox-1.0 -lpangoxft-1.0 -lpango-1.0 -lgobject-2.0 -lgmodule-2.0 -ldl -lglib-2.0

SRC =clipgtktest$(EXESUFF) tbrow$(EXESUFF) simple$(EXESUFF)  test_stock$(EXESUFF) test_image$(EXESUFF) accel$(EXESUFF) test_tree$(EXESUFF) test_image$(EXESUFF) \
	test_gtk$(EXESUFF) test_text$(EXESUFF) winpic$(EXESUFF) test$(EXESUFF) note$(EXESUFF) fonttest$(EXESUFF) text$(EXESUFF) container$(EXESUFF) test_dnd$(EXESUFF) drag_and_drop$(EXESUFF) \
        colorbtn$(EXESUFF) pango$(EXESUFF) entrytempl$(EXESUFF) combobox$(EXESUFF) ifactory$(EXESUFF) lines$(EXESUFF) draw$(EXESUFF) lists$(EXESUFF) imgmenu$(EXESUFF)\
        winback$(EXESUFF) test_rc$(EXESUFF) iconview$(EXESUFF)
all: $(SRC)

clean:
	rm -f *.exe *.c *.o *.a *.so *.po *.log *.pa *.b *.BAK *.bak *~ core* *core
	rm -f $(SRC)
	rm -rf *.ex *.nm

clipgtktest$(EXESUFF): clipgtktest.prg
	clip$(EXESUFF) -o$@ -es clipgtktest.prg -lclip-gtk2
#	clip$(EXESUFF) -o$@ -esg clipgtktest.prg ./libclip-gtk2.a -lgtk -lgdk
fonttest$(EXESUFF): fonttest.prg
	clip$(EXESUFF) -o$@ -es fonttest.prg -lclip-gtk2
#	clip$(EXESUFF) -o$@ -esg fonttest.prg ./libclip-gtk2.a $(LIBS)

accel$(EXESUFF): accel.prg
	clip$(EXESUFF) -o$@ -es -M accel.prg -lclip-gtk2

note$(EXESUFF): note.prg
	clip$(EXESUFF) -o$@ -es note.prg -lclip-gtk2

tbrow$(EXESUFF): tbrow.prg
	clip$(EXESUFF) -o$@ -es tbrow.prg -lclip-gtk2

simple$(EXESUFF): simple.prg
	clip$(EXESUFF) -o$@ -es simple.prg -lclip-gtk2

text$(EXESUFF): text.prg
	clip$(EXESUFF) -o$@ -es text.prg -lclip-gtk2
#	clip$(EXESUFF) -o$@ -esg text.prg ./libclip-gtk2.so $(LIBS)

winpic$(EXESUFF): winpic.prg
	clip$(EXESUFF) -o$@ -es winpic.prg -lclip-gtk2

test$(EXESUFF): test.prg
	clip$(EXESUFF) -o$@ -es test.prg -lclip-gtk2

test_gtk$(EXESUFF): test_gtk.prg
	clip$(EXESUFF) -o$@ -es test_gtk.prg -lclip-gtk2
#	clip$(EXESUFF) -o$@ -esg test_gtk.prg ./libclip-gtk2.a -lgtk -lgdk

test_stock$(EXESUFF): test_stock.prg
	clip$(EXESUFF) -o$@ -es test_stock.prg -lclip-gtk2
#	clip$(EXESUFF) -o$@ -egs test_stock.prg ./libclip-gtk2.so $(LIBS)

test_image$(EXESUFF): test_image.prg
	clip$(EXESUFF) -o$@ -es test_image.prg -lclip-gtk2

test_tree$(EXESUFF): test_tree.prg
	clip$(EXESUFF) -o$@ -es test_tree.prg -lclip-gtk2
#	clip$(EXESUFF) -o$@ -esg test_tree.prg ./libclip-gtk2.so $(LIBS)

iconview$(EXESUFF): iconview.prg
	clip$(EXESUFF) -o$@ -es iconview.prg -lclip-gtk2
#	clip$(EXESUFF) -o$@ -esg iconview.prg ./libclip-gtk2.so $(LIBS)

test_text$(EXESUFF): test_text.prg
	clip$(EXESUFF) -o$@ -es test_text.prg -lclip-gtk2
#	clip$(EXESUFF) -o$@ -esg test_text.prg ./libclip-gtk2.so $(LIBS)

container$(EXESUFF): container.prg
	clip$(EXESUFF) -o$@ -es container.prg -lclip-gtk2

test_dnd$(EXESUFF): test_dnd.prg
	clip$(EXESUFF) -o$@ -es test_dnd.prg -lclip-gtk2
#	clip$(EXESUFF) -o$@ -esg test_dnd.prg ./libclip-gtk2.so $(LIBS)

drag_and_drop$(EXESUFF): drag_and_drop.prg
	clip$(EXESUFF) -o$@ -es drag_and_drop.prg -lclip-gtk2
#	clip$(EXESUFF) -o$@ -esg drag_and_drop.prg ./libclip-gtk2.so $(LIBS)

colorbtn$(EXESUFF): colorbtn.prg
	clip$(EXESUFF) -o$@ -es colorbtn.prg -lclip-gtk2

pango$(EXESUFF): pango.prg
	clip$(EXESUFF) -o$@ -es pango.prg -lclip-gtk2

entrytempl$(EXESUFF): entrytempl.prg
	clip$(EXESUFF) -o$@ -es entrytempl.prg -lclip-gtk2

combobox$(EXESUFF): combobox.prg
	clip$(EXESUFF) -o$@ -es combobox.prg -lclip-gtk2

ifactory$(EXESUFF): ifactory.prg
	clip$(EXESUFF) -o$@ -es ifactory.prg -lclip-gtk2

lines$(EXESUFF): lines.prg
	clip$(EXESUFF) -o$@ -es lines.prg -lclip-gtk2

draw$(EXESUFF): draw.prg
	clip$(EXESUFF) -o$@ -es draw.prg -lclip-gtk2

lists$(EXESUFF): lists.prg
	clip$(EXESUFF) -o$@ -es lists.prg -lclip-gtk2

imgmenu$(EXESUFF): imgmenu.prg
	clip$(EXESUFF) -o$@ -es imgmenu.prg -lclip-gtk2
#	clip -esg imgmenu.prg ./libclip-gtk2.so $(LIBS)

winback$(EXESUFF): winback.prg
	clip$(EXESUFF) -o$@ -es winback.prg -lclip-gtk2
#	clip$(EXESUFF) -o$@ -esg winback.prg ./libclip-gtk2.so $(LIBS)

test_rc$(EXESUFF): test_rc.prg
	clip$(EXESUFF) -o$@ -es test_rc.prg -lclip-gtk2
#	clip$(EXESUFF) -o$@ -esg test_rc.prg ./libclip-gtk2.so $(LIBS)


