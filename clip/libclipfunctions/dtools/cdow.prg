function cdow(var)
local str:="",dn
if valtype(var)=='D'
   dn=dow(var)
   str=days[dn]
endif
if valtype(var)=='N' .and. var>0 .and. var<8
      str=days[var]
endif
return str
