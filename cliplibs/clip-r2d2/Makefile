ifndef CLIPROOT
	CLIPROOT=$(shell cd ../../../; pwd)/cliproot
endif

include $(CLIPROOT)/include/Makefile.inc

#CLIP	= $(CLIPROOT)/bin/clip
CLIPFLAGS = -O -b -w -n -l
OBJS =  \
acc_log.o  \
accpost.o 	\
addinfo.o \
an_funcs.o \
an_rdf.o  \
an_rdf2.o  \
an_rdf3.o  \
balance.o \
balance2.o \
bind2.o \
cgi_funcs.o  \
cgi_funcs2.o  \
classes_rdf.o  \
classes_xml.o  \
connect.o  \
delobj.o  \
getfile.o  \
memo.o  \
obj_arr.o \
obj_rass.o  \
odfreport.o \
print.o  \
rdfarr.o  \
rdfinfo.o  \
rdfinfo2.o  \
rdfinfo3.o \
rdf_sell.o  \
report1.o  \
report3.o  \
report4.o  \
report5.o  \
report6.o \
report7.o  \
report8.o

#rdfinfo2.o  balance2.o

OBJS2 = accpost.o bind.o

R2D2LIB = libclip-r2d2.a
R2D2FLIB = libclip-r2d2$(DLLSUFF)
R2D2FLIBREAL = libclip-r2d2$(DLLREALSUFF)

.SUFFIXES: .prg .o .po

all:    $(R2D2FLIB)

$(R2D2FLIB): $(OBJS)
	clip_makelib$(SCRIPTSUFF) $(R2D2LIB) $(OBJS)
#	$(CLIPROOT)/bin/clip_makeslib$(SCRIPTSUFF) $(R2D2FLIB) $(R2D2LIB) $(C_LIBS)
	clip_makeslib$(SCRIPTSUFF) $(R2D2FLIB)  $(OBJS)

clean:
	rm -f *.c *.s *.o *.po *.log *.bak *.a *.so *.ex *.nm

install: $(R2D2FLIB)
	clip_cp$(SCRIPTSUFF) $(R2D2LIB) $(DESTDIR)$(CLIPROOT)/lib
	clip_cp$(SCRIPTSUFF) $(R2D2FLIB) $(DESTDIR)$(CLIPROOT)/lib
	chmod 0755 $(DESTDIR)$(CLIPROOT)/lib/$(R2D2FLIB)
	cp ./r2d2lib.ch $(DESTDIR)$(CLIPROOT)/include

.prg.o:
	$(CLIP) $(CLIPFLAGS) $<


