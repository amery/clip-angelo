static function apop(a)
local v
if len(a)==0
  return nil
endif
v:=a[len(a)]
adel(a,len(a))
asize(a,len(a)-1)
return v
