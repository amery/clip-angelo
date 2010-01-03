static function if_eval(p,pc,a)
local r:=eval(p[1])
//return if(r,pc+1,if(p[2]==0,p[3],p[2]+1))
if r == .t.
  return pc+1
endif
if p[2]==0 // IF without ELSE
  return p[3]
endif
if a[p[2]][1]==x_else
  return p[2]+1
endif
r:=a[p[2]][2]
return elif_pc(r,a,p[2])
