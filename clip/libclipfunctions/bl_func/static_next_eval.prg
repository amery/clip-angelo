static function next_eval(p,pc,a)
local v
eval(a[p][2][3])
v:=eval(a[p][2][2])
if !v
  return a[p][2][4]+1
endif
return p+1
