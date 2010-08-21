static function new_clr(newColor)
	aadd(::clrHist, newColor)
	::clr := atail(::clrHist)
return
