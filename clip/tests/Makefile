TESTS = test.dbf \
a$(EXESUFF) \
a_flag$(EXESUFF) \
attoken$(EXESUFF) \
dbg$(EXESUFF) \
dispbox$(EXESUFF) \
dutil$(EXESUFF) \
err$(EXESUFF) \
hparse$(EXESUFF) \
inkey$(EXESUFF) \
key$(EXESUFF) \
loop$(EXESUFF) \
macro1$(EXESUFF) \
main$(EXESUFF) \
mainp.pa \
mathtest$(EXESUFF) \
mem_file$(EXESUFF) \
memoline$(EXESUFF) \
memo_t$(EXESUFF) \
mouse$(EXESUFF) \
obj$(EXESUFF) \
obj_p$(EXESUFF) \
param$(EXESUFF) \
param1$(EXESUFF) \
pg$(EXESUFF) \
pload$(EXESUFF) \
replace$(EXESUFF) \
run$(EXESUFF) \
set_t$(EXESUFF) \
task1$(EXESUFF) \
task2$(EXESUFF) \
task3$(EXESUFF) \
tbldec$(EXESUFF) \
testlock$(EXESUFF) \
transform$(EXESUFF) \
ttt$(EXESUFF) \
ttt_p$(EXESUFF) \
type_field$(EXESUFF) \
wind$(EXESUFF) \
wind2$(EXESUFF) \
wind3$(EXESUFF) \

#loadso$(EXESUFF) param$(EXESUFF) reclock$(EXESUFF)

#CLIP = $(CLIPROOT)/bin/clip
CLIP = clip$(EXESUFF)
#CLPFLAGS = $(CLIPFLAGS)
CLPFLAGS = -v2 -g -l
SF = -s
DBFS = test.dbf ttt.dbf ttt.dbt

all: dbfs $(TESTS)

clean:
	rm -rf *.o *.a *.so *.po *.pa *.b *.BAK *.bak *~ core* *core *.tab.c *.tab.h lex.*.c *.output
	rm -f *.c *.s *.log *.mlog *.pid $(TESTS) $(DBFS) x *.exe *.ex *.nm *.s

dbfs: $(DBFS)

test.dbf: test.dbf.orig
	cp test.dbf.orig test.dbf

ttt.dbf: ttt.dbf.orig
	cp ttt.dbf.orig ttt.dbf

ttt.dbt: ttt.dbt.orig
	cp ttt.dbt.orig ttt.dbt


a$(EXESUFF): a.prg $(CLIPLIB) dbfs
	$(CLIP) $(CLPFLAGS) $(SF) -I. -o$@ -e -M a.prg #aa.o #../a.a

a1.a:   a1.o
	rm -f a1.a
	ar q a1.a a1.o

a1.o:   a1.prg
	$(CLIP) $(CLPFLAGS) a1.prg

a_flag$(EXESUFF): a_flag.prg $(CLIPLIB) #a1.a
	$(CLIP) $(CLPFLAGS) $(SF) -o$@ -e -M -a a_flag.prg #a1.a

attoken$(EXESUFF): attoken.prg $(CLIPLIB)
	$(CLIP) $(CLPFLAGS) $(SF) -o$@ -eslM attoken.prg

dbg$(EXESUFF): dbg.prg $(CLIPLIB)
	$(CLIP) $(CLPFLAGS) $(SF) -o$@ -e -M dbg.prg

dispbox$(EXESUFF): dispbox.prg
	$(CLIP) $(CLPFLAGS) -I. -o$@ -e -M dispbox.prg

dutil$(EXESUFF): dutil.prg $(CLIPLIB)
	$(CLIP) $(CLPFLAGS) -o$@ -e -M dutil.prg -lz

err$(EXESUFF): err.prg $(CLIPLIB) errorsys.o
	$(CLIP) $(CLPFLAGS) $(SF) -o$@ -e -M err.prg errorsys.o

errorsys.o: errorsys.prg
	$(CLIP) $(CLPFLAGS) errorsys.prg

hparse$(EXESUFF): hparse.prg
	$(CLIP) $(CLPFLAGS) -I. -o$@ -e -l -s -M hparse.prg

inkey$(EXESUFF): inkey.prg
	$(CLIP) $(CLPFLAGS) -I. -o$@ -e -l -s -n inkey.prg

key$(EXESUFF): key.prg
	$(CLIP) $(CLPFLAGS) $(SF) -o$@ -e -M key.prg

loadso$(EXESUFF): loadso.prg $(CLIPLIB) obj.so
	$(CLIP) $(CLPFLAGS) $(SF) -o$@ -e -M loadso.prg

loop$(EXESUFF): loop.prg $(CLIPLIB) loop_p.po
	$(CLIP) $(CLPFLAGS) $(SF) -o$@ -e -M loop.prg loop_p.po

loop_p.po: loop_p.prg
	$(CLIP) $(CLPFLAGS) -p loop_p.prg

macro1$(EXESUFF): macro1.prg $(CLIPLIB)
	$(CLIP) $(CLPFLAGS) $(SF) -o$@ -e -M macro1.prg

main$(EXESUFF): $(CLIPLIB)
	$(CLIP) $(CLPFLAGS) $(SF) -o$@ -e -o main mainp.pa

mainp.pa: mainp.po set_p.po mem_file.po
	../tools/clipar$(EXESUFF) mainp.pa mainp.po set_p.po mem_file.po

mainp.po: mainp.prg
	$(CLIP) $(CLPFLAGS) -p mainp.prg

