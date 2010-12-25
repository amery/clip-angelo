Function ColorRepl(xNewAttr, xOldAttr)
	Local cNewAttr, cOldAttr, cScr, ser, np
	if xNewAttr == nil
		cNewAttr := GetClearA()
	elseif ValType(xNewAttr) = 'N'
		cNewAttr := Chr(xNewAttr)
	else
		cNewAttr := xNewAttr
	endif
	if xOldAttr # nil
		cOldAttr := if(ValType(xOldAttr)='N', Chr(xOldAttr), xOldAttr)
	endif
	cScr := SaveScreen(0, 0, MaxRow(), MaxCol())
	for ser := 1 to len(cScr) / 3
		np := (ser - 1)*3 + 2
		if cOldAttr == nil .or. Substr(cScr, np, 1) == cOldAttr
			cScr := Stuff(cScr, np, 1, cNewAttr)
		endif
	next
	RestScreen(0, 0, MaxRow(), MaxCol(), cScr)
Return ''
