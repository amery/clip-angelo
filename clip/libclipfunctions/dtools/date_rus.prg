function date_rus(var)
local str:="",ccc,str_mon
if empty(var) .and. valtype(var)="D"
   return "___ ___________ _____"
endif

if valtype(var)=='N'
   if var<1 .or. var>12
      str=space(len(monates[1]))
   else
      str=monates[var]
   endif
endif
if valtype(var)=='D'
   str_mon=" "+alltrim(monates[month(var)])
   if month(var)==3 .or. month(var)==8
      ccc="�"
      str_mon=rtrim(str_mon)+ccc
   else
      ccc="�"
      str_mon=substr(str_mon,1,len(str_mon)-1)+ccc
   endif
   str=str(day(var),2,0)+" "+str_mon+" "+str(year(var),4,0)+" �."
endif
return str
