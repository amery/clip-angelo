static function te_printBlock()
local i, str, x1, x2, y1, y2, l
	if !(::strblock .or. ::rectBlock)
		return .f.
	endif
	y1 := min(::koordblock[1], ::koordblock[3])
	y2 := max(::koordblock[1], ::koordblock[3])
	x1 := min(::koordblock[2], ::koordblock[4])
	x2 := max(::koordblock[2], ::koordblock[4])

	if ::strblock
		printBegin()
		for i=y1 to y2
			str := ::edbuffer[i]
			qout(str)
		next
		printEnd()
		return .t.
	endif
	l := x2-x1+1
	printBegin()
	for i=y1 to y2
		str := substr(::edbuffer[i], x1, l)
		qout(str)
	next
	printEnd()
return .t.
