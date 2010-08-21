Function SayScreen(cStr, nRow, nCol)
	Local cScr, ser, x:=3
#ifdef CLIP_DOS_SCRBUF
	x:=2
#endif
	if nRow == nil
		nRow := Row()
	endif
	if nCol == nil
		nCol := Col()
	endif
	cScr := SaveScreen(nRow, nCol, nRow, nCol + Len(cStr) - 1)
	for ser := 1 to len(cStr)
		cScr := Stuff(cScr, (ser-1)*x+1, 1, Substr(cStr, ser, 1))
	next
	RestScreen(nRow, nCol, nRow, nCol + Len(cStr) - 1, cScr)
Return ''
