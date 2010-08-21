static function block(s,a)
local cVars:= "" ,i,lfirst := .t.
if ! empty(a)
 for i := 1 to len(a)
  cVars+=a[i]+","
 next
 cVars := left(cVars,len(cVars)-1)
endif
return &("{|"+cVars+"|"+s+"}")
