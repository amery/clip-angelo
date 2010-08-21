***************************************************************
function between(p1,p2,p3)
local ret:=.f.
 if valtype(p1) == valtype(p2) .and. valtype(p1) == valtype(p3)
	if p2<=p3
	   ret=( p2<=p1 .and. p3>=p1)
	else
	   ret=( p2>=p1 .and. p3<=p1)
	endif
 endif
return ret
