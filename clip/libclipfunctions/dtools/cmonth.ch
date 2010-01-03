function cmonth(d)
local ret:=''
if valtype(d)=="D"
   ret=monates[month(d)]
endif
return ret
