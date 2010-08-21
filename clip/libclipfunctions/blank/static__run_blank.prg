static function _run_blank(tex,stdin,tipv,sbros)
static pfile:=""
local str,f1xx,str1,retstr:="",i:=0,n:=0,scr,nsel:=select()
local ret
local prevFlProm
iif(stdin==NIL .or. valtype(stdin)!="L" ,stdin:=.f.,stdin)
private _pfile,_pnumber,_nesc:=0

if stdin
	_pfile:="|-"
	_pnumber=fopen(_pfile,1)
else
	set console off
	if tipv!=NIL .and. tipv="add" .and. empty(pfile)
		pfile=tmpfile()+".txt"
		_pnumber=fcreate(pfile)
		ret:=fclose(_pnumber)
	endif
	if tipv!=NIL .and. tipv="add"
		_pfile=pfile
		_pnumber=fopen(_pfile,2)
		fseek(_pnumber,0,2)
	else
		pfile=""
		_pfile=tmpfile()+".txt"
		_pnumber=fcreate(_pfile)
		ret=fclose(_pnumber)
		_pnumber=fopen(_pfile,2)
		fseek(_pnumber,0,0)
	endif
	save screen to scr
endif
if _pnumber==-1
   if stdin
	outlog(__FILE__,__LINE__,[cannot open file:],_pfile)
   else
	message([cannot open file:]+_pfile)
   endif
   return ""
endif

// ����� ��������� �������� bSkip(), bGoTop()....
if type("__flHaveProm")!="U"
   prevFlProm:=m->__flHaveProm
   if type("__flprom")=="L"
      m->__flHaveProm:=m->__flProm
   endif
endif

str=hardcr(tex)
do while !empty(str)
   f1xx=at(vk,str)
   str1=iif(f1xx!=0,substr(str,1,f1xx+len(vk)-1),str)
   str=iif(f1xx!=0,substr(str,f1xx+len(vk)),"")
   if substr(alltrim(str1),1,1)=="*"
      i++
      loop
   else
      n++
      retstr+=str1
   endif
enddo
if select("blank")!=0
	aaaaaaaaaa(,,,,blank->progress)
endif
if stdin
	aaaaaaaaaa(,,,,.t.)
endif
aaaaaaaaaa(, n+1,"������������� ������")
_run_blank2(retstr,stdin)
ret=fclose(_pnumber)
commit
if stdin
else
	restore screen from scr
	frename(_pfile, strtran(_pfile, "txt", "xtx"))
	frename(strtran(_pfile, "txt", "xtx"), _pfile)
	select(nsel)
endif
IIF (prevFlProm!=NIL, m->__flHaveProm:=prevFlProm,)
return _pfile
