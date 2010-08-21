func ClearWin(nTop, nLeft, nBottom, nRight, xAttr, xChar)
	Local cChar, nAttr, cScr
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

	if xChar == nil
		cChar := GetClearB()
	elseif ValType(xChar) = 'N' .and. xChar >= 0 .and. xChar <= 255
		cChar := Chr(xChar)
	else
		cChar := xChar
	endif

	if xAttr == nil
		nAttr := GetClearA()
	elseif ValType(xAttr) = 'C'
		nAttr := ColorToN(xAttr)
	else
		nAttr := xAttr
	endif
	cScr := Replicate(cChar + Chr(nAttr) + Chr(0),;
		(nRight-nLeft+1)*(nBottom-nTop+1))
	RestScreen(nTop, nLeft, nBottom, nRight, cScr)
Return ''
