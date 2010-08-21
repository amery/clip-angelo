function dmy(d,lMode)
local str
if valtype(d)="L"
  lMode:=d
  d:=NIL
endif
d:=iif(d==NIL,date(),d)
lMode:=iif(lMode==NIL,.f.,lMode)
str:=alltrim(monates[month(d)])
if "ru" $ lower(getenv("LANG")) .or. "ru" $ lower(getenv("CLIP_LANG"))
    if month(d)==3 .or. month(d)==8
      str:=str+"�"
    else
      str:=left(str,len(str)-1)+"�"
    endif
endif

str:=str(day(d),2,0)+;
iif(lMode,".","")+" "+;
str+" "+;
Right(str(year(d),4,0),iif(__SetCentury(),4,2))
return str
