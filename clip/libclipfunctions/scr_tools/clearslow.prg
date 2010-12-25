Function ClearSlow(nDelay, nTop, nLeft, nBottom, nRight, xChar)
	Local cChar, nt, nl, nb, nr, ser
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
	if xChar == nil
		cChar := GetClearB()
	elseif ValType(xChar) = 'N'
		cChar := Chr(xChar)
	else
		cChar := xChar
	endif
	nt := nTop
	nl := nLeft
	nb := nBottom
	nr := nRight
	while nt < nb .or. nl < nr
		if nl < nr
			DispOutAt(nt, nl, Replicate(cChar, nr-nl+1))
			DispOutAt(nb, nl, Replicate(cChar, nr-nl+1))
		endif
		if nt < nb
			nt ++
			nb --
		endif
		if nt <= nb
			for ser := nt to nb
				DispOutAt(ser, nl, cChar)
				DispOutAt(ser, nr, cChar)
			next
		endif
		Sleep(nDelay*0.001)
		if nl < nr
			nl ++
			nr --
		endif
	enddo
Return ''
