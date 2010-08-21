func ColorWin(nTop, nLeft, nBottom, nRight, xAttr, xOld)
	Local cScr, nAttr, cAttr, nOld, ser, np,x:=3

#ifdef CLIP_DOS_SCRBUF
	x:=2
#endif

	if nTop == nil
		nTop := Row()
	endif
	if nLeft == nil
		nLeft := Col()
	endif
	if nBottom == nil
		nBottom := MaxRow()
	endif
	if nRight == nil
		nRight := MaxCol()
	endif
	if xAttr == nil
		nAttr := GetClearA()
	elseif ValType(xAttr) = 'C'
		nAttr := ColorToN(xAttr)
	else
		nAttr := xAttr
	endif
	if xOld # nil
		nOld := if(ValType(xOld) = 'C', ColorToN(xOld), xOld)
	endif

	cScr := SaveScreen(nTop, nLeft, nBottom, nRight)

	for ser := 1 to len(cScr) / x
		np := (ser-1)*x + 2
		cAttr := Substr(cScr, np, 1)
		if nOld == nil .or. nOld == Asc(cAttr)
			cScr := Stuff(cScr, np, 1, Chr(nAttr))
		endif
	next

	RestScreen(nTop, nLeft, nBottom, nRight, cScr)
Return ''
