Function ClWin(nTop, nLeft, nBottom, nRight)
	Local cScr
	if nTop == nil
		nTop := Row()
	endif
	if nLeft == nil
		nLeft := Col()
	endif
	if nBottom == nil
		nBottom := MaxRow() - 1
	endif
	if nRight == nil
		nRight := MaxCol() - 1
	endif
	cScr := Replicate(' ' + Chr(0) + Chr(0), (nRight-nLeft+1)*(nBottom-nTop+1))
	RestScreen(nTop, nLeft, nBottom, nRight, cScr)
Return ''
