Function SayDown(cStr, nDelay, nRow, nCol)
	Local ser, nSleep
	if nDelay == nil
		nDelay := 4
	endif
	nSleep := nDelay * 0.001
	if nRow == nil
		nRow := Row()
	endif
	if nCol == nil
		nCol := Col()
	endif
	for ser := 1 to len(cStr)
		DispOutAt(nRow+ser-1, nCol, Substr(cStr, ser, 1))
		Sleep(nSleep)
	next
	SetPos(nRow, nCol)
Return ''
