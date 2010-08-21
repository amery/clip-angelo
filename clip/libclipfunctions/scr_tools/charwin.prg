Function CharWin(nTop, nLeft, nBottom, nRight, xNewChar, xOldChar)
	Local cScr, cNewChar, cOldChar, ser
	if nTop == nil
		nTop := 0
	endif
	if nLeft == nil
		nLeft := 0
	endif
	if nBottom == nil
		nBottom := MaxRow()
	endif
	if nRight == nil
		nRight := MaxCol()
	endif
	if xNewChar == nil
		cNewChar := GetClearB()
	elseif ValType(xNewChar) = 'N'
		cNewChar := Chr(xNewChar)
	else
		cNewChar := xNewChar
	endif
	if xOldChar # nil
		cOldChar := if(ValType(xOldChar)='N', Chr(xOldChar), xOldChar)
	endif
	cScr := SaveScreen(nTop, nLeft, nBottom, nRight)
	for ser := 1 to len(cScr) / 3
		if cOldChar == nil .or. Substr(cScr, (ser-1)*3+1, 1) == cOldChar
			cScr := Stuff(cScr, (ser-1)*3+1, 1, cNewChar)
		endif
	next
	RestScreen(nTop, nLeft, nBottom, nRight, cScr)
Return ''
