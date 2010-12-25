function ntocmonth(d)
local ret:=''
if valtype(d)=="N" .and. d>0 .and. d<13
   ret=monates[d]
endif
return ret
