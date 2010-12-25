static function for_eval(p,pc)
local i_v:=eval(p[1])
i_v:=eval(p[2])
if !i_v
  return p[4]+1
endif
return pc+1
