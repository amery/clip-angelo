static function rest_clr()
local l:=len(::clrHist)-1
	if l<=0
		aadd(::clrHist, ::__colors[1])
	else
		asize(::clrHist, l)
	endif
	::clr := atail(::clrHist)
return
