static function while_eval(p,pc)
local r:=eval(p[1])
if !r
  return p[2]+1
endif
return pc+1
