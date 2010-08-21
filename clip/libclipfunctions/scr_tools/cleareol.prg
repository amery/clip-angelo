Function ClearEOL(nRow, nCol, xAttr, xChar)
	Local cAttr, cChar, cScr
	if nRow == nil
		nRow := Row()
	endif
	if nCol == nil
		nCol := Col()
	endif
	if xAttr == nil
		cAttr := chr(GetClearA())
	elseif ValType(xAttr) = 'N'
		cAttr := Chr(xAttr)
	else
		cAttr := xAttr
	endif
	if xChar == nil
		cChar := chr(GetClearB())
	elseif ValType(xChar) = 'N'
		cChar := Chr(xChar)
	else
		cChar := xChar
	endif
	cScr := Replicate(cChar+cAttr+Chr(0), MaxCol()-nCol+1)
	RestScreen(nRow, nCol, nRow, MaxCol(), cScr)
Return ''
