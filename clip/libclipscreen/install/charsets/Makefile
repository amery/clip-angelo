#
#                                            clip/libclipscreen/install/charsets/Makefile.in
#
Local_Depend =

all: gen_tbl$(EXESUFF)
CFLAGS=-g

gen_tbl$(EXESUFF): gen_tbl.o
	$(Trace_c)
	$(CC) $(CFLAGS) -o $@ gen_tbl.o

gen_tbl.o: gen_tbl.c


clean:
	rm -f gen_tbl$(EXESUFF) *.o *.b *.bak *.BAK
