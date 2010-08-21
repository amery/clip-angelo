static proc ShowScoreboard()
    local nRow, nCol
    if ( Set(_SET_SCOREBOARD) )
	nRow := Row()
	nCol := Col()
	SetPos(SCORE_ROW, SCORE_COL)
	DispOut( if(Set(_SET_INSERT), MSG_INSERT, MSG_OVERWRITE) )  // "Ins","   "
	SetPos(nRow, nCol)
     end
return
