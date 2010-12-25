static function elif_pc(p,a,pc)
local r:=eval(p[1])
if !(r==.t.)
  if (p[2]<>0 .and. a[p[2]][1]==x_elif) // there are more ELIF
    return elif_pc(a[p[2]][2],a,p[2])
  elseif p[2]<>0 // there are not ELIF, but ELSE
    return p[2]+1
  else // only ENDIF
    return a[p[5]][2][3]
  endif
endif
return pc+1
