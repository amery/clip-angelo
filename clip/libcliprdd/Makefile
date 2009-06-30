CFLAGS += -I.. -I../include -I../include.h
LIB = libcliprdd.a
OBJS = rddclip.o rdd.o dbf.o ntx.o cdx.o dbt.o fpt.o rushmore.o btree.o six.o hiper.o

install all:	$(LIB)

clean:
	rm -f *.o *.a *.b *.bak *.BAK *.tmp *.ex *.nm \
	test_ref.c test.c test.log \
	*.dbf *.ntx *.cdx *.dbt *.fpt \
	$(LIB) test

$(LIB): $(OBJS)
	rm -f $(LIB)
	clip_makelib$(SCRIPTSUFF) $(LIB) $(OBJS) && ranlib $(LIB)

rddclip.o:	rddclip.c ../include.h/rdd.h
rdd.o:		rdd.c ../include.h/rdd.h
dbf.o:		dbf.c ../include.h/rdd.h
ntx.o:		ntx.c ../include.h/rdd.h
cdx.o:		cdx.c ../include.h/rdd.h
dbt.o:		dbt.c ../include.h/rdd.h
fpt.o:		fpt.c ../include.h/rdd.h
rushmore.o:	rushmore.c ../include.h/rdd.h
btree.o:	btree.c btree.h
six.o:		six.c ../include.h/rdd.h ../include/six.ch
hiper.o:	hiper.c ../include.h/rdd.h btree.h

test: test.prg rddclip.o rdd.o dbf.o ntx.o cdx.o dbt.o fpt.o rushmore.o btree.o six.o hiper.o ../clipbase.h ../clipbase.c ../libclip.a
	clip -e -R test.prg
