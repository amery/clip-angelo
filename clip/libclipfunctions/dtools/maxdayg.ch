function maxdayg(xxx)
local i,x
if valtype(xxx)=="N"
   x=ctod("01/01/"+alltrim(str(xxx,4,0)))
endif
if valtype(xxx)=="D"
  x=xxx
endif
  for i=360 to 368
    if day(x+i)==day(x)
       return i
    endif
  next
return 0
