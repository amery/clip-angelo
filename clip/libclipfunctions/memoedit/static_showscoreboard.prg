static proc ShowScoreboard()
    local nRow, nCol
    if ( Set(_SET_SCOREBOARD) )
	nRow := Row()
	nCol := Col()
	SetPos(TE_SCORE_ROW, TE_SCORE_COL)
	DispOut( if(Set(_SET_INSERT), [<Ins>], [<Ovr>]) )
	SetPos(nRow, nCol)
     end
return