mathtest$(EXESUFF): mathtest.prg
	$(CLIP) $(CLPFLAGS) $(SF) -o$@ -e -M mathtest.prg

mem_file$(EXESUFF): mem_file.prg $(CLIPLIB)
	$(CLIP) $(CLPFLAGS) $(SF) -o$@ -e -M mem_file.prg

mem_file.po: mem_file.prg
	$(CLIP) $(CLPFLAGS) -p mem_file.prg

memo_t$(EXESUFF): memo_t.prg $(CLIPLIB)
	$(CLIP) $(CLPFLAGS) $(SF) -o$@ -e -M memo_t.prg

memoline$(EXESUFF): memoline.prg
	$(CLIP) $(CLPFLAGS) -I. -o$@ -e -l -s -M memoline.prg

mouse$(EXESUFF): mouse.prg
	$(CLIP) $(CLPFLAGS) $(SF) -o$@ -e -M mouse.prg

obj$(EXESUFF): obj.prg $(CLIPLIB)
	$(CLIP) $(CLPFLAGS) $(SF) -o$@ -e -M obj.prg

obj.po: obj.prg
	$(CLIP) $(CLPFLAGS) -p obj.prg

obj.so: obj.prg
	$(CLIP) $(CLPFLAGS) -s obj.prg

obj_p$(EXESUFF): obj_p.prg obj.po
	$(CLIP) $(CLPFLAGS) $(SF) -o$@ -e -M obj_p.prg obj.po

param$(EXESUFF): param.prg $(CLIPLIB)
	$(CLIP) $(CLPFLAGS) $(SF) -o$@ -e -M param.prg

param1$(EXESUFF): param1.prg $(CLIPLIB)
	$(CLIP) $(CLPFLAGS) $(SF) -o$@ -e -M param1.prg

pg$(EXESUFF): pg.prg $(CLIPLIB)
	$(CLIP) $(CLPFLAGS) $(SF) -o$@ -e -M pg.prg

pload$(EXESUFF): pload.prg $(CLIPLIB) obj.po
	$(CLIP) $(CLPFLAGS) $(SF) -o$@ -e -M pload.prg

reclock$(EXESUFF): reclock.prg dbfs
	$(CLIP) $(CLPFLAGS) -I. -o$@ -e -M reclock.prg

replace$(EXESUFF): replace.prg replacep.po $(CLIPLIB)
	$(CLIP) $(CLPFLAGS) $(SF) -o$@ -e -M replace.prg replacep.po

replacep.po: replacep.prg
	$(CLIP) $(CLPFLAGS) -p replacep.prg

run$(EXESUFF): run.prg $(CLIPLIB)
	$(CLIP) $(CLPFLAGS) $(SF) -o$@ -e -M run.prg

set_p.po: set_p.prg
	$(CLIP) $(CLPFLAGS) -p set_p.prg

set_t$(EXESUFF): set_t.prg set_p.po
	$(CLIP) $(CLPFLAGS) $(SF) -o$@ -e -M set_t.prg set_p.po

task1$(EXESUFF): task1.prg $(CLIPLIB)
	$(CLIP) $(CLPFLAGS) $(SF) -I. -o$@ -e -M task1.prg

task2$(EXESUFF): task2.prg $(CLIPLIB)
	$(CLIP) $(CLPFLAGS) $(SF) -I. -o$@ -e -M task2.prg

task3$(EXESUFF): task3.prg $(CLIPLIB)
	$(CLIP) $(CLPFLAGS) $(SF) -I. -o$@ -e -M task3.prg

tbldec$(EXESUFF): tbldec.prg
	$(CLIP) $(CLPFLAGS) $(SF) -o$@ -e -M tbldec.prg

testlock$(EXESUFF): testlock.prg
	$(CLIP) $(CLPFLAGS) -I. -o$@ -e -M testlock.prg

ttt$(EXESUFF): ttt.prg $(CLIPLIB) test.dbf
	$(CLIP) $(CLPFLAGS) $(SF) -o$@ -e -M ttt.prg

ttt_p$(EXESUFF): ttt_p.prg ttt.po
	$(CLIP) $(CLPFLAGS) $(SF) -o$@ -e -M ttt_p.prg ttt.po

ttt.po: ttt.prg
	$(CLIP) $(CLPFLAGS) -p ttt.prg

type_field$(EXESUFF): type_field.prg $(CLIPLIB)
	$(CLIP) $(CLPFLAGS) $(SF) -o$@ -e -M type_field.prg

transform$(EXESUFF): transform.prg
	$(CLIP) $(CLPFLAGS) -I. -o$@ -e -l -s -M transform.prg

wind$(EXESUFF): wind.prg $(CLIPLIB)
	$(CLIP) $(CLPFLAGS) $(SF) -o$@ -e -M wind.prg

wind2$(EXESUFF): wind2.prg $(CLIPLIB)
	$(CLIP) $(CLPFLAGS) $(SF) -o$@ -e -M wind2.prg

wind3$(EXESUFF): wind3.prg $(CLIPLIB)
	$(CLIP) $(CLPFLAGS) $(SF) -o$@ -e -M wind3.prg

x$(EXESUFF): x.prg $(CLIPLIB)
#	$(CLIP) $(CLPFLAGS) -e -M x.prg ../libxclip.a `gtk-config --libs` -lz
	$(CLIP) $(CLPFLAGS) -o$@ -e -M x.prg -lxclip -lz  `gtk-config --libs`
