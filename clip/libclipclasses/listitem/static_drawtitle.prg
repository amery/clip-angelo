static function drawTitle()
local i, nl:=::nLeft
	for i=1 to ::colWin
		dispOutAt(::nTop-1, nl, padc(::title[i], ::lencol[i]), ::__colors[4] )
		if i!=::colWin
			dispOutAt(::nTop-1, nl+::lencol[i], ::delim, ::__colors[1] )
		endif
		nl += ::lencol[i]+1
	next
return .t.
