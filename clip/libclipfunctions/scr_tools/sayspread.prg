Function SaySpread(cStr, nDelay, nRow, nCol)
	Local n1 := Int(len(cStr)/2), n2, nSleep
	if n1 = 0
		n1 := 1
	endif
	n2 := n1 + 1
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
	nCol -= n1
	while .t.
		if n1 > 0
			DispOutAt(nRow, nCol + n1, Substr(cStr, n1, 1))
		endif
		if n2 <= len(cStr)
			DispOutAt(nRow, nCol + n2, Substr(cStr, n2, 1))
		endif
		Sleep(nSleep)
		n1 --
		n2 ++
		if n1 < 1 .and. n2 > Len(cStr)
			exit
		endif
	enddo
	SetPos(nRow, nCol)
Return ''
