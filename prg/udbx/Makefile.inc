#	Created automatically by configure
#

#
#	Compile flags
#
PIXBUFINCLUDEDIRS=-I/usr/include/gdk-pixbuf-1.0 -I/usr/include/gtk-1.2 -I/usr/include/glib-1.2 -I/usr/lib64/glib/include -I/usr/include
INCLUDEDIRS=$(PIXBUFINCLUDEDIRS)
C_FLAGS+= $(INCLUDEDIRS) 
LIBS=-L/usr/lib64 -lgtk -lgdk -lgmodule -lglib -ldl -lXi -lXext -lX11 -lm
PIXBUFLIBS=-L/usr/lib64 -lgdk_pixbuf
PIXBUFSRC=gdkpixbuf.c gdkrgb.c
PIXBUFOBJ=gdkpixbuf.o gdkrgb.o
XCFLAGS=-I/usr/include/gtk-1.2 -I/usr/include/glib-1.2 -I/usr/lib64/glib/include -I/usr/include -I/usr/include/gdk-pixbuf-1.0 -I/usr/include/gtk-1.2 -I/usr/include/glib-1.2 -I/usr/lib64/glib/include -I/usr/include

