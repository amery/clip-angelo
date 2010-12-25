Function ClEOL(nRow, nCol)
	Local cScr
	if nRow == nil
		nRow := Row()
	endif
	if nCol == nil
		nCol := Col()
	endif
	cScr := Replicate(' '+Chr(0)+Chr(0), MaxCol()-nCol+1)
	RestScreen(nRow, nCol, nRow, MaxCol(), cScr)
Return ''
