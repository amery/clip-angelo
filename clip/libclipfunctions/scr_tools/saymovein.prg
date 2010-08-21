Function SayMoveIn(cStr, nDelay, nRow, nCol, lDir)
	Local n1 := 1, nSleep
	if nDelay == nil
		nDelay := 4
	endif
	if lDir == nil
		lDir := .f.
	endif
	nSleep := nDelay * 0.001
	if nRow == nil
		nRow := Row()
	endif
	if nCol == nil
		nCol := Col()
	endif
	while n1 <= len(cStr)
		DispOutAt(nRow, if(lDir, nCol+len(cStr)-n1, nCol),;
		iif(lDir, Left(cStr, n1), Right(cStr, n1)))
			Sleep(nSleep)
			n1 ++
	enddo
	SetPos(nRow, nCol)
Return ''
