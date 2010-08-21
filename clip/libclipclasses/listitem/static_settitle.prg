static function setTitle(Title)
local i
	if empty(Title) .or. valtype(Title)!="A"
		return .f.
	endif
	if empty(::title)
		::rowWin --
		::nTop ++
	endif
	asize(::title, ::colWin)
	afill(::title, "")
	for i=1 to ::colWin
		if len(Title)<i
			exit
		endif
		::title[i] := Title[i]
	next
return .t.
